#include "keybindingsnative.hpp"


#define KB_ReadItemsFromScreen 1
#define KB_EscapeProgram 2
#define KB_ReadPreviousItems 3
#define KB_WindowVisibility 4
#define KB_BackupConfig 5
#define KB_ExampleItems 6
#define KB_ReadRelicTitle 7

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)



void initializeKeyBindingsMap(std::map<int, KeyBind>& keyBindings, ToolConfig& config) {


    keyBindings = {
    { KB_ReadItemsFromScreen, KeyBind(StringToVirtualKeyCode(config["keyBind_ReadItemsFromScreen"]), "Read items from current screen") },
    { KB_EscapeProgram,KeyBind(StringToVirtualKeyCode(config["keyBind_EscapeProgram"]),"Escape program") },
    { KB_ReadPreviousItems,KeyBind(StringToVirtualKeyCode(config["keyBind_ReadPreviousItems"]),"Read previously shown items") },
    { KB_WindowVisibility,KeyBind(StringToVirtualKeyCode(config["keyBind_WindowVisibility"]),"Toggle window visibility") },
    { KB_BackupConfig,KeyBind(StringToVirtualKeyCode(config["keyBind_BackupConfig"]),"Save current config to copy file") },
    { KB_ExampleItems,KeyBind(StringToVirtualKeyCode(config["keyBind_ExampleItems"]),"Show example items") },
    { KB_ReadRelicTitle,KeyBind(StringToVirtualKeyCode(config["keyBind_ReadRelicTitle"]),"Relic Title Screenshot")}
    };


}

#endif


#if __linux__
void initializeKeyBindingsMap(std::map<int, KeyBind>& keyBindings, ToolConfig& config) {
    errorLog(true, "not implemented yet");
}

#endif