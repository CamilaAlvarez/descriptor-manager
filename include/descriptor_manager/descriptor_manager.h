//
// Created by Camila Alvarez on 13-03-17.
//

#ifndef DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP
#define DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP

#include <iostream>
#include "caffe/caffe.hpp"
#include "descriptor_manager/config_file.h"
#include "descriptor_manager/descriptor.h"
#include "descriptor_manager/descriptors.h"
#include "opencv2/imgproc/imgproc.hpp"

class DescriptorManager {

public:
    explicit DescriptorManager(std::string network_config_file, std::string separator);
    explicit DescriptorManager(ConfigFile config_file);
    float * calculateDescriptorForImage(const cv::Mat& image);
    float * calculateDescriptorForDatum(const caffe::Datum &datum);
    Descriptor calculateDescriptorForDatum(const caffe::Datum &datum, const std::string &image_id);
    Descriptor calculateDescriptorForImage(const cv::Mat& image, const std::string &image_id);
    Descriptors calculateDescriptorsForImagesInFile(const std::string &images_file, const std::string &separator,
                                                    int number_images_per_line, int total_number_channels);
private:
    caffe::Net<float> net;
    ConfigFile config_file;


};


#endif //DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP
