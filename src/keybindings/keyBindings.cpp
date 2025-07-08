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
    

    if (state.msg.message == WM_HOTKEY) {
        
        try {
            KeyBind keybind = keyBindings.at(state.msg.wParam);
            std::cout << "Alt + " << VirtualKeyCodeToString(keybind.getKey())<<"\n";
        }
        catch (std::out_of_range e) {
            std::cout << "didnt find this hotkey :3 \n";    //should never happen btw
        }
        
        //keybind logic
        switch (state.msg.wParam) {
            
        case KB_ReadItemsFromScreen:
        {
            state.items = readFissureRewardsScreen(state);
            state.itemDisplayFlag = ITEMTYPE_fissureItems;
        }
            break;
        case KB_EscapeProgram: state.running = false;
            break;
        case KB_ReadPreviousItems: { 
            state.items = readPreviousFissureRewardsScreen(state);
            state.itemDisplayFlag = ITEMTYPE_fissureItems;

            }
            break;
        case KB_WindowVisibility:
        {
            state.isVisible = !state.isVisible;
            state.window.setVisible(state.isVisible);

            if (state.isVisible) {
                state.window.setFramerateLimit(state.fpsVisible);
            }
            else {
                state.window.setFramerateLimit(state.fpsHidden);
            }

        }
            break;
        case KB_BackupConfig: copyConfigToOldFile(); break;
        case KB_ExampleItems: {
            if (state.itemDisplayFlag == ITEMTYPE_fissureItems) {
                state.items = exampleItems;
            }
            else if (state.itemDisplayFlag == ITEMTYPE_relicItems) {
                state.currentRelic= FetchRelicItemPrices("Lith A1 Relic (Intact)");
            }
           


        }
            break;
        case KB_ReadRelicTitle: {

            state.currentRelic=readItemsFromRelicTitleTesseract(state.tesseractApi);



            if (state.currentRelic.relicPrice == 0.0) {
                warningLog("No relic name found in its designated area, shifting to right.(Maybe user is in a mission)");
                state.currentRelic = readItemsFromRelicTitleTesseractShifted(state.tesseractApi);

            }
            printRelic(state.currentRelic);


            state.itemDisplayFlag = ITEMTYPE_relicItems;
            

            break;
        }
        }
    


    }



}
