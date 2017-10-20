//
// Created by calvarez on 20-10-17.
//

#include <queue>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#define STRIP_FLAG_HELP 1
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "boost/filesystem.hpp"
#include "descriptor_manager/network_manager.h"
#ifdef _OPENMP
#include <omp.h>
#endif

DEFINE_string(images_file, "", "Image list with their corresponding id and class");
DEFINE_int32(images_per_line, 1, "Number of images per line in images file (default = 1)");
DEFINE_int32(channels_per_input, 3, "Number of channels per network's image input (default = 3)");
DEFINE_string(output_dir, "", "Directory where experiment results will be saved");
DEFINE_string(network_config, "", "Network configuration file");
DEFINE_string(classes_file,"", "File that contains classes"); //map name\tid

static bool checkValidFlags(){
    return FLAGS_images_file.size()!=0 && FLAGS_output_dir.size()!=0
           && FLAGS_network_config.size()!=0 && FLAGS_classes_file.size()!=0;
}

static void fillVectorWithFile(const std::string &file, std::vector<std::string> &vector_to_fill){
    std::ifstream input_file(file);
    std::string line;
    while(std::getline(input_file, line)){
        vector_to_fill.push_back(line);
    }
}

static void writeResult(const std::string &output_file, std::vector<int>& correct_predictions_class,
                        std::map<int, std::string>& classes_map){
    std::ofstream output(output_file);
    CHECK(output.is_open()) << "COULDN'T OPEN OUTPUT FILE: "+output_file;
    int total_correct = 0;
    for (int i = 0; i < correct_predictions_class.size(); ++i) {
        std::string class_name = classes_map[i];
        int correct_predictions_per_class = correct_predictions_class[i];
        total_correct += correct_predictions_per_class;
        output << class_name << '\t' << correct_predictions_per_class << std::endl;
    }
    output << "total\t" << total_correct << std::endl;
    output.close();
}

static float calculateDistance(float *v, float *u, int size){
    float result = 0;
    int last_group = size-3;//to ensure the last group has four members
    float diff_0 = 0, diff_1 = 0, diff_2 = 0, diff_3 = 0;
    unsigned int i = 0;
    while (i < last_group) {
        diff_0 = v[i] - u[i];
        diff_1 = v[i + 1] - u[i + 1];
        diff_2 = v[i + 2] - u[i + 2];
        diff_3 = v[i + 3] - u[i + 3];
        result += diff_0 * diff_0 + diff_1 * diff_1 + diff_2 * diff_2 + diff_3 * diff_3;
        i += 4;
    }
    while (i < size) {
        diff_0 = v[i] - u[i];
        result += diff_0 * diff_0;
        i++;
    }
    return result;
}

static void loadClasses(std::map<int, std::string> &map_categories, std::string& classes_file){
    std::ifstream classes(classes_file);
    std::string class_name;
    int class_id;
    while (classes >> class_name >> class_id)
    {
        map_categories[class_id] = class_name;
    }
}

int main(int argc, char *argv[]){
    FLAGS_logtostderr = 1;
    gflags::SetUsageMessage("command line brew\n"
                                    "usage: run_classification_experiment --images_file <file>\n "
                                    "--images_per_line <number images>\n "
                                    "--channels_per_input <number of channels>\n "
                                    "--output_dir <output directory>\n "
                                    "--network_config <network config file>\n"
                                    "--classes_file <file with classes>");
    ::google::InitGoogleLogging(argv[0]);
    ::gflags::ParseCommandLineFlags(&argc, &argv, true);
    if(!checkValidFlags()){
        gflags::ShowUsageWithFlagsRestrict(argv[0], "run_classification_experiment");
        return 1;
    }
    std::map<int, std::string> classes_map;
    loadClasses(classes_map, FLAGS_classes_file);
    descriptor::ConfigFile config_file(FLAGS_network_config);
    descriptor::NetworkManager manager(config_file);
    descriptor::ImageFile image_file(FLAGS_images_file, manager.getExpectedImageSize(),
                                     FLAGS_images_per_line, FLAGS_channels_per_input);
    int numberImages = image_file.getNumberOfImages();
    descriptor::Descriptors descriptors = manager.calculateDescriptorsForImagesInFile(image_file);

    boost::filesystem::path output_dir(FLAGS_output_dir);
    LOG_IF(WARNING, boost::filesystem::is_directory(output_dir)) << "OUTPUT DIRECTORY EXISTS. FILES MAY BE OVERWRITTEN";
    if(!boost::filesystem::is_directory(output_dir)){
        boost::filesystem::create_directory(output_dir);
    }

    std::vector<int> correct_predictions_class;
    for(int i = 0; i < classes_map.size(); i++){
        correct_predictions_class.push_back(0);
    }

    std::vector<std::string> queries;
    for (int j = 0; j < numberImages; ++j) {
        queries.push_back(image_file.getImageId(j));
    }
#ifdef _OPENMP
    omp_lock_t lock;
    omp_init_lock(&lock);
#endif
#pragma omp parallel for num_threads(3)
    for(int i = 0; i < numberImages ; ++i){
        std::string query_name = queries[i];
#ifdef _OPENMP
        omp_set_lock(&lock);
#endif
        LOG(INFO) << "Experiment for: "+query_name;
#ifdef _OPENMP
        omp_unset_lock(&lock);
#endif
        descriptor::Descriptor query_descriptor_object = descriptors.getDescriptor(query_name);
        int query_class = image_file.getImageClass(i);
        int descriptor_size = query_descriptor_object.getDescriptorSize();
        float *query_descriptor = query_descriptor_object.getDescriptor();
        int max_index = -1;
        float current_prob = 0;
        for(int index = 0; index < descriptor_size ; index++){
            if(current_prob < query_descriptor[index]){
                current_prob = query_descriptor[index];
                max_index = index;
            }
        }
        if(max_index == query_class){
            correct_predictions_class[query_class]++;
        }
#ifdef _OPENMP
        omp_set_lock(&lock);
#endif
        LOG(INFO) << "Finished experiment for: "+query_name;
#ifdef _OPENMP
        omp_unset_lock(&lock);
#endif
    }
#ifdef _OPENMP
    omp_destroy_lock(&lock);
#endif
    std::string output_file_name = FLAGS_output_dir + "/classification.txt";
    writeResult(output_file_name, correct_predictions_class, classes_map);
    descriptors.destroyDescriptors();
    return 0;


}