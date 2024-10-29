// warframe_tool.cpp: definiuje punkt wejścia dla aplikacji.
//

#include "warframe_tool.h"




using json = nlohmann::json;
int main()
{


    if (RegisterHotKey(
        NULL,
        1,
        MOD_ALT | MOD_NOREPEAT,
        0x42))  //0x42 is 'b'
    {
        std::cout << "succesfully registered a hotkey";
    }



    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            std::cout << "hotkey pressed";
            readItemsFromScreen();
        }


    }




    return 0;
}



