#include "core/core.h"

using std::vector, std::string;

int main()
{
    AppState state = initApp();

    customizeWindow(state);
    setImGuiStyle(state.config);

    std::thread backgroundThread([&state]() {
        while (state.running) {
            listenToEELog(state); 
        }
        });

    mainLoop(state); 

    backgroundThread.join(); 



    ImGui::SFML::Shutdown();
    state.tesseractApi->End();
    unregisterHotkeys();
    //std::cout << "Press to leave: ";
    //getchar();
    return 0;
}
