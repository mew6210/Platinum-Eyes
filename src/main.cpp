#include "core/core.h"

int main(){
    AppState state = initApp();
    customizeWindow(state);
    setImGuiStyle(state.config);
    EELogWatcher eeLogWatcher = listenToEELog(state);
    mainLoop(state);
    closeApp(state);
}