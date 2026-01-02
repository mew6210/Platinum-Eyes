#include "keybindings.h"

std::map<int, KeyBind> keyBindings = {};

constexpr int KB_ReadItemsFromScreen = 1;
constexpr int KB_EscapeProgram = 2;
constexpr int KB_ReadPreviousItems = 3;
constexpr int KB_WindowVisibility = 4;
constexpr int KB_BackupConfig = 5;
constexpr int KB_ExampleItems = 6;
constexpr int KB_ReadRelicTitle =  7;

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




void handleEvents(AppState& state) {
    
    handleNativeEvents(state,keyBindings);
    
}
