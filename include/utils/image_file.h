//
// Created by Camila Alvarez on 19-03-17.
//

#ifndef DESCRIPTOR_MANAGER_IMAGE_FILE_HPP
#define DESCRIPTOR_MANAGER_IMAGE_FILE_HPP

#include <iostream>
#include "image.h"
#include "caffe/caffe.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class ImageFile {
public:
    ImageFile(const std::string &images_file,
              const std::string &separator,
              int number_images_per_line = 1,
              int total_number_channels = 3);

    int getNumberOfImages();
    caffe::Datum getImageDatum(int index);
    cv::Mat getImageCVMat(int index);
    std::string getImageId(int index);

private:
    int number_images;
    std::vector<Image> images;

};


#endif //DESCRIPTOR_MANAGER_IMAGE_FILE_HPP
