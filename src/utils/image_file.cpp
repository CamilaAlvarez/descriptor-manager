//
// Created by Camila Alvarez on 19-03-17.
//

#include "utils/image_file.h"

namespace descriptor {

    ImageFile::ImageFile(const std::string &images_file, int number_images_per_line,
                         int total_number_channels, const std::string &separator) {
        std::ifstream image_file(images_file);
        std::string line;
        number_images = 0;
        while (std::getline(image_file, line)) {
            number_images++;
            std::vector<std::string> splitted_line;
            size_t separator_position = line.find(separator);
            while (separator_position != std::string::npos) {
                std::string line_part = line.substr(0, separator_position);
                std::string line_rest = line.substr(separator_position + 1);
                splitted_line.push_back(line_part);
                separator_position = line_rest.find(separator);
            }
#if HAS_LOG
            CHECK(number_images_per_line + 2 == splitted_line.size()) << "REAL NUMBER OF ARGUMENTS DOESN'T MATCH EXPECTED ONE";
#endif
            std::vector<std::string> images_list(splitted_line.begin() + 1,
                                                 splitted_line.begin() + number_images_per_line + 1);
            images.push_back(Image(splitted_line[0], total_number_channels, images_list, splitted_line.back()));
        }
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