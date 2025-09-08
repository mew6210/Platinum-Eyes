#pragma once
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "wood.h"

class ToolConfig {
    std::map<std::string, std::string> properties;

    std::string getPropertyValue(std::string key);

public:
    ToolConfig();

    void setPropertyValue(std::string key, std::string value);

    std::string operator[](std::string s);

    bool operator==(ToolConfig& otherConfig);

    ToolConfig operator=(ToolConfig& config);

    std::vector<std::string> getDifferenceList(ToolConfig& otherConfig);

    void printConfig();
};