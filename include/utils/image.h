//
// Created by Camila Alvarez on 19-03-17.
//

#ifndef DESCRIPTOR_MANAGER_IMAGE_HPP
#define DESCRIPTOR_MANAGER_IMAGE_HPP

#include <iostream>
#include <vector>
#include "caffe/caffe.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Image {
public:
    Image(const std::string &image_id,int number_channels, const std::vector<std::string>& image_parts);
    caffe::Datum getImageDatum();
    cv::Mat getImageCVMat();
    std::string getImageId();

private:
    std::string image_id;
    int number_of_channels;
    std::vector<std::string> image_parts;
    std::vector<cv::Mat> cv_images;
    cv::Size max_size;
    void loadImages();
};


#endif //DESCRIPTOR_MANAGER_IMAGE_HPP