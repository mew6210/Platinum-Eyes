#include "warframe_tool.h"


using json = nlohmann::json;
int main()
{


    std::map<std::string, ItemDetails> currentItems;

    if (!checkIfConfigFileExists()) {
        createConfigFile();
        std::cout << "Config file successfully created. For program to work, configure the config file named tool_config.txt \nFor tips check github repo.";
        return 0;
    }

    ToolConfig toolConfig = readConfigFile();

    registerHotkeys();

    sf::RenderWindow window(sf::VideoMode(1200, 300), "OpenGL", sf::Style::None, sf::ContextSettings(32));
    
    customizeWindow(window);

    sf::Clock deltaClock;
    // run the main loop
    bool running = true;
    bool visible = true;
    while (running)
    {
        //handle keybinds
        MSG msg = { 0 };
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            
            checkKeyPressed(msg,currentItems,toolConfig,running,visible,window);

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

        
        createImGuiWindow(running);
        generateImGuiTable(currentItems);
        //ImGui::ShowDemoWindow(&running);

        ImGui::End();

       
        window.clear(sf::Color(0,0,0,30));

        ImGui::SFML::Render(window);
        window.display();
        
    }





    return 0;
}