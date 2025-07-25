#include "keybindings.h"

std::map<int, KeyBind> keyBindings = {};

#define KB_ReadItemsFromScreen 1
#define KB_EscapeProgram 2
#define KB_ReadPreviousItems 3
#define KB_WindowVisibility 4
#define KB_BackupConfig 5
#define KB_ExampleItems 6
#define KB_ReadRelicTitle 7
//keybindings are declared here
void initializeKeyBindingsMap(std::map<int, KeyBind>& keyBindings,ToolConfig& config) {


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


void validateProperty(std::string property,std::string name) {

    if (property.length() != 1 && property != "esc") {
        warningLog("Keybinds in config should have only 1 character in length (except esc). Program will proceed with its first character. Property: "+name+"\n");
    }

}


void validateConfigKeybinds(ToolConfig& config) {

    for (std::string keybind : keyBindNames) {
        validateProperty(config[keybind], keybind);     //validate every declared hotkey in config
    }

}

void reRegisterHotkeys(ToolConfig& config) {

    unregisterHotkeys();
    registerHotkeys(config);

}


void registerHotkeys(ToolConfig& config) {
    //every registered hotkey is assumed to be: alt+(hotkey)
    validateConfigKeybinds(config);
    initializeKeyBindingsMap(keyBindings,config);
    registerNativeHotkeys(keyBindings);
}

void unregisterHotkeys() {
    unregisterNativeHotkeys(keyBindings);
}




//TO NATIVEFY
void handleEvents(AppState state) {
    
    handleNativeEvents(state);
    
}
