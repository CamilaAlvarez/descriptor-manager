//
// Created by Camila Alvarez on 19-03-17.
//

#include "utils/image.h"
#include "opencv2/highgui/highgui.hpp"
#ifdef HAS_GLOG
    #include "glog/logging.h"
#endif

namespace descriptor {

    Image::Image(const std::string &image_id, int number_channels, const std::vector<std::string> &image_parts,
                 const std::string &image_class, const cv::Size &expected_size) :
            image_id(image_id), number_of_channels(number_channels), image_parts(image_parts),
            image_class(image_class), expected_size(expected_size) {}

    void Image::loadImages() {
        int current_number_channel = 0;
        for (std::vector<std::string>::iterator it = image_parts.begin(); it != image_parts.end(); ++it) {
            cv::Mat image = cv::imread(*it);
            current_number_channel += image.channels();
            cv::resize(image, image, expected_size, 0, 0, cv::INTER_CUBIC);
            cv_images.push_back(image);
        }
#ifdef HAS_LOG
        CHECK(current_number_channel == number_of_channels) << "REAL NUMBER OF CHANNELS DOESN'T MATCH EXPECTED NUMBER. "+
                std::to_string(current_number_channel)+"!="+std::to_string(number_of_channels);
#endif
    }

    caffe::Datum Image::getImageDatum() {
        if (cv_images.size() == 0)
            loadImages();

        caffe::Datum datum;
        int height = expected_size.height;
        int width = expected_size.width;
        datum.set_channels(number_of_channels);
        datum.set_height(height);
        datum.set_height(width);
        std::string buffer(static_cast<size_t>(number_of_channels * height * width), ' ');
        int previous_number_channels = 0;
        for (std::vector<cv::Mat>::iterator it = cv_images.begin(); it != cv_images.end(); ++it) {
            caffe::Datum image_datum;
            cv::Mat image = *it;
            int channel_number = it->channels();
            for (int h = 0; h < height; ++h) {
                const uchar *ptr = image.ptr<uchar>(h);
                int img_index = 0;
                for (int w = 0; w < width; ++w) {
                    for (int c = 0; c < channel_number; ++c) {
                        int datum_index = previous_number_channels * height * width + (c * height + h) * width + w;
                        buffer[datum_index] = static_cast<char>(ptr[img_index++]);
                    }
                }
            }
            previous_number_channels += channel_number;
        }
        datum.set_data(buffer);
        return datum;
    }

    cv::Mat Image::getImageCVMat() {
        if (cv_images.size() == 0)
            loadImages();
        if(cv_images.size() == 1){
#ifdef HAS_LOG
            CHECK(number_of_channels != cv_images[0].channels()) << "NUMBER OF CHANNELS DIFFERENT THAN EXPECTED. "+
                    std::to_string(number_of_channels)+"!="+std::to_string(cv_images[0].channels());
#endif
            return cv_images[0];
        }
#ifdef HAS_LOG
        CHECK(number_of_channels <= 3) << "TO MANY CHANNELS FOR OPENCV MATRIX";
#endif
        cv::Mat new_channels[number_of_channels];
        int new_channels_index = 0;
        for (std::vector<cv::Mat>::iterator it = cv_images.begin(); it != cv_images.end(); ++it) {
            int number_channels = it->channels();
            cv::Mat image_channels[number_channels];
            cv::split(*it, image_channels);
            for (int i = 0; i < number_channels; i++) {
                new_channels[new_channels_index] = image_channels[i];
                new_channels_index++;
            }
        }
        cv::Mat output;
        cv::merge(new_channels, static_cast<size_t>(number_of_channels), output);
        return output;
    }

    std::string Image::getImageId() {
        return image_id;
    }

    std::string Image::getImageClass() {
        return image_class;
    }
}