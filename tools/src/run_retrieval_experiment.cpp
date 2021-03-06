//
// Created by calvarez on 21-03-17.
//

#include <queue>
#include <iostream>
#include <fstream>
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
DEFINE_string(queries, "", "Query id list");
DEFINE_string(retrieved_items, "", "Retrieved items id list");
DEFINE_string(output_dir, "", "Directory where experiment results will be saved");
DEFINE_string(network_config, "", "Network configuration file");


struct min_heap_comparator{
    bool operator()(const std::pair<std::string, float> &a,const std::pair<std::string, float> &b){
        return a.second > b.second;
    }
};

static bool checkValidFlags(){
    return FLAGS_images_file.size()!=0 && FLAGS_queries.size()!=0 && FLAGS_output_dir.size()!=0
           && FLAGS_network_config.size()!=0 && FLAGS_retrieved_items.size()!=0;
}

static void fillVectorWithFile(const std::string &file, std::vector<std::string> &vector_to_fill){
    std::ifstream input_file(file);
    std::string line;
    while(std::getline(input_file, line)){
        vector_to_fill.push_back(line);
    }
}

static void writeResult(const std::string &output_file, const std::string &image_name, const std::string &image_class,
                        std::priority_queue<std::pair<std::string, float>, std::vector<std::pair<std::string, float>>,
                                min_heap_comparator> results_heap, descriptor::Descriptors& descriptors){
    std::ofstream output(output_file);
    CHECK(output.is_open()) << "COULDN'T OPEN OUTPUT FILE: "+output_file;
    output << image_name << "\t" << image_class << std::endl;
    output << results_heap.size() << std::endl;
    int rank = 1;
    while(!results_heap.empty()){
        std::pair<std::string, float> retrieved_item = results_heap.top();
        results_heap.pop();
        std::string retrieved_item_id = retrieved_item.first;
        output << rank << "\t" << retrieved_item_id << "\t" <<
               descriptors.getDescriptor(retrieved_item_id).getImageClass()
               << "\t" << retrieved_item.second << std::endl;
        rank++;
    }
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

int main(int argc, char *argv[]){
    FLAGS_logtostderr = 1;
    gflags::SetUsageMessage("command line brew\n"
                                    "usage: run_retrieval_experiment --images_file <file>\n "
                                    "--images_per_line <number images>\n "
                                    "--channels_per_input <number of channels>\n "
                                    "--queries <queries file>\n "
                                    "--retrieved_items <retrieved items list file>\n "
                                    "--output_dir <output directory>\n "
                                    "--network_config <network config file>");
    ::google::InitGoogleLogging(argv[0]);
    ::gflags::ParseCommandLineFlags(&argc, &argv, true);
    if(!checkValidFlags()){
        gflags::ShowUsageWithFlagsRestrict(argv[0], "run_retrieval_experiment");
        return 1;
    }

    descriptor::ConfigFile config_file(FLAGS_network_config);
    descriptor::NetworkManager manager(config_file);
    descriptor::ImageFile image_file(FLAGS_images_file, manager.getExpectedImageSize(),
                                     FLAGS_images_per_line, FLAGS_channels_per_input);
    descriptor::Descriptors descriptors = manager.calculateDescriptorsForImagesInFile(image_file);

    boost::filesystem::path output_dir(FLAGS_output_dir);
    LOG_IF(WARNING, boost::filesystem::is_directory(output_dir)) << "OUTPUT DIRECTORY EXISTS. FILES MAY BE OVERWRITTEN";
    if(!boost::filesystem::is_directory(output_dir)){
        boost::filesystem::create_directory(output_dir);
    }

    std::vector<std::string> queries;
    fillVectorWithFile(FLAGS_queries, queries);
    std::vector<std::string> retrieved_items;
    fillVectorWithFile(FLAGS_retrieved_items, retrieved_items);
    size_t number_queries = queries.size();
#ifdef _OPENMP
    omp_lock_t lock;
    omp_init_lock(&lock);
#endif
#pragma omp parallel for num_threads(3)
    for(int i = 0; i < number_queries ; ++i){
        std::priority_queue<std::pair<std::string, float>, std::vector<std::pair<std::string, float>>,
                min_heap_comparator> min_heap;
        std::string query_name = queries[i];
#ifdef _OPENMP
        omp_set_lock(&lock);
#endif
        LOG(INFO) << "Experiment for: "+query_name;
#ifdef _OPENMP
        omp_unset_lock(&lock);
#endif
        descriptor::Descriptor query_descriptor_object = descriptors.getDescriptor(query_name);
        int descriptor_size = query_descriptor_object.getDescriptorSize();
        float *query_descriptor = query_descriptor_object.getDescriptor();
        for(std::vector<std::string>::iterator retrieved_it =  retrieved_items.begin();
            retrieved_it != retrieved_items.end(); ++retrieved_it){
            std::string retrieved_item = *retrieved_it;
            descriptor::Descriptor retrieved_item_descriptor_object = descriptors.getDescriptor(retrieved_item);
            float * retrieved_item_descriptor = retrieved_item_descriptor_object.getDescriptor();
            //Calculate distance
            float distance = calculateDistance(query_descriptor, retrieved_item_descriptor, descriptor_size);
            min_heap.push(std::make_pair(retrieved_item, distance));
        }
        std::string output_file_name = FLAGS_output_dir+"/"+query_name;
        writeResult(output_file_name, query_name, query_descriptor_object.getImageClass(), min_heap, descriptors);
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
    descriptors.destroyDescriptors();
    return 0;


}