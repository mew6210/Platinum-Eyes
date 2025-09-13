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

#include "native/nativeheaders.hpp"

enum class ItemDisplayMode {
	FissureDisplay = 1,
	RelicDisplay = 2,
	StartingScreenDisplay = 3
};

namespace Rarity {

	enum class level {
		Common=1,
		Uncommon=2,
		Rare=3,
		Undefined = -1
	};

}

class ItemDetails{
public:

	float averagePrice = 0;
	std::vector<int> lowestPrices;
	Rarity::level rarity;

	ItemDetails() : averagePrice(0), lowestPrices({ 0, 0, 0, 0, 0 }), rarity(Rarity::level::Undefined) {}

	ItemDetails(float avg,std::vector<int> lowestprices,Rarity::level r) {
		averagePrice = avg;
		lowestPrices = lowestprices;
		rarity = r;

	}
	ItemDetails(float avg, int lowestprices) {
		averagePrice = 0;
		lowestPrices = std::vector<int>{
		0,0,0,0,0
		};
		rarity = Rarity::level::Undefined;
	}
};

#include "../config/toolconfig.hpp"

struct WindowParameters {
	int width=0;
	int height=0;

	WindowParameters(int width, int height) :width(width), height(height) {};
	WindowParameters() {};
};

class Item {
public:
	std::string preparedName = "";
	std::string rawName = "";
	ItemDetails itemDetails;

	Item(std::string prepared = "Undefined", std::string raw = "undefined", ItemDetails details = ItemDetails()) {
		preparedName = prepared;
		rawName = raw;
		itemDetails = details;
	}
	Item() : preparedName(""), rawName(""), itemDetails(ItemDetails()) {};
};

class RelicItem : public Item {
public:
	float percentage;
	
	RelicItem(std::string prepared = "Undefined", std::string raw = "undefined", ItemDetails details = ItemDetails(), float perc = 0.0f)
		: Item(prepared, raw, details), percentage(perc) {
	}
};

class RelicInfo {
public:
	std::string name;
	std::vector<RelicItem> items;
	float relicPrice;
	RelicInfo(std::string name, 
		std::vector<RelicItem> items,
		float price):

		name(name),
		items(items),
		relicPrice(price)
	{}

	/*
	- calculates relic's relicPrice, if items are provided
	*/
	void calculateRelicPrice() {

		float price = 0.0;
		if (items.empty()) return;

		for (auto& item : items) {
			float averageItemPrice = 0;
			ItemDetails details = item.itemDetails;

			for (int& order : details.lowestPrices) {
				averageItemPrice += order;
			}
			averageItemPrice /= details.lowestPrices.size();
			averageItemPrice *= (item.percentage / 100);

			price += averageItemPrice;

		}
		relicPrice = price;
	}

};


struct AppState {

	std::atomic<bool> eeLogTakeScreenshot = false;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	MSG msg;
#endif

#if __linux__
		XEvent msg;
#endif

	std::vector<Item> items;
	ToolConfig config;
	std::unique_ptr<sf::RenderWindow> window;
	bool running;
	bool isVisible;
	WindowParameters sfmlSize;
	WindowParameters imguiSize;
	bool settingsVisible;
	std::unique_ptr<tesseract::TessBaseAPI> tesseractApi;
	bool shouldReSizeImGui;
	ItemDisplayMode itemDisplayMode;
	RelicInfo currentRelic;
	bool shouldUpdateFonts;
	std::vector<std::string> allAvalibleItems;
	int fpsVisible;
	int fpsHidden;


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
#include "../utilities/utilities.h"
#include "../gui/gui.h"
#include "../config/config.h"

void mainLoop(AppState& state);
std::pair<int, int> getFps(ToolConfig& toolConfig);
void loadDatabases(ToolConfig& toolConfig);
AppState initApp();
void closeApp(AppState& state);