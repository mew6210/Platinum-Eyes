#include "toolconfig.hpp"
#include "config.h"

std::string ToolConfig::getPropertyValue(std::string key) {
    if (properties.count(key) > 0) {
        std::string propertyValue = properties[key];
        trim(propertyValue);
        return propertyValue;
    }
    else {
        errorLog(false, "Not found property in config: ", key);
        return "not found";
    }
}

void ToolConfig::setPropertyValue(std::string key, std::string value) {
    if (properties.count(key) > 0) {
        trim(value);
        properties[key] = value;
    }
    else {
        errorLog(false, "Couldnt find ", key, " key in ToolConfig");
        return;
    }
}

ToolConfig::ToolConfig() {
    for (std::string property_key : CONFIGPROPERTIES) {
        properties.insert(std::pair<std::string, std::string>(property_key, "undefined"));
    }
}

std::string ToolConfig::operator[](std::string s) {
    return getPropertyValue(s);
}

bool ToolConfig::operator==(ToolConfig& otherConfig) {
    for (auto& property : properties) {
        if (otherConfig[property.first] != property.second) return false;
    }
    return true;
}

ToolConfig ToolConfig::operator=(ToolConfig& config) {
    properties = config.properties;
    return config;
}

std::vector<std::string> ToolConfig::getDifferenceList(ToolConfig& otherConfig) {
    std::vector<std::string> differenceList;
    for (auto& property : properties) {
        if (otherConfig[property.first] != property.second)
            differenceList.push_back(property.first);
    }
    return differenceList;
}

void ToolConfig::printConfig() {
    for (auto& property : properties) {
        std::cout << property.first + ": ";
        std::cout << property.second << std::endl;
    }
}