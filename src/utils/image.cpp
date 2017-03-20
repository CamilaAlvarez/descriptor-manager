//
// Created by Camila Alvarez on 19-03-17.
//

#include "utils/image.h"
#include "opencv2/highgui/highgui.hpp"
#include "caffe/util/io.hpp"
#include <cmath>

Image::Image(const std::string &image_id, int number_channels,const  std::vector<std::string> &image_parts):
image_id(image_id), number_of_channels(number_channels), image_parts(image_parts){ }

void Image::loadImages() {
    max_size = cv::Size(0,0);
    int current_number_channel = 0;
    for(std::vector<std::string>::iterator it = image_parts.begin(); it != image_parts.end(); ++it){
        cv::Mat image = cv::imread(*it);
        max_size.height = std::max(max_size.height, image.rows);
        max_size.width = std::max(max_size.width, image.cols);
        current_number_channel += image.channels();
        cv_images.push_back(image);
    }
    //assert(current_number_channel == number_channels)
}

caffe::Datum Image::getImageDatum() {
    if (cv_images.size() == 0)
        loadImages();

    caffe::Datum datum;
    datum.set_channels(number_of_channels);
    datum.set_height(max_size.height);
    datum.set_height(max_size.width);
    //llenar datum
    /*for(std::vector<cv::Mat>::iterator it = cv_images.begin(); it != cv_images.end(); ++it){
        caffe::Datum image_datum;
        cv::Mat image = *it;
        cv::resize(image, image, image.size(), 0, 0, cv::INTER_CUBIC);
        caffe::CVMatToDatum(image, &image_datum);
        image_datum.da
    }*/
    return datum;
}

cv::Mat Image::getImageCVMat(){
    if (cv_images.size() == 0)
        loadImages();
    //assert(number_of_channels <= 3)
}

std::string Image::getImageId() {
    return image_id;
}