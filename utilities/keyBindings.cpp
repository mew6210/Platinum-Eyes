#include "../warframe_tool.h"


std::map<int, int> keyBindings = {
    {1,0x42},   //b
    {2,VK_ESCAPE},  //escape duh
    {3,0x5a}    //z
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
    for (std::pair<int, int> p : keyBindings) {
    

        if (RegisterHotKey(
            NULL,
            p.first,
            MOD_ALT | MOD_NOREPEAT,
            p.second))  //alt + b
        {
            std::cout << "Succesfully registered hotkey: "<<VirtualKeyCodeToString(p.second)<<std::endl;
        }
        else {
            std::cout << "Failed to register hotkey: "<<VirtualKeyCodeToString(p.second)<<std::endl;
        }
        

    }




}



void checkKeyPressed(MSG& msg,std::map<std::string,ProductPricing>& currentItems,ToolConfig& config,bool& running) {

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

}

