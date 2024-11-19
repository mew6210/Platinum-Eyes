#include "../warframe_tool.h"


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


    HKL keyboard = LoadKeyboardLayoutA("00000415", KLF_ACTIVATE);        //polish programmer keyboard layout

    return VkKeyScanEx(s[0], keyboard);
}



static int counter = 1;
std::map<int, KeyBind> keyBindings = {
    {counter++,KeyBind(StringToVirtualKeyCode("b"),"Read items from current screen")},  
    {counter++,KeyBind(StringToVirtualKeyCode("esc"),"Escape program")},  
    {counter++,KeyBind(StringToVirtualKeyCode("x"),"Read previously shown items")},    
    {counter++,KeyBind(StringToVirtualKeyCode("c"),"Toggle window visibility")},    
    {counter++,KeyBind(StringToVirtualKeyCode("a"),"Save current config to copy file")}, 
    {counter++,KeyBind(StringToVirtualKeyCode("s"),"Show example items")}  
};



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




void registerHotkeys() {
    //every registered hotkey is assumed to be: alt+(hotkey)

    
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
            
        case 1: state.items = readItemsFromScreen(state.config);  
            break;
        case 2: state.running = false;  
            break;
        case 3: state.items = readItemsFromScreenWithoutScreenshot(state.config); 
            break;
        case 4: 
            /*
            if (state.isVisible == 0) {
                state.isVisible = 1;
                state.window.setVisible(state.isVisible);
            }
            else {
                state.isVisible = 0;
                state.window.setVisible(state.isVisible);
            }
            */
            state.isVisible = !state.isVisible;
            state.window.setVisible(state.isVisible);
            break;
        case 5: copyConfigToOldFile(); break;
        case 6: state.items = exampleItems; break;
        }
    


    }



}
