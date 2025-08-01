#include "core/core.h"

using std::vector, std::string;





int main()
{
    AppState state = initApp();

    customizeWindow(state);
    setImGuiStyle(state.config);

    mainLoop(state);

    ImGui::SFML::Shutdown();
    state.tesseractApi->End();
    unregisterHotkeys();
    //std::cout << "Press to leave: ";
    //getchar();
    return 0;
}
