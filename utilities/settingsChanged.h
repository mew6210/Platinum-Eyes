#pragma once

#include <vector>
#include <string>


bool contains(std::vector<std::string> targets, std::vector<std::string> differences);
bool keyBindsChanged(std::vector<std::string> differences);
bool windowSizesChanged(std::vector<std::string> differences);
