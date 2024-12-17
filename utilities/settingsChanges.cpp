#include "settingsChanged.h"

const std::vector<std::string> keyBindNames = {
	"keyBind_ReadItemsFromScreen",
	"keyBind_EscapeProgram",
	"keyBind_ReadPreviousItems",
	"keyBind_WindowVisibility",
	"keyBind_BackupConfig",
	"keyBind_ExampleItems"
};
const std::vector<std::string> windowSizesNames = {
"sfmlSize",
"imguiSize"
};



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