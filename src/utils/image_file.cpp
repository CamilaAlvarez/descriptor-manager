//
// Created by Camila Alvarez on 19-03-17.
//

#include "utils/image_file.h"

namespace descriptor {

    ImageFile::ImageFile(const std::string &images_file, const cv::Size &expected_size, int number_images_per_line,
                         int total_number_channels, const std::string &separator):
            expected_image_size(expected_size){
        std::ifstream image_file(images_file);
        std::string line;
        number_images = 0;
#if HAS_LOG
        LOG(INFO) << "LOADING IMAGES";
#endif
        while (std::getline(image_file, line)) {
            number_images++;
            std::vector<std::string> splitted_line;
            size_t separator_position = line.find(separator);
            std::string aux_line = line;
            while (separator_position != std::string::npos) {
                std::string line_part = aux_line.substr(0, separator_position);
                std::string line_rest = aux_line.substr(separator_position + 1);
                splitted_line.push_back(line_part);
                separator_position = line_rest.find(separator);
                aux_line = line_rest;
            }
#if HAS_LOG
            CHECK(number_images_per_line + 2 == splitted_line.size()) << "REAL NUMBER OF ARGUMENTS DOESN'T MATCH EXPECTED ONE";
#endif
            std::vector<std::string> images_list(splitted_line.begin() + 1,
                                                 splitted_line.begin() + number_images_per_line + 1);
            images.push_back(Image(splitted_line[0], total_number_channels, images_list, splitted_line.back(),
                                   expected_size));
#if HAS_LOG
            LOG(INFO) << "LOADED "+ std::to_string(number_images)+" IMAGES";
#endif
        }
#if HAS_LOG
        LOG(INFO) << "FINISHED LOADING IMAGES";
#endif
    }

    caffe::Datum ImageFile::getImageDatum(int index) {
        return images[index].getImageDatum();
    }

    cv::Mat ImageFile::getImageCVMat(int index) {
        return images[index].getImageCVMat();
    }

    int ImageFile::getNumberOfImages() {
        return number_images;
    }

    std::string ImageFile::getImageId(int index) {
        return images[index].getImageId();
    }

    std::string ImageFile::getImageClass(int index) {
        return images[index].getImageClass();
    }
}