//
// Created by Camila Alvarez on 13-03-17.
//

#ifndef DESCRIPTOR_MANAGER_CONFIGFILE_HPP
#define DESCRIPTOR_MANAGER_CONFIGFILE_HPP

#include <iostream>
#include <map>

class ConfigFile {
public:
    ConfigFile(const std::string &file, const std::string &separator = "\t");
    std::string getValueForKey(const std::string &key);
    bool hasKey(const std::string &key);
private:
    std::map<std::string, std::string> config_map;
    void loadConfigFileToMap(const std::string &file, const std::string &separator);

};


#endif //DESCRIPTOR_MANAGER_CONFIGFILE_HPP
