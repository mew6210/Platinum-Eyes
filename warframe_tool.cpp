// warframe_tool.cpp: definiuje punkt wejścia dla aplikacji.
//

#include "warframe_tool.h"









using json = nlohmann::json;
int main()
{
    std::map<std::string, ProductPricing> currentItems;

    if (!checkIfConfigFileExists()) {
        createConfigFile();
        std::cout << "Config file successfully created. For program to work, configure the config file named tool_config.txt \nFor tips check github repo.";
        return 0;
    }

    ToolConfig toolConfig = readConfigFile();

    registerHotkeys();

    sf::RenderWindow window(sf::VideoMode(1200, 300), "OpenGL", sf::Style::None, sf::ContextSettings(32));
    
    HWND hwnd = window.getSystemHandle();
    
    window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width-1225,25));
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    style |= WS_EX_TOOLWINDOW;
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, style);
    setTransparency(hwnd);
    

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

        

        ImGuiWindowFlags flags = 1 || 128;

        

        ImGuiCond cond = ImGuiCond_Once;
        ImGui::SetNextWindowBgAlpha(0.7);
        ImGui::SetNextWindowSize(ImVec2(1100,200),cond);
        ImGui::SetNextWindowPos(ImVec2(50, 50), cond);
        ImGui::Begin("Hello, world!",&running,flags);
        
        generateImGuiTable(currentItems);
        //ImGui::ShowDemoWindow(&running);




        ImGui::End();

        
        window.clear(sf::Color(0,0,0,30));

        ImGui::SFML::Render(window);
        window.display();
        
    }





    return 0;
}



