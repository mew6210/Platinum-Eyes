#include "core/core.h"

using std::vector, std::string;

int main()
{
    vector<Item> currentFissureItems;
    currentFissureItems.push_back(Item("placeholder","placeholder",ItemDetails()));
    std::filesystem::create_directory("data");
    RelicInfo currentRelic;

    ToolConfig toolConfig = initConfig();
    loadDatabases(toolConfig);
    registerHotkeys(toolConfig);
    

    WindowParameters sfmlSize = getWindowSize("sfml", toolConfig);
    WindowParameters imguiSize = getWindowSize("imgui", toolConfig);


    vector<string> allAvalibleItems = loadAllAvalibleItemsToVector();

    tesseract::TessBaseAPI tesseractapi;
    tesseractInit(tesseractapi);



    sf::RenderWindow window(sf::VideoMode({ 
        static_cast<unsigned int>(sfmlSize.width), 
        static_cast<unsigned int>(sfmlSize.height)}),
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



    AppState state(
        msg,
        currentFissureItems,
        toolConfig,
        window,
        running,
        visible,
        sfmlSize,
        imguiSize,
        settingsOpen,
        tesseractapi,
        shouldReSizeImGui,
        itemDisplayFlag,
        currentRelic,
        shouldUpdateFonts,
        allAvalibleItems,
        fps.first,
        fps.second
    );
    

    customizeWindow(state);
    setImGuiStyle(state.config);


    mainLoop(state);

    ImGui::SFML::Shutdown();
    tesseractapi.End();
    unregisterHotkeys();
    //std::cout << "Press to leave: ";
    //getchar();
    return 0;
}
