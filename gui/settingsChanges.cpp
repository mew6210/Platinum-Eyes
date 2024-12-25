#include "settingsChanged.h"
#include "../config/configNames.h"



bool contains(std::vector<std::string> targets, std::vector<std::string> differences) {

	for (auto& target : targets) {
		for (auto& difference : differences) {
			if (target == difference) return true;
		}
	}
	return false;



}


bool keyBindsChanged(std::vector<std::string> differences) {

	return contains(keyBindNames, differences);

}

bool windowSizesChanged(std::vector<std::string> differences) {

	return contains(windowSizesNames, differences);

}