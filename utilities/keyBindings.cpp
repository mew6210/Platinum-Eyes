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

std::map<int, KeyBind> keyBindings = {
    {1,KeyBind(0x42,"Take Screenshot")},   //b
    {2,KeyBind(VK_ESCAPE,"Escape program")},  //escape duh
    {3,KeyBind(0x58,"Read previously made screenshot")},    //x
    {4,KeyBind(0x43,"Toggle window visibility")}    //c
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



void checkKeyPressed(MSG& msg,std::map<std::string,ItemDetails>& currentItems,ToolConfig& config,bool& running,bool& visible, sf::RenderWindow& windowState) {




    if (msg.message == WM_HOTKEY && msg.wParam == 1) {
        std::cout << "Global hotkey detected.\n";
        currentItems = readItemsFromScreen(config);  // Close the SFML window when Escape is pressed globally
    }
    if (msg.message == WM_HOTKEY && msg.wParam == 2) {
        std::cout << "Global hotkey detected.\n";
        running = false;  // Close the SFML window when Escape is pressed globally
    }
    if (msg.message == WM_HOTKEY && msg.wParam == 3) {
        std::cout << "Global hotkey detected.\n";
        currentItems = readItemsFromScreenWithoutScreenshot(config);
    }
    if (msg.message == WM_HOTKEY && msg.wParam == 4) {
        if (visible == 0) {
            visible = 1;
            windowState.setVisible(visible);
            std::cout << "visible";
        }
        else {
            visible = 0;
            windowState.setVisible(visible);
            std::cout << "hidden";
        }
        
    }

}



void checkKeyPressedThroughState(AppState state) {




    if (state.msg.message == WM_HOTKEY && state.msg.wParam == 1) {
        std::cout << "Global hotkey detected.\n";
        state.items= readItemsFromScreen(state.config);  // Close the SFML window when Escape is pressed globally
    }
    if (state.msg.message == WM_HOTKEY && state.msg.wParam == 2) {
        std::cout << "Global hotkey detected.\n";
        state.running = false;  // Close the SFML window when Escape is pressed globally
    }
    if (state.msg.message == WM_HOTKEY && state.msg.wParam == 3) {
        std::cout << "Global hotkey detected.\n";
        state.items = readItemsFromScreenWithoutScreenshot(state.config);
    }
    if (state.msg.message == WM_HOTKEY && state.msg.wParam == 4) {
        if (state.isVisible == 0) {
            state.isVisible = 1;
            state.window.setVisible(state.isVisible);
            std::cout << "visible";
        }
        else {
            state.isVisible = 0;
            state.window.setVisible(state.isVisible);
            std::cout << "hidden";
        }

    }

}
