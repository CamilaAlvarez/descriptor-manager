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

class DescriptorManager {

public:
    explicit DescriptorManager(std::string network_config_file, std::string separator);
    explicit DescriptorManager(ConfigFile config_file);
    float * calculateDescriptorForImage(const cv::Mat& image);
    float * calculateDescriptorForDatum(const caffe::Datum &datum);
    Descriptor calculateDescriptorForDatum(const caffe::Datum &datum, const std::string &image_id,
                                           const std::string& image_class = "0");
    Descriptor calculateDescriptorForImage(const cv::Mat& image, const std::string &image_id,
                                           const std::string& image_class = "0");
    Descriptors calculateDescriptorsForImagesInFile(const std::string &images_file, const std::string &separator,
                                                    int number_images_per_line = 1, int total_number_channels = 3);
    Descriptors calculateDescriptorsForImagesInFile(ImageFile image_file);
    void calculateAndWriteDescriptorsForImagesInFile(const std::string &images_file,
                                                     const std::string &separator, int number_images_per_line,
                                                     int total_number_channels, const std::string &outfile);
private:
    caffe::Net<float> net;
    ConfigFile config_file;


};


#endif //DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP
