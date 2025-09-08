#pragma once



#define NOMINMAX
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <ranges>
#include "../utilities/trim.cpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <filesystem>
#include "wood.h"



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#include <dwmapi.h>
#endif


#if __linux__
#include <X11/Xlib.h>
#ifdef None
#undef None
#endif
#endif

using json = nlohmann::json;

#define ITEMTYPE_fissureItems true
#define ITEMTYPE_relicItems false


namespace Rarity {

	enum level {
		Common=1,
		Uncommon=2,
		Rare=3,
		Undefined = -1
	};

}

class Point {
public:
 	int x = 0;
	int y = 0;



	Point(int a,int b) {

		x = a;
		y=b;
	}
	Point() {

	}


};

class Timer {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

public:
	void start() {
		begin = std::chrono::steady_clock::now();
	}
	void stop() {
		end = std::chrono::steady_clock::now();
	}

	void print(std::string s) {
		std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms] " << s << std::endl;
	}

	void stop(const std::string& s) {
		end = std::chrono::steady_clock::now();
		std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms] " << s << std::endl;
	}

};

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

struct WFMItem {
	std::string slug = "";
	std::vector<std::string> tags = {};
	int ducats = 0;
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
	bool itemDisplayFlag;
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
		bool idf,
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
		itemDisplayFlag(idf),
		currentRelic(cr),
		shouldUpdateFonts(suf),
		allAvalibleItems(std::move(aai)),
		fpsVisible(fv),
		fpsHidden(fh) {};

};

const std::vector<Item> exampleItems = {
	Item("Mesa Prime Neuroptics","Mesa Prime Neuroptics",ItemDetails(5,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("Blah blah blah mesa prime neuroptics","Blah blah blah mesa prime neuroptics",ItemDetails(6,std::vector<int>({1,1,1,1,1}),Rarity::level::Uncommon)),
	Item("someItemName3","someItemName3",ItemDetails(7,std::vector<int>({1,1,1,1,1}),Rarity::level::Rare)),
	Item("someItemName4","someItemName4",ItemDetails(8,std::vector<int>({1,1,1,1,1}),Rarity::level::Undefined)),
	Item("someItemName5","someItemName5",ItemDetails(9,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName6","someItemName6",ItemDetails(10,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName7","someItemName7",ItemDetails(11,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName8","someItemName8",ItemDetails(12,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName9","someItemName9",ItemDetails(13,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName10","someItemName10",ItemDetails(14,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName11","someItemName11",ItemDetails(15,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName12","someItemName12",ItemDetails(16,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName13","someItemName13",ItemDetails(17,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName14","someItemName14",ItemDetails(18,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName15","someItemName15",ItemDetails(19,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName16","someItemName16",ItemDetails(20,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
};

class KeyBind {

	int key;
	std::string description;

public:
	KeyBind(int k, std::string s) {
		key = k;
		description = s;
	}

	int getKey() {
		return key;
	}
	std::string getDescription() {
		return description;
	}
};


#include "native/nativecore.hpp"
#include "../ocr/ocr.h"
#include "../utilities/utilities.h"
#include "../keybindings/keybindings.h"
#include "../gui/gui.h"
#include "../config/config.h"
#include "../relics/relics.h"
#include "../wfmd/wfmd.hpp"
#include "../clipboard/clipboard.hpp"
#include "../eelogwatcher/eelogwatcher.hpp"

void mainLoop(AppState& state);
std::pair<int, int> getFps(ToolConfig& toolConfig);
void loadDatabases(ToolConfig& toolConfig);
AppState initApp();
void closeApp(AppState& state);