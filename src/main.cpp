#include "core/core.h"

int main(){
    AppState state = initApp();
    customizeWindow(state);
    setImGuiStyle(state.config);
    std::thread watchEEthread([&state]() {
        while (state.running) {
            listen(state);
        }
        });
    mainLoop(state);
    watchEEthread.join();

    closeApp(state);
}