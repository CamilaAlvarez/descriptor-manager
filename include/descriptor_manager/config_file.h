//
// Created by Camila Alvarez on 13-03-17.
//

#ifndef DESCRIPTOR_MANAGER_CONFIGFILE_HPP
#define DESCRIPTOR_MANAGER_CONFIGFILE_HPP

#include <iostream>
#include <map>

class ConfigFile {
public:
    ConfigFile(std::string file, std::string separator = "\t");

private:
    std::map<std::string, std::string> config_map;
};


#endif //DESCRIPTOR_MANAGER_CONFIGFILE_HPP
