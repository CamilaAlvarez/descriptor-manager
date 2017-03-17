//
// Created by Camila Alvarez on 13-03-17.
//

#include "descriptor_manager/descriptor_manager.h"


float * DescriptorManager::calculateDescriptorForImage(const cv::Mat& image) {
    caffe::Datum datum;
    caffe::CVMatToDatum(image, &datum);
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

//agregar assert
    const boost::shared_ptr<caffe::Blob<float>> &descriptor_blob = net.blob_by_name(config_file.getValueForKey("LAYER"));
    const float * descriptor_data  = descriptor_blob->cpu_data();
    int dimension = descriptor_blob->count();
    float* descriptor = new float[dimension];
    std::copy(descriptor_data, descriptor_data+dimension, descriptor);
    return descriptor;

}