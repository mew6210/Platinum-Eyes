#include "core.h"

using std::vector, std::string;

int main()
{
    vector<Item> currentFissureItems;
    currentFissureItems.push_back(Item("placeholder","placeholder",ItemDetails()));

    RelicInfo currentRelic;

    ToolConfig toolConfig = initConfig();
    loadRelicDatabase(toolConfig);
    registerHotkeys(toolConfig);


    WindowParameters sfmlSize = getWindowSize("sfml", toolConfig);
    WindowParameters imguiSize = getWindowSize("imgui", toolConfig);


    vector<string> allAvalibleItems = loadAllAvalibleItemsToVector();

    tesseract::TessBaseAPI tesseractapi;
    tesseractInit(tesseractapi);



    sf::RenderWindow window(sf::VideoMode(sfmlSize.width, sfmlSize.height), "Warframe tool", sf::Style::None);
    
    sf::Clock deltaClock;
    
    bool running = true;
    bool visible = true;
    MSG msg = { 0 };
    bool settingsOpen = false;
    bool shouldReSizeImGui = false;
    bool itemDisplayFlag = ITEMTYPE_fissureItems;
    bool shouldUpdateFonts = false;
    int fpsVisible = std::stoi(toolConfig["fpsVisible"]);
    int fpsHidden = std::stoi(toolConfig["fpsHidden"]);


    AppState state(
        currentFissureItems,
        toolConfig,
        window,
        running,
        visible,
        msg,
        sfmlSize,
        imguiSize,
        settingsOpen,
        tesseractapi,
        shouldReSizeImGui,
        itemDisplayFlag,
        currentRelic,
        shouldUpdateFonts,
        allAvalibleItems,
        fpsVisible,
        fpsHidden
    );
    

    customizeWindow(window,state.sfmlSize);
    setImGuiStyle(state.config);


    while (running)
    {
        //handle keybinds
       
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            
            handleEvents(state);
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

        
        createImGuiWindow(running,state);
        generateImGuiTable(state);
        //ImGui::ShowDemoWindow(&running);

        ImGui::End();

       
        window.clear(sf::Color(0,0,0,30));

        ImGui::SFML::Render(window);
        window.display();
        
        handleBetweenFrameImGuiUpdates(state);

    }

    tesseractapi.End();
    unregisterHotkeys();
    //std::cout << "Press to leave: ";
    //getchar();
    return 0;
}