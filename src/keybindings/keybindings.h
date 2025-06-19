#pragma once
#include "../core/core.h"



void registerHotkeys(ToolConfig& config);

void handleEvents(AppState state);
void unregisterHotkeys();
int StringToVirtualKeyCode(std::string s);
void reRegisterHotkeys(ToolConfig& config);

