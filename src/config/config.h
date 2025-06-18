#pragma once
#include "../core.h"



//should be included in configproperties[] in core.h
const std::vector<std::string> screenshotParametersNames = {
	"screenShotFilePath",
	"coordinatesOfScreenShotCenter",
	"screenShotWidth",
	"screenShotHeight"
};

//should be included in configproperties[] in core.h
const std::vector<std::string> windowSizesNames = {
	"sfmlSize",
	"imguiSize"
};


//should be included in configproperties[] in core.h
const std::vector<std::string> keyBindNames = {
	"keyBind_ReadItemsFromScreen",
	"keyBind_EscapeProgram",
	"keyBind_ReadPreviousItems",
	"keyBind_WindowVisibility",
	"keyBind_BackupConfig",
	"keyBind_ExampleItems",
	"keyBind_ReadRelicTitle"
};

//should be included in configproperties[] in core.h
const std::vector<std::string> fontNames = {
	"fontFile",
	"fontSize"
};

//should be included in configproperties[] in core.h
const std::vector<std::string> dataNames = {
	"data_LastTimeLaunched",
	"data_LatestUpdate"
};

//should be included in configproperties[] in core.h
const std::vector<std::string> databaseNames = {

	"updatingType"


};

const std::vector<std::string> fpsNames = {

	"fpsVisible",
	"fpsHidden"

};


bool contains(std::vector<std::string> targets, std::vector<std::string> differences);
bool keyBindsChanged(std::vector<std::string> differences);
bool windowSizesChanged(std::vector<std::string> differences);
bool fontsChanged(std::vector<std::string> differences);
bool fpsChanged(std::vector<std::string> differences);


void copyConfigToOldFile();
bool checkIfConfigFileExists();
void createConfigFile();
ToolConfig readConfigFile();
WindowParameters getWindowSize(std::string s, ToolConfig& toolconfig);
void rewriteConfigFile(ToolConfig& config);

ToolConfig initConfig();