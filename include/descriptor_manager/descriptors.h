//
// Created by Camila Alvarez on 18-03-17.
//

#ifndef DESCRIPTOR_MANAGER_DESCRIPTORS_HPP
#define DESCRIPTOR_MANAGER_DESCRIPTORS_HPP

#include <iostream>
#include <map>

class Descriptors {

public:
    void setDescriptorSize(int descriptor_size);
    void addDescriptor(const std::string &image_id, float *descriptor);

private:
    int descriptors_size;
    std::map<std::string, float*> descriptors;
};


#endif //DESCRIPTOR_MANAGER_DESCRIPTORS_HPP
