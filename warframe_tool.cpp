// warframe_tool.cpp: definiuje punkt wejścia dla aplikacji.
//

#include "warframe_tool.h"




using json = nlohmann::json;
int main()
{
    

    if (!checkIfConfigFileExists()) {
        createConfigFile();
        std::cout << "Config file successfully created. For program to work, configure the config file named tool_config.txt \nFor tips check github repo.";
        return 0;
    }

    ToolConfig toolConfig = readConfigFile();




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
            std::cout << "hotkey detected"<<std::endl;
            readItemsFromScreen(toolConfig);
        }


    }




    return 0;
}



