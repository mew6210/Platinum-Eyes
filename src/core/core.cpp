#include "core.h"


using std::pair, std::string, std::vector;

pair<int, int> getFps(ToolConfig& toolConfig) {


    int fpsVisible;
    try{
        fpsVisible = std::stoi(toolConfig["fpsVisible"]);
    }
    catch (const std::exception& e) {
        fpsVisible = 30;
        warningLog("fpsVisible not a number, defaulting to 30");
    }
    

    int fpsHidden;
    try {
        fpsHidden = std::stoi(toolConfig["fpsHidden"]);
    }
    catch (const std::exception& e) {
        fpsHidden = 30;
        warningLog("fpsHidden not a number, defaulting to 30");
    }

    return { fpsVisible,fpsHidden };
}


void loadDatabases(ToolConfig& toolConfig) {

    auto updateOrders = shouldUpdateDatabase(toolConfig);

    loadRelicDatabase(toolConfig,updateOrders);

    if(updateOrders.first) loadWFMD();
}


void mainLoop(AppState& state) {

    sf::Clock deltaClock;

    while (state.running){
        
        //handle keybinds
        listenAndHandleEvents(state);

        while (const std::optional event = state.window->pollEvent()){
            ImGui::SFML::ProcessEvent(*state.window,*event);

            if (event->is<sf::Event::Closed>())
            {
                // end the program
                state.running = false;
            }
            
        }
        ImGui::SFML::Update(*state.window, deltaClock.restart());


        createImGuiWindow(state.running, state);
        generateImGuiTable(state);
        //ImGui::ShowDemoWindow(&running);

        ImGui::End();

        state.window->clear(sf::Color(0, 0, 0, 30));

        ImGui::SFML::Render(*state.window);
        state.window->display();

        handleBetweenFrameImGuiUpdates(state);

    }
}


AppState initApp() {

    vector<Item> currentFissureItems;
    currentFissureItems.push_back(Item("placeholder", "placeholder", ItemDetails()));

    RelicInfo currentRelic=RelicInfo("",{},0);

    ToolConfig toolConfig = initConfig();

    std::filesystem::create_directory("data");
    loadDatabases(toolConfig);
    registerHotkeys(toolConfig);

    WindowParameters sfmlSize = getWindowSize("sfml", toolConfig);
    WindowParameters imguiSize = getWindowSize("imgui", toolConfig);

    vector<string> allAvalibleItems = loadAllAvalibleItemsToVector();

    auto  tesseractapi = std::make_unique<tesseract::TessBaseAPI>();
    tesseractInit(*tesseractapi);

    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode({
        static_cast<unsigned int>(sfmlSize.width),
        static_cast<unsigned int>(sfmlSize.height) }),
        "Warframe tool",
        sf::Style::None);

    bool running = true;
    bool visible = true;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    MSG msg = { 0 };
#endif
#if __linux__
    XEvent* msg;
#endif

    bool settingsOpen = false;
    bool shouldReSizeImGui = false;
    bool itemDisplayFlag = ITEMTYPE_fissureItems;
    bool shouldUpdateFonts = false;
    auto fps = getFps(toolConfig);


    return AppState(
        msg,
        std::move(currentFissureItems),
        std::move(toolConfig),
        std::move(window),
        running,
        visible,
        sfmlSize,
        imguiSize,
        settingsOpen,
        std::move(tesseractapi),
        shouldReSizeImGui,
        itemDisplayFlag,
        currentRelic,
        shouldUpdateFonts,
        std::move(allAvalibleItems),
        fps.first,
        fps.second
    );
}

void closeApp(AppState& state) {
    ImGui::SFML::Shutdown();
    state.tesseractApi->End();
    unregisterHotkeys();
}