//
// Created by Camila Alvarez on 18-03-17.
//

#ifndef DESCRIPTOR_MANAGER_DESCRIPTOR_HPP
#define DESCRIPTOR_MANAGER_DESCRIPTOR_HPP

#include <iostream>

class Descriptor {
public:
    Descriptor(const std::string& image_id, float* descriptor, int descriptor_size);
    std::string getImageId();
    float* getDescriptor();
    int getDescriptorSize();

private:
    std::string image_id;
    float * descriptor;
    int descriptor_size;

};


#endif //DESCRIPTOR_MANAGER_DESCRIPTOR_HPP
