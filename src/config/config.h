#pragma once
#include <fstream>
#include <SFML/Window.hpp>

#include "toolconfig.hpp"
#include "native/nativeconfig.hpp"
#include "../utilities/utilities.h"
#include "../utilities/trim/trim.hpp"

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

const std::vector<std::string> fontNames = {
	"fontFile",
	"fontSize"
};

const std::vector<std::string> dataNames = {
	"data_LastTimeLaunched",
	"data_LatestUpdate"
};

const std::vector<std::string> databaseNames = {

	"updatingType"


};

const std::vector<std::string> fpsNames = {

	"fpsVisible",
	"fpsHidden"

};

const std::vector<std::string> clipboardNames = {

	"clipboardCopy",
	"clipboardWatermark"

};

const std::vector<std::string> eeLogNames = {

	"eeLogShouldTakeScreenshot",
	"eeLogPath"

};

const std::vector<std::string> cacheNames = {

	"shouldCache",
	"cacheDuration"

};
/*
should be a sum of vectors above
its default property should also be coded in fillOutConfigFile
it should also probably have its own representation in settingsMenu
*/
const std::vector<std::string> CONFIGPROPERTIES = [] {
	std::vector<std::string> result;
	result.insert(result.end(), screenshotParametersNames.begin(), screenshotParametersNames.end());
	result.insert(result.end(), windowSizesNames.begin(), windowSizesNames.end());
	result.insert(result.end(), keyBindNames.begin(), keyBindNames.end());
	result.insert(result.end(), fontNames.begin(), fontNames.end());
	result.insert(result.end(), dataNames.begin(), dataNames.end());
	result.insert(result.end(), databaseNames.begin(), databaseNames.end());
	result.insert(result.end(), fpsNames.begin(), fpsNames.end());
	result.insert(result.end(), clipboardNames.begin(), clipboardNames.end());
	result.insert(result.end(), eeLogNames.begin(), eeLogNames.end());
	result.insert(result.end(), cacheNames.begin(), cacheNames.end());
	return result;
	}();

bool configStringToBool(const std::string& input);
bool contains(const std::vector<std::string>& targets, const std::vector<std::string>& differences);
bool keyBindsChanged(const std::vector<std::string>& differences);
bool windowSizesChanged(const std::vector<std::string>& differences);
bool fontsChanged(const std::vector<std::string>& differences);
bool fpsChanged(const std::vector<std::string>& differences);
bool eeLogWatcherChanged(const std::vector<std::string>& differences);


void copyConfigToOldFile();
bool checkIfConfigFileExists();
void createConfigFile();
ToolConfig readConfigFile();
WindowParameters getWindowSize(std::string s, ToolConfig& toolconfig);
void rewriteConfigFile(ToolConfig& config);

ToolConfig initConfig();
int getScreenResolution(int screenWidth, int screenHeight);


#define SCR_Size_NotSpecified 0
#define SCR_Size_1920x1080 1
#define SCR_Size_3440x1440 2
