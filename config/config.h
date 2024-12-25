#pragma once
#include "../platinumEyes.h"





const std::vector<std::string> ocrConfigNames = {
	"ocrType",
	"ocrIp",
	"ocrPort"
};

const std::vector<std::string> screenshotParametersNames = {
	"screenShotFilePath",
	"coordinatesOfScreenShotCenter",
	"screenShotWidth",
	"screenShotHeight"
};

const std::vector<std::string> windowSizesNames = {
	"sfmlSize",
	"imguiSize"
};



const std::vector<std::string> keyBindNames = {
	"keyBind_ReadItemsFromScreen",
	"keyBind_EscapeProgram",
	"keyBind_ReadPreviousItems",
	"keyBind_WindowVisibility",
	"keyBind_BackupConfig",
	"keyBind_ExampleItems",
	"keyBind_ReadRelicTitle"
};









bool contains(std::vector<std::string> targets, std::vector<std::string> differences);
bool keyBindsChanged(std::vector<std::string> differences);
bool windowSizesChanged(std::vector<std::string> differences);
void copyConfigToOldFile();
bool checkIfConfigFileExists();
void createConfigFile();
ToolConfig readConfigFile();
WindowParameters getWindowSize(std::string s, ToolConfig& toolconfig);
void rewriteConfigFile(ToolConfig& config);