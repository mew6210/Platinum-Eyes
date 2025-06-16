#pragma once
#include "../core.h"

void generateImGuiTable(AppState state);
HRESULT setTransparency(HWND hWnd);
void customizeWindow(sf::RenderWindow& w, WindowParameters& state);
void createImGuiWindow(bool& isRunning, WindowParameters& imguiParameters, WindowParameters& sfmlParameters, bool& settingsOpen, AppState state, bool& shouldReSizeImGui);
void showSettingsMenu(bool* p_open, AppState state);
void reSizeSfmlWindow(sf::RenderWindow& w, WindowParameters& sfmlParameters);
void setImGuiStyle(ToolConfig& config);
void setNewFont(ToolConfig& config);
void handleBetweenFrameImGuiUpdates(AppState state);
