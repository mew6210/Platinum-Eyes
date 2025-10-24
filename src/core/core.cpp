#include "core.h"
#include "../items/itemcache/itemcache.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

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

    while (state.system->running){
        
        //handle keybinds
        listenAndHandleEvents(state);

        while (const std::optional event = state.gui.window->pollEvent()){
            ImGui::SFML::ProcessEvent(*state.gui.window,*event);

            if (event->is<sf::Event::Closed>())
            {
                // end the program
                state.system->running = false;
            }
        }
        ImGui::SFML::Update(*state.gui.window, deltaClock.restart());


        createImGuiWindow(state.system->running, state);
        generateImGuiTable(state.data);
        //ImGui::ShowDemoWindow(&running);

        ImGui::End();

        state.gui.window->clear(sf::Color(0, 0, 0, 30));

        ImGui::SFML::Render(*state.gui.window);
        state.gui.window->display();

        handleBetweenFrameImGuiUpdates(state);
    }
}

DataLayer initDataLayer() {

    vector<Item> currentFissureItems;
    RelicInfo currentRelic = RelicInfo("", {}, 0);
    vector<string> allAvalibleItems = loadAllAvalibleItemsToVector();
    ItemDisplayMode itemDisplayMode = ItemDisplayMode::StartingScreenDisplay;

    return DataLayer{ currentFissureItems, currentRelic,itemDisplayMode,std::move(allAvalibleItems) };
}

GraphicLayer initGraphicLayer(ToolConfig& config) {

    WindowParameters sfmlSize = getWindowSize("sfml", config);
    WindowParameters imguiSize = getWindowSize("imgui", config);

    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode({
        static_cast<unsigned int>(sfmlSize.width),
        static_cast<unsigned int>(sfmlSize.height) }),
        "Warframe tool",
        sf::Style::None);

    bool visible = true;
    bool settingsOpen = false;
    bool shouldReSizeImGui = false;
    bool shouldUpdateFonts = false;
    auto fps = getFps(config);

    return GraphicLayer{
        std::move(window),
        visible,
        sfmlSize,
        imguiSize,
        settingsOpen,
        shouldReSizeImGui,
        shouldUpdateFonts,
        fps.first,
        fps.second
    };
}

std::unique_ptr<SystemLayer> initSystemLayer() {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    MSG msg = { 0 };
#endif
#if __linux__
    XEvent msg;
#endif
    
    return std::make_unique<SystemLayer>(msg,true);
}

OcrLayer initOcrLayer() {

    auto tesseractapi = std::make_unique<tesseract::TessBaseAPI>();
    tesseractInit(*tesseractapi);
    return OcrLayer{ std::move(tesseractapi) };
}

AppState initApp() {
    std::filesystem::create_directory("data");
    ToolConfig toolConfig = initConfig();
	createItemCache();
    loadDatabases(toolConfig);

    auto dataLayer = initDataLayer();

    registerHotkeys(toolConfig);

    auto graphicLayer = initGraphicLayer(toolConfig);
    auto systemLayer = initSystemLayer();
    auto ocrLayer = initOcrLayer();

    return AppState(
        std::move(dataLayer),
        std::move(toolConfig),
        std::move(graphicLayer),
        std::move(systemLayer),
        std::move(ocrLayer)
    );
}

void closeApp(AppState& state) {
    ImGui::SFML::Shutdown();
    state.ocr.tesseractApi->End();
    unregisterHotkeys();
}