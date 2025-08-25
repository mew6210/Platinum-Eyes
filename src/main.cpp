#include "core/core.h"

int main(){
    AppState state = initApp();
    customizeWindow(state);
    setImGuiStyle(state.config);
    mainLoop(state);
    closeApp(state);
}