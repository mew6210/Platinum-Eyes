#pragma once
#include "../../platinumEyes.h"

void generateImGuiTable(std::map<std::string, ItemDetails>& items);
HRESULT setTransparency(HWND hWnd);
void customizeWindow(sf::RenderWindow& w, WindowParameters& state);
void createImGuiWindow(bool& isRunning, WindowParameters& imguiParameters, WindowParameters& sfmlParameters, bool& settingsOpen, AppState state, bool& shouldReSizeImGui);
void showSettingsMenu(bool* p_open, AppState state);
void reSizeSfmlWindow(sf::RenderWindow& w, WindowParameters& sfmlParameters);
