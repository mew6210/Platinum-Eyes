#include "keybindings.h"


class KeyBind {

    int key;
    std::string description;

public: 
    KeyBind(int k, std::string s) {
        key = k;
        description = s;
    }

    int getKey() {
        return key;
    }
    std::string getDescription() {
        return description;
    }


};


int StringToVirtualKeyCode(std::string s) {

    for (auto& c : s) {
        c = std::tolower(c);
    }


    if (s == "esc") {
        return VK_ESCAPE;
    }



    HKL keyboard = LoadKeyboardLayoutA("00000409", KLF_ACTIVATE);        //primary english US keyboard layout

    return VkKeyScanEx(s[0], keyboard);
}


#define KB_ReadItemsFromScreen 1
#define KB_EscapeProgram 2
#define KB_ReadPreviousItems 3
#define KB_WindowVisibility 4
#define KB_BackupConfig 5
#define KB_ExampleItems 6
#define KB_RelicTitleScreenshot 7



std::map<int, KeyBind> keyBindings = {};



std::string VirtualKeyCodeToString(UCHAR virtualKey)
{
    UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

    CHAR szName[128];
    int result = 0;
    switch (virtualKey)
    {
    case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
    case VK_RCONTROL: case VK_RMENU:
    case VK_LWIN: case VK_RWIN: case VK_APPS:
    case VK_PRIOR: case VK_NEXT:
    case VK_END: case VK_HOME:
    case VK_INSERT: case VK_DELETE:
    case VK_DIVIDE:
    case VK_NUMLOCK:
        scanCode |= KF_EXTENDED;
    default:
        result = GetKeyNameTextA(scanCode << 16, szName, 128);
    }
    if (result == 0)
        throw std::system_error(std::error_code(GetLastError(), std::system_category()),
            "WinAPI Error occured.");
    return szName;
}


void initializeKeyBindingsMap(std::map<int, KeyBind>& keyBindings,ToolConfig& config) {


    keyBindings = { 
    { KB_ReadItemsFromScreen, KeyBind(StringToVirtualKeyCode(config["keyBind_ReadItemsFromScreen"]), "Read items from current screen") },
    { KB_EscapeProgram,KeyBind(StringToVirtualKeyCode(config["keyBind_EscapeProgram"]),"Escape program") },
    { KB_ReadPreviousItems,KeyBind(StringToVirtualKeyCode(config["keyBind_ReadPreviousItems"]),"Read previously shown items") },
    { KB_WindowVisibility,KeyBind(StringToVirtualKeyCode(config["keyBind_WindowVisibility"]),"Toggle window visibility") },
    { KB_BackupConfig,KeyBind(StringToVirtualKeyCode(config["keyBind_BackupConfig"]),"Save current config to copy file") },
    { KB_ExampleItems,KeyBind(StringToVirtualKeyCode(config["keyBind_ExampleItems"]),"Show example items") },
    { KB_RelicTitleScreenshot,KeyBind(StringToVirtualKeyCode(config["keyBind_ReadRelicTitle"]),"Relic Title Screenshot")}
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

    //loop through every declared hotkey
    for (auto& p : keyBindings) {
    

        if (RegisterHotKey(
            NULL,
            p.first,
            MOD_ALT | MOD_NOREPEAT,
            p.second.getKey()))  
        {
            std::cout << "Succesfully registered hotkey: "<<VirtualKeyCodeToString(p.second.getKey())<<" for: "<<p.second.getDescription()<< std::endl;
        }
        else {
            std::cout << "Failed to register hotkey: "<<VirtualKeyCodeToString(p.second.getKey())<<" for: "<<p.second.getDescription()<< std::endl;
        }
        

    }




}

void unregisterHotkeys() {

    for (auto& p : keyBindings) {
        if (UnregisterHotKey(NULL, p.first)) {
            std::cout << "Succesfully unregistered Alt + " << VirtualKeyCodeToString(p.second.getKey())<<"\n";
        }
        else {
            std::cout << "Failed to unregister Alt + " << VirtualKeyCodeToString(p.second.getKey()) << "\n";
        }
    }

}





void checkKeyPressed(AppState state) {
    

    if (state.msg.message == WM_HOTKEY) {
        
        try {
            KeyBind keybind = keyBindings.at(state.msg.wParam);
            std::cout << "Alt + " << VirtualKeyCodeToString(keybind.getKey())<<"\n";
        }
        catch (std::out_of_range e) {
            std::cout << "didnt find this hotkey :3 \n";
        }
        
        //keybind logic
        switch (state.msg.wParam) {
            
        case KB_ReadItemsFromScreen:
        {
            state.items = readItemsFromScreen(state);
            state.itemDisplayFlag = ITEMTYPE_fissureItems;
        }
            break;
        case KB_EscapeProgram: state.running = false;
            break;
        case KB_ReadPreviousItems: { 
            state.items = readItemsFromScreenWithoutScreenShot(state);             
            state.itemDisplayFlag = ITEMTYPE_fissureItems;

            }
            break;
        case KB_WindowVisibility:
        {
            state.isVisible = !state.isVisible;
            state.window.setVisible(state.isVisible);

            if (state.isVisible) {
                state.window.setFramerateLimit(60);
            }
            else {
                state.window.setFramerateLimit(5);
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
        case KB_RelicTitleScreenshot: {

            state.currentRelic=readItemsFromRelicTitleTesseract(state.tesseractApi);



            if (state.currentRelic.relicPrice == 0.0) {
                state.currentRelic = readItemsFromRelicTitleTesseractShifted(state.tesseractApi);

            }
            printRelic(state.currentRelic);


            state.itemDisplayFlag = ITEMTYPE_relicItems;
            

            break;
        }
        }
    


    }



}
