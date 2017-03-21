//
// Created by Camila Alvarez on 18-03-17.
//

#include "descriptor_manager/descriptor.h"

namespace descriptor {

    Descriptor::Descriptor() {}

    Descriptor::Descriptor(const std::string &image_id, float *descriptor, int descriptor_size,
                           const std::string &image_class) :
            image_id(image_id), descriptor(descriptor), descriptor_size(descriptor_size), image_class(image_class) {}

    Descriptor::Descriptor(const Descriptor &descriptor_to_copy) {
        image_id = descriptor_to_copy.image_id;
        descriptor = descriptor_to_copy.descriptor;
        descriptor_size = descriptor_to_copy.descriptor_size;
        image_class = descriptor_to_copy.image_class;
    }

    std::string Descriptor::getImageId() {
        return image_id;
    }

    float *Descriptor::getDescriptor() {
        return descriptor;
    }

    int Descriptor::getDescriptorSize() {
        return descriptor_size;
    }

    std::string Descriptor::getImageClass() {
        return image_class;
    }

    void Descriptor::destroyDescriptor() {
        delete descriptor;
    }
}