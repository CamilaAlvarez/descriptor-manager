//
// Created by Camila Alvarez on 13-03-17.
//

#ifndef DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP
#define DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP

#include <iostream>
#include "caffe/net.hpp"

class DescriptorManager {

public:
    explicit DescriptorManager(std::string network_config_file);
    explicit DescriptorManager()
private:
    caffe::Net net;

};


#endif //DESCRIPTOR_MANAGER_DESCRIPTORMANAGER_HPP
