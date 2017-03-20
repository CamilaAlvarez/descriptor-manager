//
// Created by Camila Alvarez on 13-03-17.
//

#include "descriptor_manager/descriptor_manager.h"
#ifdef HAS_GLOG
    #include "glog/logging.h"
#endif

namespace descriptor {

    float *DescriptorManager::calculateDescriptorForImage(const cv::Mat &image) {
        caffe::Datum datum;
        caffe::CVMatToDatum(image, &datum);
        return calculateDescriptorForDatum(datum);
    }

    float *DescriptorManager::calculateDescriptorForDatum(const caffe::Datum &datum) {
        Descriptor descriptor_object = calculateDescriptorForDatum(datum, "");
        return descriptor_object.getDescriptor();
    }

    Descriptor DescriptorManager::calculateDescriptorForImage(const cv::Mat &image, const std::string &image_id,
                                                              const std::string &image_class) {
        caffe::Datum datum;
        caffe::CVMatToDatum(image, &datum);
        return calculateDescriptorForDatum(datum, image_id, image_class);
    }

    Descriptor DescriptorManager::calculateDescriptorForDatum(const caffe::Datum &datum, const std::string &image_id,
                                                              const std::string &image_class) {
        std::vector<int> image_shape;
        image_shape.push_back(1);
        image_shape.push_back(datum.channels());
        image_shape.push_back(datum.height());
        image_shape.push_back(datum.width());
        caffe::Blob<float> *blob = new caffe::Blob<float>(image_shape);
        std::vector<caffe::Blob<float> *> image_blob;
        image_blob.push_back(blob);
        float loss;
        net.Forward(image_blob, &loss);
#if HAS_LOG
        CHECK(config_file.hasKey("LAYER")) << "MISSING LAYER PARAMETER";
#endif
        const boost::shared_ptr<caffe::Blob<float>> &descriptor_blob = net.blob_by_name(
                config_file.getValueForKey("LAYER"));
        const float *descriptor_data = descriptor_blob->cpu_data();
        int dimension = descriptor_blob->count();
        float *descriptor = new float[dimension];
        std::copy(descriptor_data, descriptor_data + dimension, descriptor);

        return Descriptor(image_id, descriptor, dimension, image_class);
    }

    Descriptors DescriptorManager::calculateDescriptorsForImagesInFile(const std::string &images_file,
                                                                       const std::string &separator,
                                                                       int number_images_per_line,
                                                                       int total_number_channels) {
        ImageFile image_file(images_file, separator, number_images_per_line, total_number_channels);
        return calculateDescriptorsForImagesInFile(image_file);
    }

    Descriptors DescriptorManager::calculateDescriptorsForImagesInFile(ImageFile image_file) {
        int number_images = image_file.getNumberOfImages();
#if HAS_LOG
        CHECK(number_images > 0) << "THERE ARE NO IMAGES";
#endif
        Descriptors descriptors(number_images);
        int descriptor_size = 0;
        for (int i = 0; i < number_images; i++) {
            caffe::Datum image = image_file.getImageDatum(i);
            std::string image_id = image_file.getImageId(i);
            Descriptor d = calculateDescriptorForDatum(image, image_id);
            descriptor_size = d.getDescriptorSize();
            descriptors.addDescriptor(image_id, d);
        }
        descriptors.setDescriptorSize(descriptor_size);
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
                                                                        const std::string &separator,
                                                                        int number_images_per_line,
                                                                        int total_number_channels,
                                                                        const std::string &outfile) {
        Descriptors descriptors = calculateDescriptorsForImagesInFile(images_file, separator, number_images_per_line,
                                                                      total_number_channels);
        descriptors.writeDescriptorsToFile(outfile);


    }
}