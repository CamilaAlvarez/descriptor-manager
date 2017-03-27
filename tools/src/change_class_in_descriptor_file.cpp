//
// Created by calvarez on 27-03-17.
//
#include <iostream>
#include <fstream>
#include <map>
#include "glog/logging.h"
#include "gflags/gflags.h"
#include "descriptor_manager/descriptors.h"

DEFINE_string(descriptors_file, "", "File that contains descriptors");
DEFINE_string(output_descriptors_file, "", "Output file");
DEFINE_string(image_class_file, "", "File that contains image-class relationship");

int main(int argc, char* argv[]){
    FLAGS_logtostderr = true;
    gflags::SetUsageMessage("command line brew\n"
                                    "usage: change_class_in_descriptor_file\n"
                                    "--descriptors_file <descriptors_file>\n"
                                    "--output_descriptors_file <output_file>\n"
                                    "--image_class_file <image-class relation file>");
    ::google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    if(FLAGS_descriptors_file.empty() || FLAGS_output_descriptors_file.empty() || FLAGS_image_class_file.empty() ){
        gflags::ShowUsageWithFlagsRestrict(argv[0],"change_class_in_descriptor_file");
        return 1;
    }

    std::ifstream image_class_file(FLAGS_image_class_file);
    std::string line;
    std::map<std::string, std::string> image_and_class;

    CHECK(image_class_file.is_open()) << "Could not open image class file";

    while(std::getline(image_class_file, line)){
        size_t position = line.find("\t");
        std::string image = line.substr(0, position);
        std::string image_class =  line.substr(position+1);
        image_and_class[image] = image_class;
    }
    descriptor::Descriptors descriptors;
    descriptors.loadDescriptorsFromFile(FLAGS_descriptors_file);

    for(std::map<std::string, std::string>::iterator it = image_and_class.begin(); it != image_and_class.end(); ++it){
        descriptors.setClassForImage(it->first, it->second);
    }

    descriptors.writeDescriptorsToFile(FLAGS_output_descriptors_file);
    return 0;

}