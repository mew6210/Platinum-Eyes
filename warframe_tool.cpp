#include "warframe_tool.h"


using json = nlohmann::json;
int main()
{

    std::map<std::string, ItemDetails> currentItems;

    if (!checkIfConfigFileExists()) {
        createConfigFile();
        std::cout << "Config file successfully created. For program to work, configure the config file named tool_config.txt \nFor tips check github repo.";
        getchar();
        return 0;
    }

    ToolConfig toolConfig = readConfigFile();

    registerHotkeys();
    
    WindowParameters sfmlSize = getWindowSize("sfml",toolConfig);
    WindowParameters imguiSize = getWindowSize("imgui",toolConfig);

    sf::RenderWindow window(sf::VideoMode(sfmlSize.width, sfmlSize.height), "Warframe tool", sf::Style::None, sf::ContextSettings(32));
    
    sf::Clock deltaClock;
    
    bool running = true;
    bool visible = true;
    MSG msg = { 0 };

    AppState state(currentItems,toolConfig,window,running,visible,msg,sfmlSize,imguiSize);
    


    customizeWindow(window,state.sfmlSize);



    while (running)
    {
        //handle keybinds
       
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            
            checkKeyPressed(state);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }


        // handle events
        sf::Event event;

        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }




        }
        ImGui::SFML::Update(window, deltaClock.restart());

        
        createImGuiWindow(running,state.imguiSize,state.sfmlSize);
        generateImGuiTable(currentItems);
        //ImGui::ShowDemoWindow(&running);

        ImGui::End();

       
        window.clear(sf::Color(0,0,0,30));

        ImGui::SFML::Render(window);
        window.display();
        
    }


    unregisterHotkeys();
    std::cout << "Press to leave: ";
    getchar();
    return 0;
}