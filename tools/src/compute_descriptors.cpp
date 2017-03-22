//
// Created by calvarez on 22-03-17.
//


#include <iostream>
#define STRIP_FLAG_HELP 1
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "descriptor_manager/network_manager.h"


DEFINE_string(images_file, "", "Image list with their corresponding id and class");
DEFINE_int32(images_per_line, 1, "Number of images per line in images file (default = 1)");
DEFINE_int32(channels_per_input, 3, "Number of channels per network's image input (default = 3)");
DEFINE_string(output_file, "", "Directory where experiment results will be saved");
DEFINE_string(network_config, "", "Network configuration file");


static bool checkValidFlags(){
    return FLAGS_images_file.size()!=0 && FLAGS_output_file.size()!=0 && FLAGS_network_config.size()!=0;
}


int main(int argc, char *argv[]){
    FLAGS_logtostderr = 1;
    gflags::SetUsageMessage("command line brew\n"
                                    "usage: compute_descriptors --images_file <file>\n "
                                    "--images_per_line <number images>\n "
                                    "--channels_per_input <number of channels>\n "
                                    "--output_file <output file>\n "
                                    "--network_config <network config file>");
    ::google::InitGoogleLogging(argv[0]);
    ::gflags::ParseCommandLineFlags(&argc, &argv, true);
    if(!checkValidFlags()){
        gflags::ShowUsageWithFlagsRestrict(argv[0], "compute_descriptors");
        return 1;
    }

    descriptor::ConfigFile config_file(FLAGS_network_config);
    descriptor::NetworkManager manager(config_file);
    descriptor::ImageFile image_file(FLAGS_images_file, manager.getExpectedImageSize(),
                                     FLAGS_images_per_line, FLAGS_channels_per_input);
    manager.calculateAndWriteDescriptorsForImagesInFile(image_file,
                                                        FLAGS_output_file);


    return 0;


}