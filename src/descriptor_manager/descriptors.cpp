//
// Created by Camila Alvarez on 18-03-17.
//

#include "descriptor_manager/descriptors.h"
#include <fstream>

typedef std::map<std::string, Descriptor>::iterator map_iter;

Descriptors::Descriptors(int number_items):number_items(number_items) {}

void Descriptors::setDescriptorSize(int descriptor_size){
    this->descriptors_size = descriptor_size;
}

void Descriptors::addDescriptor(const std::string &image_id, const Descriptor& descriptor){
    //warning si se repite image_id
    descriptors[image_id] = descriptor;
}

int Descriptors::getNumberOfItems(){
    return number_items;
}

int Descriptors::getDescriptorSize() {
    return descriptors_size;
}

Descriptor Descriptors::getDescriptor(const std::string& image_id){
    return descriptors[image_id];
}

void Descriptors::destroyDescriptors() {
    for(map_iter it = descriptors.begin(); it != descriptors.end(); ++it ){
        Descriptor d = it->second;
        d.destroyDescriptor();
    }
}

void Descriptors::writeDescriptorsToFile(const std::string &outfile) {
    std::ofstream output;
    output.open(outfile, std::ios::out | std::ios::binary);

    //assert(output.is_open())
    output.write(reinterpret_cast<char *>(&number_items), sizeof(int));
    output.write(reinterpret_cast<char *>(&descriptors_size), sizeof(int));
    for(map_iter it = descriptors.begin(); it != descriptors.end(); ++it ){
        std::string image_id = it->first;
        Descriptor d = it->second;
        std::string image_class = d.getImageClass();

        int image_name_size = static_cast<int>(image_id.size());
        output.write(reinterpret_cast<char*>(&image_name_size), sizeof(image_name_size));
        int image_class_size = static_cast<int>(image_class.size());
        output.write(reinterpret_cast<char*>(&image_class_size), sizeof(image_class_size));
        output.write(image_id.c_str(), image_name_size);
        output.write(image_class.c_str(), image_class_size);
        float *descriptor_array = d.getDescriptor();
        output.write(reinterpret_cast<char *>(descriptor_array), sizeof(descriptor_array[0])*descriptors_size);
    }
}

void Descriptors::loadDescriptorsFromFile(const std::string &infile) {
    std::ifstream input_file;
    input_file.open(infile, std::ios::in | std::ios::binary);
    //assert(input_file.is_open())

    input_file.read(reinterpret_cast<char *>(&number_items), sizeof(number_items));
    input_file.read(reinterpret_cast<char *>(&descriptors_size), sizeof(descriptors_size));
    for(int i = 0; i < number_items; i++){
        int image_name_size;
        input_file.read(reinterpret_cast<char *>(&image_name_size), sizeof(image_name_size));
        int image_class_size;
        input_file.read(reinterpret_cast<char *>(&image_class_size), sizeof(image_class_size));
        char image_name_array[image_name_size];
        input_file.read(image_name_array, image_name_size);
        std::string image_id(image_name_array);
        char image_class_array[image_class_size];
        input_file.read(image_class_array, image_class_size);
        std::string image_class(image_class_array);
        float *descriptor = new float[descriptors_size];
        input_file.read(reinterpret_cast<char *>(descriptor), sizeof(descriptor[0])*descriptors_size);
        descriptors[image_id] = Descriptor(image_id, descriptor, descriptors_size, image_class);
    }
}

