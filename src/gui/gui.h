#pragma once
#include "../core/core.h"
#include "../keybindings/keybindings.h"
#include "native/guinative.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

void updateFps(ToolConfig& config, GraphicLayer& gui);
void drawData(DataLayer& data);
void customizeMainWindow(GraphicLayer& data);
void createMainWindow(bool& isRunning,AppState& state);
void showSettingsWindow(bool* p_open, AppState& state);
void setImGuiStyle(ToolConfig& config);
void handleBetweenFrameImGuiUpdates(AppState& state);