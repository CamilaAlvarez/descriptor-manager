//
// Created by Camila Alvarez on 13-03-17.
//

#include "utils/config_file.h"
#include <fstream>
#ifdef HAS_GLOG
    #include "glog/logging.h"
#endif

ConfigFile::ConfigFile(const std::string &file, const std::string &separator) {
    loadConfigFileToMap(file, separator);
}

void ConfigFile::loadConfigFileToMap(const std::string &file, const std::string &separator) {
    std::ifstream config_file(file);
#if HAS_LOG
    CHECK(config_file.is_open()) << "CONFIG FILE COULD NOT BE OPENED";
#endif
    std::string line;
    while(std::getline(config_file, line)){
        size_t separator_location = line.find(separator);
#if HAS_LOG
        CHECK(separator_location != std::string::npos) << "LINE DOES NOT CONTAIN SEPARATOR";
#endif
        std::string config_key = line.substr(0, separator_location);
        std::string config_value = line.substr(separator_location+1);
#if HAS_LOG
        LOG_IF(WARNING, config_map.find(config_key) != config_map.end()) << "REPEATED KEY WILL REPLACE OLD VALUE";
#endif
        config_map[config_key] = config_value;
    }
}

std::string ConfigFile::getValueForKey(const std::string &key) {
#if HAS_LOG
    CHECK(config_map.find(key) != config_map.end()) << "INVALID KEY";
#endif
    return config_map[key];
}

bool ConfigFile::hasKey(const std::string &key) {
    return config_map.find(key) != config_map.end();
}
