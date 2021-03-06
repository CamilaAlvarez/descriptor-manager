//
// Created by Camila Alvarez on 18-03-17.
//

#include "descriptor_manager/descriptors.h"
#include <fstream>
#include <algorithm>
#ifdef HAS_GLOG
    #include "glog/logging.h"
#endif

namespace descriptor {

    typedef std::map<std::string, Descriptor>::iterator map_iter;

    Descriptors::Descriptors() : number_items(0) {}

    Descriptors::Descriptors(int number_items) : number_items(number_items) {}

    void Descriptors::setDescriptorSize(int descriptor_size) {
        this->descriptors_size = descriptor_size;
    }

    void Descriptors::addDescriptor(const std::string &image_id, const Descriptor &descriptor) {
#ifdef HAS_LOG
        LOG_IF(WARNING, descriptors.find(image_id) != descriptors.end()) << "THERE IS A  SAVED DESCRIPTOR WITH THE SAME IMAGE ID";
#endif
        descriptors[image_id] = descriptor;
    }

    int Descriptors::getNumberOfItems() {
        if(number_items != descriptors.size()){
            number_items = static_cast<int>(descriptors.size());
        }
        return number_items;
    }

    int Descriptors::getDescriptorSize() {
        return descriptors_size;
    }

    Descriptor Descriptors::getDescriptor(const std::string &image_id) {
#if HAS_LOG
        CHECK(descriptors.find(image_id) != descriptors.end()) << "THERE ARE NOT DESCRIPTORS FOR THAT IMAGE ID: "+image_id;
#endif
        return descriptors[image_id];
    }

    void Descriptors::destroyDescriptors() {
        for (map_iter it = descriptors.begin(); it != descriptors.end(); ++it) {
            Descriptor d = it->second;
            d.destroyDescriptor();
        }
    }

    void Descriptors::writeDescriptorsToFile(const std::string &outfile,
                                             const std::vector<std::string> &selected_images) {
        std::ofstream output;
        output.open(outfile, std::ios::out | std::ios::binary);

#ifdef HAS_LOG
        CHECK(output.is_open()) << "COULD NOT OPEN OUTPUT FILE";
        LOG(INFO) << "WRITING DESCRIPTORS IN FILE";
#endif
        if (selected_images.empty()) {
            if(number_items != descriptors.size()){
                number_items = static_cast<int>(descriptors.size());
            }
            output.write(reinterpret_cast<char *>(&number_items), sizeof(int));
        }
        else {
            int items_number = static_cast<int >(selected_images.size());
            output.write(reinterpret_cast<char *>(&items_number), sizeof(int));
        }
        output.write(reinterpret_cast<char *>(&descriptors_size), sizeof(int));
        for (map_iter it = descriptors.begin(); it != descriptors.end(); ++it) {
            if(!selected_images.empty() &&
                    std::find(selected_images.begin(), selected_images.end(), it->first) == selected_images.end()) {
                continue;
            }
            std::string image_id = it->first;
            Descriptor d = it->second;
            std::string image_class = d.getImageClass();

            int image_name_size = static_cast<int>(image_id.size());
            output.write(reinterpret_cast<char *>(&image_name_size), sizeof(image_name_size));
            int image_class_size = static_cast<int>(image_class.size());
            output.write(reinterpret_cast<char *>(&image_class_size), sizeof(image_class_size));
            output.write(image_id.c_str(), image_name_size);
            output.write(image_class.c_str(), image_class_size);
            float *descriptor_array = d.getDescriptor();
            output.write(reinterpret_cast<char *>(descriptor_array), sizeof(descriptor_array[0]) * descriptors_size);
        }
#ifdef HAS_LOG
        LOG(INFO) << "FINISHED WRITING DESCRIPTORS IN FILE";
#endif
        output.close();
    }



    void Descriptors::loadDescriptorsFromFile(const std::string &infile) {
        std::ifstream input_file;
        input_file.open(infile, std::ios::in | std::ios::binary);
#ifdef HAS_LOG
        LOG_IF(WARNING, descriptors.size() > 0) << "THIS WILL OVERWRITE CURRENT DESCRIPTORS";
        CHECK(input_file.is_open()) << "COULD NOT OPEN OUTPUT FILE";
        LOG(INFO) << "LOADING DESCRIPTORS";
#endif

        if (descriptors.size() > 0)
            destroyDescriptors();

        input_file.read(reinterpret_cast<char *>(&number_items), sizeof(number_items));
        input_file.read(reinterpret_cast<char *>(&descriptors_size), sizeof(descriptors_size));
        for (int i = 0; i < number_items; i++) {
            int image_name_size;
            input_file.read(reinterpret_cast<char *>(&image_name_size), sizeof(image_name_size));
            int image_class_size;
            input_file.read(reinterpret_cast<char *>(&image_class_size), sizeof(image_class_size));
            char image_name_array[image_name_size+1];
            input_file.read(image_name_array, image_name_size);
            image_name_array[image_name_size] = '\0';
            std::string image_id(image_name_array);
            char image_class_array[image_class_size+1];
            input_file.read(image_class_array, image_class_size);
            image_class_array[image_class_size] = '\0';
            std::string image_class(image_class_array);
            float *descriptor = new float[descriptors_size];
            input_file.read(reinterpret_cast<char *>(descriptor), sizeof(descriptor[0]) * descriptors_size);
            descriptors[image_id] = Descriptor(image_id, descriptor, descriptors_size, image_class);
        }
#ifdef HAS_LOG
        LOG(INFO) << "FINISHED LOADING DESCRIPTORS";
#endif
        input_file.close();
    }

    void Descriptors::setClassForImage(const std::string &image_id, const std::string &class_id) {
#ifdef HAS_LOG
        CHECK(descriptors.find(image_id)!=descriptors.end()) << "IMAGE COULD NOT BE FOUND: "+image_id;
#endif
        descriptors[image_id] = Descriptor(image_id, descriptors[image_id].getDescriptor(), descriptors_size,
                                           class_id);
    }

    void Descriptors::includeDescriptors(const Descriptors &extra_descriptors) {
        LOG_IF( WARNING, extra_descriptors.descriptors.size()!=0) << "NO DESCRIPTORS WILL BE ADDED";
        for(std::map<std::string, Descriptor>::const_iterator it = extra_descriptors.descriptors.begin();
            it != extra_descriptors.descriptors.end(); ++it){
            addDescriptor(it->first, it->second);
        }
    }

}

