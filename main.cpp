#include "platinumEyes.h"


using json = nlohmann::json;
int main()
{
    
    std::map<std::string, ItemDetails> currentItems;

    if (!checkIfConfigFileExists()) {
        createConfigFile();
        std::cout << "Config file successfully created. There are still some things that have to be configured, check the settings button and look for '---' parameters, they need to be filled out. \nFor tips check github repo.\n";

    }

    ToolConfig toolConfig = readConfigFile();

    tesseract::TessBaseAPI tesseractapi;
    int ocrType = tesseractInit(tesseractapi,toolConfig);

    /*
    Timer timer = Timer();
    timer.start();
    parseRelicData();
    timer.stop();
    timer.print("parsing database");

    

    timer.start();
    timer.stop();
    timer.print("fetching relics");
    for (auto& price : relic.items) {

        std::cout << "Name: " << std::get<0>(price);
        std::cout << " percentages: " << std::get<1>(price);
        std::cout << " prices: " << getFormatedAveragePrices(std::get<2>(price).lowestPrices);
        std::cout << "rarity: " << rarityToString(std::get<2>(price).rarity) << "\n";
    }
    std::cout << "Average relic price: " << relic.relicPrice;
    */
    Timer timer = Timer();
    timer.start();


    timer.stop();


    registerHotkeys(toolConfig);

    
    WindowParameters sfmlSize = getWindowSize("sfml",toolConfig);
    WindowParameters imguiSize = getWindowSize("imgui",toolConfig);

    sf::RenderWindow window(sf::VideoMode(sfmlSize.width, sfmlSize.height), "Warframe tool", sf::Style::None, sf::ContextSettings(32));
    
    sf::Clock deltaClock;
    
    bool running = true;
    bool visible = true;
    MSG msg = { 0 };
    bool settingsOpen = false;
    bool shouldReSizeImGui = false;
    AppState state(
        currentItems,
        toolConfig,
        window,
        running,
        visible,
        msg,
        sfmlSize,
        imguiSize,
        settingsOpen,
        ocrType,
        tesseractapi,
        shouldReSizeImGui
    );
    


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

        
        createImGuiWindow(running,state.imguiSize,state.sfmlSize,state.settingsVisible,state,shouldReSizeImGui);
        generateImGuiTable(currentItems);
        //ImGui::ShowDemoWindow(&running);

        ImGui::End();

       
        window.clear(sf::Color(0,0,0,30));

        ImGui::SFML::Render(window);
        window.display();
        
    }

    tesseractapi.End();
    unregisterHotkeys();
    //std::cout << "Press to leave: ";
    //getchar();
    return 0;
}