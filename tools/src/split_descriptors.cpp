//
// Created by calvarez on 29-03-17.
//

#include <iostream>
#include <fstream>
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "descriptor_manager/descriptors.h"

DEFINE_string(original_descriptors_file, "", "Descriptors file");
DEFINE_string(outputs, "", "Comma separated value which indicates output files");
DEFINE_string(images_ids, "", "Comma separated value with files containing images id to separate descriptors");

static void splitStringIntoVector(std::string &string_to_split, std::vector<std::string> &output_vector){
    size_t comma_position = string_to_split.find(",");
    std::string aux = string_to_split;
    while(comma_position != std::string::npos){
        std::string output = aux.substr(0, comma_position);
        if(output.size() != 0)
            output_vector.push_back(output);
        aux = aux.substr(comma_position+1);
        comma_position = aux.find(",");
    }
    if(aux.size()!=0)
        output_vector.push_back(aux);
}

int main(int argc, char *argv[]){
    FLAGS_logtostderr = true;
    gflags::SetUsageMessage("command line brew\n"
                                    "usage: split_descriptors\n"
                                    "--original_descriptors_file <descriptors_file>\n"
                                    "--outputs <output_file1,output_file2,...>\n"
                                    "--images_ids <images1,images2>");
    ::google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    if(FLAGS_original_descriptors_file.empty() || FLAGS_outputs.empty() || FLAGS_images_ids.empty() ){
        gflags::ShowUsageWithFlagsRestrict(argv[0],"split_descriptors");
        return 1;
    }

    std::vector<std::string> outputs;
    splitStringIntoVector(FLAGS_outputs, outputs);
    std::vector<std::string> images_files;
    splitStringIntoVector(FLAGS_images_ids, images_files);

    CHECK(outputs.size()==images_files.size()) << "Incompatible outputs vs images files";

    std::vector<std::vector<std::string> > selected_images;
    for(std::vector<std::string>::iterator it = images_files.begin(); it != images_files.end(); ++it){
        std::ifstream file(*it);
        CHECK(file.is_open()) << "Could not open file: "+*it;
        std::string line;
        std::vector<std::string> images;
        while(std::getline(file, line)){
            images.push_back(line);
        }
        selected_images.push_back(images);
    }

    CHECK(outputs.size()==selected_images.size()) << "Incompatible outputs vs images files";

    descriptor::Descriptors descriptors;
    descriptors.loadDescriptorsFromFile(FLAGS_original_descriptors_file);
    for(size_t i = 0; i < outputs.size(); i++){
        descriptors.writeDescriptorsToFile(outputs[i], selected_images[i]);
    }
    descriptors.destroyDescriptors();

    return 0;

}