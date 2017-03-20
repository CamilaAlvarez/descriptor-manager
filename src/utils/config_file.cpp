//
// Created by Camila Alvarez on 13-03-17.
//

#include "utils/config_file.h"
#include <fstream>
#include <assert.h>

ConfigFile::ConfigFile(const std::string &file, const std::string &separator) {
    loadConfigFileToMap(file, separator);
}

void ConfigFile::loadConfigFileToMap(const std::string &file, const std::string &separator) {
    std::ifstream config_file(file);
    //Check if file did open
    assert(config_file.is_open());
    std::string line;
    while(std::getline(config_file, line)){
        size_t separator_location = line.find(separator);
        assert(separator_location != std::string::npos);
        std::string config_key = line.substr(0, separator_location);
        std::string config_value = line.substr(separator_location+1);
        //Add log to tell user if key is repeated
        config_map[config_key] = config_value;
    }
}

std::string ConfigFile::getValueForKey(const std::string &key) {
    assert(config_map.find(key) != config_map.end());
    return config_map[key];
}

bool ConfigFile::hasKey(const std::string &key) {
    return config_map.find(key) != config_map.end();
}