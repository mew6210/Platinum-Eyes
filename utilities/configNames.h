#pragma once
#include <vector>
#include <string>


const std::string CONFIGPROPERTIES[] = {
	"ocrType",
	"ocrIp",
	"ocrPort",
	"screenShotFilePath",
	"coordinatesOfScreenShotCenter",
	"screenShotWidth",
	"screenShotHeight",
	"sfmlSize",
	"imguiSize",
	"keyBind_ReadItemsFromScreen",
	"keyBind_EscapeProgram",
	"keyBind_ReadPreviousItems",
	"keyBind_WindowVisibility",
	"keyBind_BackupConfig",
	"keyBind_ExampleItems",
	"keyBind_ReadRelicTitle"

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
const std::vector<std::string> windowSizesNames = {
	"sfmlSize",
	"imguiSize"
};
