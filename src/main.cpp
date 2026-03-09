#include "core/core.h"
#include "eelogwatcher/eelogwatcher.hpp"

int main(){
    AppState state = initApp();
    customizeMainWindow(state.gui);
    setImGuiStyle(state.config);
    EELogWatcher eeLogWatcher = listenToEELog(state);
    mainLoop(state);
    closeApp(state);
}