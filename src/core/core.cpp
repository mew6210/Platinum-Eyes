#include "core.h"


using std::pair;

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

    while (state.running)
    {
        //handle keybinds

        if (PeekMessage(&state.msg, NULL, 0, 0, PM_REMOVE)) {

            handleEvents(state);
            TranslateMessage(&state.msg);
            DispatchMessage(&state.msg);
        }


        // handle events
        

        while (const std::optional event = state.window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(state.window,*event);

            if (event->is<sf::Event::Closed>())
            {
                // end the program
                state.running = false;
            }
            


        }
        ImGui::SFML::Update(state.window, deltaClock.restart());


        createImGuiWindow(state.running, state);
        generateImGuiTable(state);
        //ImGui::ShowDemoWindow(&running);

        ImGui::End();


        state.window.clear(sf::Color(0, 0, 0, 30));

        ImGui::SFML::Render(state.window);
        state.window.display();

        handleBetweenFrameImGuiUpdates(state);

    }


}