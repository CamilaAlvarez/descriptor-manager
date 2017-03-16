//
// Created by Camila Alvarez on 13-03-17.
//

#ifndef DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP
#define DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP

#include <iostream>
#include "caffe/net.hpp"
#include "descriptor_manager/config_file.h"

class DescriptorManager {

public:
    explicit DescriptorManager(std::string network_config_file, std::string separator);
    explicit DescriptorManager(ConfigFile config_file);
private:
    caffe::Net<float> net;
    ConfigFile config_file;

};


#endif //DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP
