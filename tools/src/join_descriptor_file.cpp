//
// Created by calvarez on 29-03-17.
//

#include <iostream>
#include <fstream>
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "descriptor_manager/descriptors.h"

DEFINE_string(descriptors_files, "", "Comma separated value which indicates input descriptors files");
DEFINE_string(output_file, "", "Name of the final descriptor file");

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

int main(int argc, char *argv[]) {
    FLAGS_logtostderr = true;
    gflags::SetUsageMessage("command line brew\n"
                                    "usage: join_descriptors\n"
                                    "--descriptors_files <descriptors_file1, descriptors_file2,...>\n"
                                    "--output_file <output_file>\n");
    ::google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    if (FLAGS_descriptors_files.empty() || FLAGS_output_file.empty()) {
        gflags::ShowUsageWithFlagsRestrict(argv[0], "join_descriptors");
        return 1;
    }
    std::vector<std::string> input_files;
    splitStringIntoVector(FLAGS_descriptors_files, input_files);

    CHECK(input_files.size() <= 1) << "No files to read!!";
    descriptor::Descriptors main_descriptors;
    main_descriptors.loadDescriptorsFromFile(input_files[0]);

    for (size_t i = 1; i < input_files.size() ; ++i) {
        descriptor::Descriptors descriptors;
        descriptors.loadDescriptorsFromFile(input_files[i]);
        main_descriptors.includeDescriptors(descriptors);
    }

    main_descriptors.writeDescriptorsToFile(FLAGS_output_file);
    return 0;

}