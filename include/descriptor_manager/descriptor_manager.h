//
// Created by Camila Alvarez on 13-03-17.
//

#ifndef DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP
#define DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP

#include <iostream>
#include "caffe/caffe.hpp"
#include "utils/config_file.h"
#include "descriptor_manager/descriptor.h"
#include "descriptor_manager/descriptors.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "utils/image_file.h"

namespace descriptor {
    class DescriptorManager {

    public:
        explicit DescriptorManager(const std::string &network_config_file, const std::string& separator);

        explicit DescriptorManager(const ConfigFile& config_file);

        float *calculateDescriptorForImage(const cv::Mat &image, bool normalized=true);

        float *calculateDescriptorForDatum(const caffe::Datum &datum, bool normalized=true);

        Descriptor calculateDescriptorForDatum(const caffe::Datum &datum, const std::string &image_id,
                                               bool normalized=true,
                                               const std::string &image_class = "0");

        Descriptor calculateDescriptorForImage(const cv::Mat &image, const std::string &image_id,
                                               bool normalized=true,
                                               const std::string &image_class = "0");

        Descriptors calculateDescriptorsForImagesInFile(const std::string &images_file, bool normalized=true,
                                                        const std::string &separator = "\t",
                                                        int number_images_per_line = 1, int total_number_channels = 3);

        Descriptors calculateDescriptorsForImagesInFile(ImageFile image_file, bool normalized=true);

        void calculateAndWriteDescriptorsForImagesInFile(const std::string &images_file, const std::string &outfile,
                                                         bool normalized=true,
                                                         const std::string &separator="\t", int number_images_per_line=1,
                                                         int total_number_channels=3);

        cv::Size getExpectedImageSize();

    private:
        caffe::Net<float> *net;
        std::string extractor_layer;
        std::string memory_data_layer_name;
        cv::Size expected_image_size;

        void init(ConfigFile config_file);
    };
}


#endif //DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP
