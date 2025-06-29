#include "core/core.h"

using std::vector, std::string;

int main()
{
    vector<Item> currentFissureItems;
    currentFissureItems.push_back(Item("placeholder","placeholder",ItemDetails()));

    RelicInfo currentRelic;

    ToolConfig toolConfig = initConfig();
    loadDatabases(toolConfig);
    registerHotkeys(toolConfig);
    

    WindowParameters sfmlSize = getWindowSize("sfml", toolConfig);
    WindowParameters imguiSize = getWindowSize("imgui", toolConfig);


    vector<string> allAvalibleItems = loadAllAvalibleItemsToVector();

    tesseract::TessBaseAPI tesseractapi;
    tesseractInit(tesseractapi);



    sf::RenderWindow window(sf::VideoMode(sfmlSize.width, sfmlSize.height), "Warframe tool", sf::Style::None);
    
        
    bool running = true;
    bool visible = true;
    MSG msg = { 0 };
    bool settingsOpen = false;
    bool shouldReSizeImGui = false;
    bool itemDisplayFlag = ITEMTYPE_fissureItems;
    bool shouldUpdateFonts = false;
    auto fps = getFps(toolConfig);



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
        fps.first,
        fps.second
    );
    

    customizeWindow(state);
    setImGuiStyle(state.config);


    mainLoop(state);

    tesseractapi.End();
    unregisterHotkeys();
    //std::cout << "Press to leave: ";
    //getchar();
    return 0;
}