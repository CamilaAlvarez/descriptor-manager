//
// Created by Camila Alvarez on 18-03-17.
//

#include "descriptor_manager/descriptors.h"

void Descriptors::setDescriptorSize(int descriptor_size){
    this->descriptors_size = descriptor_size;
}

void Descriptors::addDescriptor(const std::string &image_id, float *descriptor){
    descriptors[image_id] = descriptor;
}