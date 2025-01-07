#include "platinumEyes.h"


int main()
{
    
    std::map<std::string, ItemDetails> currentFissureItems;
    currentFissureItems.insert({"placeholder",ItemDetails()});
    RelicInfo currentRelic;

    if (!checkIfConfigFileExists()) {
        createConfigFile();
        std::cout << "Config file successfully created. There are still some things that have to be configured, check the settings button and look for '---' parameters, they need to be filled out. \nFor tips check github repo.\n";

    }

    ToolConfig toolConfig = readConfigFile();
    loadRelicDatabase(toolConfig);
    tesseract::TessBaseAPI tesseractapi;
    int ocrType = tesseractInit(tesseractapi,toolConfig);

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
    bool itemDisplayFlag = ITEMTYPE_fissureItems;
    bool shouldUpdateFonts = false;
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
        ocrType,
        tesseractapi,
        shouldReSizeImGui,
        itemDisplayFlag,
        currentRelic,
        shouldUpdateFonts
    );
    


    customizeWindow(window,state.sfmlSize);
    setImGuiStyle(state.config);


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