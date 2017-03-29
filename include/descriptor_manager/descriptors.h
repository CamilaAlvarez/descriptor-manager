//
// Created by Camila Alvarez on 18-03-17.
//

#ifndef DESCRIPTOR_MANAGER_DESCRIPTORS_HPP
#define DESCRIPTOR_MANAGER_DESCRIPTORS_HPP

#include <iostream>
#include <map>
#include <vector>
#include "descriptor.h"

namespace descriptor {
    class Descriptors {

    public:

        Descriptors();

        Descriptors(int number_items);

        void setDescriptorSize(int descriptor_size);

        void setClassForImage(const std::string &image_id, const std::string &class_id);

        void addDescriptor(const std::string &image_id, const Descriptor &descriptor);

        int getNumberOfItems();

        int getDescriptorSize();

        Descriptor getDescriptor(const std::string &image_id);

        void destroyDescriptors();

        void writeDescriptorsToFile(const std::string &outfile,
                                    const std::vector<std::string> &selected_images = std::vector<std::string>());

        void loadDescriptorsFromFile(const std::string &infile);

        void includeDescriptors(const Descriptors &extra_descriptors);


    private:
        int descriptors_size;
        std::map<std::string, Descriptor> descriptors;
        int number_items;
    };
}

#endif //DESCRIPTOR_MANAGER_DESCRIPTORS_HPP
