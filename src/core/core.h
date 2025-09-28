#pragma once

#define NOMINMAX
#include <string>
#include <vector>
#include <chrono>
#include <nlohmann/json.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "wood.h"
#include <atomic>

#include "native/nativeheaders.hpp"

#include "../config/toolconfig.hpp"
#include "../utilities/utilities.h"
#include "../items/item/item.hpp"
#include "../relics/relicinfo/relicinfo.hpp"

struct AppState {

	std::atomic<bool> eeLogTakeScreenshot = false;		//idk

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	MSG msg;
#endif

#if __linux__
		XEvent msg;		//idk
#endif

	std::vector<Item> items;	//data
	ToolConfig config;			//settings
	std::unique_ptr<sf::RenderWindow> window;	//graphic
	bool running;	//idk
	bool isVisible;	//graphic
	WindowParameters sfmlSize;	//graphic
	WindowParameters imguiSize;	//graphic
	bool settingsVisible;	//graphic
	std::unique_ptr<tesseract::TessBaseAPI> tesseractApi; //ocr
	bool shouldReSizeImGui;	//graphic
	ItemDisplayMode itemDisplayMode;	//graphic
	RelicInfo currentRelic;		//data
	bool shouldUpdateFonts;		//graphic
	std::vector<std::string> allAvalibleItems; //data
	int fpsVisible;	//graphic
	int fpsHidden;	//graphic


	AppState(
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		MSG m,
#endif
#if __linux__
		XEvent m,
#endif

		std::vector<Item> i,
		ToolConfig c,
		std::unique_ptr<sf::RenderWindow> w,
		bool r,
		bool v,
		WindowParameters sfmlS,
		WindowParameters imguiS,
		bool sv,
		std::unique_ptr<tesseract::TessBaseAPI> t,
		bool sri,
		ItemDisplayMode idm,
		RelicInfo cr,
		bool suf,
		std::vector<std::string> aai,
		int fv,
		int fh

	) :
		msg(m),
		items(std::move(i)), 
		config(std::move(c)), 
		window(std::move(w)),
		running(r), 
		isVisible(v), 
		
		sfmlSize(sfmlS),
		imguiSize(imguiS),
		settingsVisible(sv),
		tesseractApi(std::move(t)),
		shouldReSizeImGui(sri),
		itemDisplayMode(idm),
		currentRelic(cr),
		shouldUpdateFonts(suf),
		allAvalibleItems(std::move(aai)),
		fpsVisible(fv),
		fpsHidden(fh) {};

};

#include "../ocr/ocr.h"
#include "../gui/gui.h"
#include "../config/config.h"

void mainLoop(AppState& state);
std::pair<int, int> getFps(ToolConfig& toolConfig);
void loadDatabases(ToolConfig& toolConfig);
AppState initApp();
void closeApp(AppState& state);
