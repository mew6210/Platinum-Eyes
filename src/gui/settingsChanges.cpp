#include "../config/config.h"

bool contains(const std::vector<std::string>& targets, const std::vector<std::string>& differences) {

	for (auto& target : targets) {
		for (auto& difference : differences) {
			if (target == difference) return true;
		}
	}
	return false;



}


bool keyBindsChanged(const std::vector<std::string>& differences) {

	return contains(keyBindNames, differences);

}

bool windowSizesChanged(const std::vector<std::string>& differences) {

	return contains(windowSizesNames, differences);

}

bool fontsChanged(const std::vector<std::string>& differences) {
	return contains(fontNames, differences);
}

bool fpsChanged(const std::vector<std::string>& differences) {

	return contains(fpsNames,differences);

}

bool eeLogWatcherChanged(const std::vector<std::string>& differences) {

	return contains(eeLogNames, differences);

}