#pragma once
#include "../core/core.h"
#include "native/guinative.hpp"

void updateFps(AppState& state);
void generateImGuiTable(AppState& state);
void customizeWindow(AppState& state);
void createImGuiWindow(bool& isRunning,AppState& state);
void showSettingsMenu(bool* p_open, AppState& state);
void reSizeSfmlWindow(sf::RenderWindow& w, WindowParameters& sfmlParameters);
void setImGuiStyle(ToolConfig& config);
void setNewFont(ToolConfig& config);
void handleBetweenFrameImGuiUpdates(AppState& state);
