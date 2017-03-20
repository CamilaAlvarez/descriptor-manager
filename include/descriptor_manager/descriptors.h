//
// Created by Camila Alvarez on 18-03-17.
//

#ifndef DESCRIPTOR_MANAGER_DESCRIPTORS_HPP
#define DESCRIPTOR_MANAGER_DESCRIPTORS_HPP

#include <iostream>
#include <map>
#include "descriptor.h"

class Descriptors {

public:
    Descriptors(int number_items);
    void setDescriptorSize(int descriptor_size);
    void addDescriptor(const std::string &image_id, const Descriptor& descriptor);
    int getNumberOfItems();
    int getDescriptorSize();
    Descriptor getDescriptor(const std::string &image_id);
    void destroyDescriptors();
    void writeDescriptorsToFile(const std::string &outfile);
    void loadDescriptorsFromFile(const std::string &infile);

private:
    int descriptors_size;
    std::map<std::string, Descriptor> descriptors;
    int number_items;
};


#endif //DESCRIPTOR_MANAGER_DESCRIPTORS_HPP
