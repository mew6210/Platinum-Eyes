#pragma once

#define NOMINMAX


#include "native/nativeheaders.hpp"


#include "appstate/appstate.hpp"


#include "../ocr/ocr.h"
#include "../gui/gui.h"
#include "../config/config.h"

void mainLoop(AppState& state);
std::pair<int, int> getFps(ToolConfig& toolConfig);
void loadDatabases(ToolConfig& toolConfig);
AppState initApp();
void closeApp(AppState& state);
