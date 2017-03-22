//
// Created by Camila Alvarez on 13-03-17.
//

#include "descriptor_manager/descriptor_manager.h"
#include "caffe/layers/memory_data_layer.hpp"
#ifdef HAS_GLOG
    #include "glog/logging.h"
#endif

namespace descriptor {

    DescriptorManager::DescriptorManager(const ConfigFile &config_file){
        init(config_file);
    }

    DescriptorManager::DescriptorManager(const std::string &network_config_file, const std::string &separator) {
        ConfigFile config_file = ConfigFile(network_config_file, separator);
        init(config_file);
    }

    float *DescriptorManager::calculateDescriptorForImage(const cv::Mat &image, bool normalized) {
        caffe::Datum datum;
        cv::Mat output_image;
        cv::resize(image, output_image, expected_image_size, 0, 0, cv::INTER_CUBIC);
        caffe::CVMatToDatum(output_image, &datum);
        return calculateDescriptorForDatum(datum, normalized);
    }

    float *DescriptorManager::calculateDescriptorForDatum(const caffe::Datum &datum, bool normalized) {

        Descriptor descriptor_object = calculateDescriptorForDatum(datum, "", normalized);
        return descriptor_object.getDescriptor();
    }

    Descriptor DescriptorManager::calculateDescriptorForImage(const cv::Mat &image, const std::string &image_id,
                                                              bool normalized, const std::string &image_class) {
        caffe::Datum datum;
        cv::Mat output_image;
        cv::resize(image, output_image, expected_image_size, 0, 0, cv::INTER_CUBIC);
        caffe::CVMatToDatum(output_image, &datum);
        return calculateDescriptorForDatum(datum, image_id, normalized, image_class);
    }

    Descriptor DescriptorManager::calculateDescriptorForDatum(const caffe::Datum &datum, const std::string &image_id,
                                                              bool normalized,
                                                              const std::string &image_class) {
#ifdef HAS_LOG
        CHECK(datum.height() == expected_image_size.height && datum.width() == expected_image_size.width) << "INVALID IMAGE DIMENSIONS. "+
                std::to_string(datum.height())+"!=" +std::to_string(expected_image_size.height)+" or " + std::to_string(datum.width())
                +"!="+std::to_string(expected_image_size.width);
        LOG(INFO) << "CALCULATING DESCRIPTOR";
#endif
        std::vector<caffe::Datum> datum_vector;
        datum_vector.push_back(datum);
        float loss;
        const boost::shared_ptr<caffe::Layer<float> > memory_layer = net->layer_by_name(memory_data_layer_name);
        const boost::shared_ptr<caffe::MemoryDataLayer<float>> memory_data_layer =
                boost::static_pointer_cast<caffe::MemoryDataLayer<float>>(memory_layer);
        memory_data_layer->AddDatumVector(datum_vector);
        net->Forward(&loss);
        const boost::shared_ptr<caffe::Blob<float>> &descriptor_blob = net->blob_by_name(
                extractor_layer);
        const float *descriptor_data = descriptor_blob->cpu_data();
        int dimension = descriptor_blob->count();
        float *descriptor = new float[dimension];
        std::copy(descriptor_data, descriptor_data + dimension, descriptor);
#ifdef HAS_LOG
        LOG(INFO) << "FINISHED CALCULATING DESCRIPTOR";
#endif
        if(normalized){
            float norm = 0;
            for (int i = 0; i < dimension ; ++i) {
                int sign = 1;
                if(descriptor[i] < 0) sign = -1;
                descriptor[i] = sign*std::sqrt(std::abs(descriptor[i]));
                norm += descriptor[i]*descriptor[i];
            }
            if(norm != 0){
                for (int i = 0; i < dimension ; ++i) {
                    descriptor[i] = descriptor[i]/norm;
                }
            }
        }
        return Descriptor(image_id, descriptor, dimension, image_class);
    }

    Descriptors DescriptorManager::calculateDescriptorsForImagesInFile(const std::string &images_file,
                                                                       bool normalized,
                                                                       const std::string &separator,
                                                                       int number_images_per_line,
                                                                       int total_number_channels) {
        ImageFile image_file(images_file, expected_image_size, total_number_channels,
                             number_images_per_line, separator);
        return calculateDescriptorsForImagesInFile(image_file, normalized);
    }

    Descriptors DescriptorManager::calculateDescriptorsForImagesInFile(ImageFile image_file,
                                                                       bool normalized) {
        int number_images = image_file.getNumberOfImages();
#ifdef HAS_LOG
        CHECK(number_images > 0) << "THERE ARE NO IMAGES";
        LOG(INFO) << "CALCULATING DESCRIPTORS FOR IMAGES IN FILE";
#endif
        Descriptors descriptors(number_images);
        int descriptor_size = 0;
        for (int i = 0; i < number_images; i++) {
            caffe::Datum image = image_file.getImageDatum(i);
            std::string image_id = image_file.getImageId(i);
            std::string image_class = image_file.getImageClass(i);
            Descriptor d = calculateDescriptorForDatum(image, image_id, normalized, image_class);
            descriptor_size = d.getDescriptorSize();
            descriptors.addDescriptor(image_id, d);
        }
        descriptors.setDescriptorSize(descriptor_size);
#ifdef HAS_LOG
        LOG(INFO) << "FINISHED CALCULATING DESCRIPTORS FOR IMAGES IN FILE";
#endif
        return descriptors;
    }

/*
 * <n_items:int>
 * <dim:int>
 * <
 *      <image_name_size:int>
 *      <image_class_size:int>
 *      <name:char[image_name_size]>
 *      <class:char[image_class_size]>
 *      <desriptor:float[dim]>
 * > [n_items]
 */
    void DescriptorManager::calculateAndWriteDescriptorsForImagesInFile(const std::string &images_file,
                                                                        const std::string &outfile,
                                                                        bool normalized,
                                                                        const std::string &separator,
                                                                        int number_images_per_line,
                                                                        int total_number_channels) {
        Descriptors descriptors = calculateDescriptorsForImagesInFile(images_file, normalized, separator,
                                                                      number_images_per_line,
                                                                      total_number_channels);
        descriptors.writeDescriptorsToFile(outfile);


    }

    void DescriptorManager::init(ConfigFile config_file) {
#ifdef HAS_LOG
        CHECK(config_file.hasKey("LAYER")) << "MISSING NETWORK EXTRACTOR LAYER. PARAMETER: LAYER";
#endif
        extractor_layer = config_file.getValueForKey("LAYER");

        if(config_file.hasKey("MEMORY_LAYER")){
            memory_data_layer_name = config_file.getValueForKey("LAYER");
        }
        else{
            memory_data_layer_name = "data";
        }
#ifdef HAS_LOG
        CHECK(config_file.hasKey("IMAGE_HEIGHT")) << "MISSING IMAGE HEIGHT. PARAMETER: IMAGE_HEIGHT";
        CHECK(config_file.hasKey("IMAGE_WIDTH")) << "MISSING IMAGE WIDTH. PARAMETER: IMAGE_WIDTH";
#endif
#ifdef HAS_LOG
        CHECK(config_file.hasKey("ARCHITECTURE")) << "MISSING NETWORK ARCHITECTURE FILE. PARAMETER: ARCHITECTURE";
#endif
        std::string architecture = config_file.getValueForKey("ARCHITECTURE");
#ifdef HAS_LOG
        CHECK(config_file.hasKey("MODEL")) << "MISSING NETWORK MODEL FILE. PARAMETER: MODEL";
#endif
        try {
            int image_height = std::stoi(config_file.getValueForKey("IMAGE_HEIGHT"));
            int image_width = std::stoi(config_file.getValueForKey("IMAGE_WIDTH"));
            expected_image_size = cv::Size(image_width, image_height);
        }
        catch (const std::invalid_argument& e){
            throw  std::runtime_error("INVALID HEIGHT OR WIDTH");
        }

        std::string model = config_file.getValueForKey("MODEL");
        if(config_file.hasKey("BASE_DIR")){
            std::string base_dir = config_file.getValueForKey("BASE_DIR");
            model = base_dir+"/"+model;
            architecture = base_dir+"/"+architecture;
        }

#ifdef CPU_ONLY
        caffe::Caffe::set_mode(caffe::Caffe::CPU);
#else
        caffe::Caffe::set_mode(caffe::Caffe::GPU);
#endif
#ifdef HAS_LOG
        LOG(INFO) << "STARTING NETWORK LOADING";
#endif
        net = new caffe::Net<float>(architecture, caffe::TEST);
        net->CopyTrainedLayersFrom(model);
#ifdef HAS_LOG
        LOG(INFO) << "NETWORK FINISHED LOADING";
#endif
    }

    cv::Size DescriptorManager::getExpectedImageSize() {
        return expected_image_size;
    }
}