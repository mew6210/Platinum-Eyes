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
        sf::Event event;

        while (state.window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                // end the program
                state.running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
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