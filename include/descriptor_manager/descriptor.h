//
// Created by Camila Alvarez on 18-03-17.
//

#ifndef DESCRIPTOR_MANAGER_DESCRIPTOR_HPP
#define DESCRIPTOR_MANAGER_DESCRIPTOR_HPP

#include <iostream>

namespace descriptor {
    class Descriptor {
    public:
        Descriptor();

        explicit Descriptor(const std::string &image_id, float *descriptor, int descriptor_size,
                            const std::string &image_class);

        Descriptor(const Descriptor &descriptor_to_copy);

        std::string getImageId();

        float *getDescriptor();

        int getDescriptorSize();

        std::string getImageClass();

        void destroyDescriptor();

    private:
        std::string image_id;
        float *descriptor;
        int descriptor_size;
        std::string image_class;

    };
}

#endif //DESCRIPTOR_MANAGER_DESCRIPTOR_HPP
