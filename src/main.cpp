#include "core/core.h"

int main(){
    AppState state = initApp();
    customizeWindow(state);
    setImGuiStyle(state.config);
    listenToEELog(state);
    mainLoop(state);
    closeApp(state);
}