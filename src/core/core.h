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
#include "wood.h"





using json = nlohmann::json;
#define ASSERTION_ERROR 499



#define ITEMTYPE_fissureItems true
#define ITEMTYPE_relicItems false



void myAssert(bool stmt,std::string s);

const std::string CONFIG_FILENAME = "tool_config.txt";
const std::string COPY_FILENAME = "tool_config_old.txt";




const int SFMLWINDOWSIZEX = 1200;
const int SFMLWINDOWSIZEY = 300;

const int IMGUIWINDOWSIZEX = SFMLWINDOWSIZEX - 100;
const int IMGUIWINDOWSIZEY = SFMLWINDOWSIZEY - 100;



/* 
should be a sum of vectors in config.h
its default property should also be coded in fillOutConfigFile
it should also probably have its own representation in settingsMenu
*/
const std::string CONFIGPROPERTIES[] = {

	"screenShotFilePath",
	"coordinatesOfScreenShotCenter",
	"screenShotWidth",
	"screenShotHeight",


	"sfmlSize",
	"imguiSize",



	"keyBind_ReadItemsFromScreen",
	"keyBind_EscapeProgram",
	"keyBind_ReadPreviousItems",
	"keyBind_WindowVisibility",
	"keyBind_BackupConfig",
	"keyBind_ExampleItems",
	"keyBind_ReadRelicTitle",



	"fontFile",
	"fontSize",




	"data_LastTimeLaunched",
	"data_LatestUpdate",


	"updatingType",

	"fpsVisible",
	"fpsHidden",

	"clipboardCopy",
	"clipboardWatermark"


};

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

class ToolConfig {

	std::map<std::string, std::string> properties;



	std::string getPropertyValue(std::string key) {


		if (properties.count(key) > 0) {

			std::string propertyValue = properties[key];
			trim(propertyValue);
			return propertyValue;
		}
		else {
			errorLog(false,"Not found property in config: ",key);
			return "not found";
		}


	}



public:

	void setPropertyValue(std::string key, std::string value) {

		if (properties.count(key) > 0) {
			trim(value);

			properties[key] = value;


		}
		else {
			errorLog(false,"Couldnt find ",key," key in ToolConfig");
			return;
		}

	}


	ToolConfig() {
		for (std::string property_key : CONFIGPROPERTIES) {
			properties.insert(std::pair<std::string, std::string>(property_key, "undefined"));
		}
	}

	std::string operator[](std::string s) {
		return getPropertyValue(s);
	}


	bool operator==(ToolConfig& otherConfig) {

		for (auto& property : properties) {

			if (otherConfig[property.first] != property.second) return false;

		}

		return true;

	}
	ToolConfig operator=(ToolConfig& config) {

		properties = config.properties;

		return config;
	}

	std::vector<std::string> getDifferenceList(ToolConfig& otherConfig) {
		std::vector<std::string> differenceList;
		for (auto& property : properties) {

			if (otherConfig[property.first] != property.second) differenceList.push_back(property.first);

		}

		return differenceList;

	}


	void printConfig() {

		for (auto& property : properties) {

			std::cout << property.first + ": ";
			std::cout << property.second << std::endl;

		}


	}


};


struct WindowParameters {

	int width=0;
	int height=0;

	WindowParameters(int width, int height) :width(width), height(height) {};
	WindowParameters() {};
};


class RelicInfo {
public:
	std::string name;
	std::vector<std::tuple<std::string, float, ItemDetails>> items;
	float relicPrice;

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



struct WFMItem {

	std::string slug = "";
	std::vector<std::string> tags = {};
	int ducats = 0;

};





struct AppState {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	MSG& msg;
#endif

#if __linux__
		XEvent* msg;
#endif


	std::vector<Item>& items;
	ToolConfig& config;
	sf::RenderWindow& window;
	bool& running;
	bool& isVisible;
	
	WindowParameters& sfmlSize;
	WindowParameters& imguiSize;
	bool& settingsVisible;
	tesseract::TessBaseAPI& tesseractApi;
	bool& shouldReSizeImGui;
	bool& itemDisplayFlag;
	RelicInfo& currentRelic;
	bool& shouldUpdateFonts;
	std::vector<std::string>& allAvalibleItems;
	int& fpsVisible;
	int& fpsHidden;


	AppState(



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

		MSG& m,
#endif
#if __linux__
		XEvent* m,

#endif

		std::vector<Item>& i,
		ToolConfig& c,
		sf::RenderWindow& w,
		bool& r,
		bool& v,
		WindowParameters& sfmlS,
		WindowParameters& imguiS,
		bool& sv,
		tesseract::TessBaseAPI& t,
		bool& sri,
		bool& idf,
		RelicInfo& cr,
		bool& suf,
		std::vector<std::string>& aai,
		int& fv,
		int& fh

	) :
		msg(m),
		items(i), 
		config(c), 
		window(w),
		running(r), 
		isVisible(v), 
		
		sfmlSize(sfmlS),
		imguiSize(imguiS),
		settingsVisible(sv),
		tesseractApi(t),
		shouldReSizeImGui(sri),
		itemDisplayFlag(idf),
		currentRelic(cr),
		shouldUpdateFonts(suf),
		allAvalibleItems(aai),
		fpsVisible(fv),
		fpsHidden(fh) {};

};






const std::vector<Item> exampleItems = {
	Item("someItemName1","someItemName1",ItemDetails(5,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName2","someItemName2",ItemDetails(6,std::vector<int>({1,1,1,1,1}),Rarity::level::Uncommon)),
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



#include "../ocr/ocr.h"
#include "../utilities/utilities.h"
#include "../keybindings/keybindings.h"
#include "../gui/gui.h"
#include "../config/config.h"
#include "../relics/relics.h"
#include "../wfmd/wfmd.hpp"
#include "../native/native.hpp"



void mainLoop(AppState& state);
std::pair<int, int> getFps(ToolConfig& toolConfig);
void loadDatabases(ToolConfig& toolConfig);