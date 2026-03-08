#pragma once
#include "../core/core.h"
#include "../keybindings/keybindings.h"
#include "native/guinative.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

void updateFps(ToolConfig& config, GraphicLayer& gui);
void drawData(DataLayer& data);
void customizeWindow(GraphicLayer& data);
void createMainWindow(bool& isRunning,AppState& state);
void showSettingsWindow(bool* p_open, AppState& state);
void reSizeSfmlWindow(sf::RenderWindow& w, WindowParameters& sfmlParameters);
void setImGuiStyle(ToolConfig& config);
void setNewFont(ToolConfig& config);
void handleBetweenFrameImGuiUpdates(AppState& state);
