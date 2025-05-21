#pragma once



#define NOMINMAX
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <ranges>
#include "modules/utilities/trim.cpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGl.hpp>
#include<SFML/Graphics/RenderWindow.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <dwmapi.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "lodepng.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>




using json = nlohmann::json;
#define ASSERTION_ERROR 499



#define ITEMTYPE_fissureItems true
#define ITEMTYPE_relicItems false

void errorLog(const std::string& s);
void warningLog(const std::string& s);
void successLog(const std::string& s);


void myAssert(bool stmt,std::string s);

const std::string CONFIG_FILENAME = "tool_config.txt";
const std::string COPY_FILENAME = "tool_config_old.txt";




const int SFMLWINDOWSIZEX = 1200;
const int SFMLWINDOWSIZEY = 300;

const int IMGUIWINDOWSIZEX = SFMLWINDOWSIZEX - 100;
const int IMGUIWINDOWSIZEY = SFMLWINDOWSIZEY - 100;



//should be a sum of vectors in config.h
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


	"updatingType"

};

namespace Rarity {

	enum level {
		Common,
		Uncommon,
		Rare,
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
			errorLog("Not found property in config: "+ key);
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
			errorLog("Couldnt find " + key + " key in ToolConfig");
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



struct AppState {

	std::map<std::string, ItemDetails>& items;
	ToolConfig& config;
	sf::RenderWindow& window;
	bool& running;
	bool& isVisible;
	MSG& msg;
	WindowParameters& sfmlSize;
	WindowParameters& imguiSize;
	bool& settingsVisible;
	tesseract::TessBaseAPI& tesseractApi;
	bool& shouldReSizeImGui;
	bool& itemDisplayFlag;
	RelicInfo& currentRelic;
	bool& shouldUpdateFonts;
	std::vector<std::string>& allAvalibleItems;


	AppState(
		std::map<std::string, ItemDetails>& i,
		ToolConfig& c,
		sf::RenderWindow& w,
		bool& r,
		bool& v,
		MSG& m,
		WindowParameters& sfmlS,
		WindowParameters& imguiS,
		bool& sv,
		tesseract::TessBaseAPI& t,
		bool& sri,
		bool& idf,
		RelicInfo& cr,
		bool& suf,
		std::vector<std::string>& aai

	) :items(i), config(c), window(w), running(r), isVisible(v), msg(m),sfmlSize(sfmlS),imguiSize(imguiS),settingsVisible(sv),tesseractApi(t),shouldReSizeImGui(sri),itemDisplayFlag(idf),currentRelic(cr),shouldUpdateFonts(suf),allAvalibleItems(aai) {};

};



const std::map<std::string, ItemDetails> exampleItems = {
	std::pair<std::string,ItemDetails>("someItemName1",ItemDetails(5,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName2",ItemDetails(6,std::vector<int>({1,1,1,1,1}),Rarity::level::Uncommon)),
	std::pair<std::string,ItemDetails>("someItemName3",ItemDetails(7,std::vector<int>({1,1,1,1,1}),Rarity::level::Rare)),
	std::pair<std::string,ItemDetails>("someItemName4",ItemDetails(8,std::vector<int>({1,1,1,1,1}),Rarity::level::Undefined)),
	std::pair<std::string,ItemDetails>("someItemName5",ItemDetails(9,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName6",ItemDetails(10,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName7",ItemDetails(11,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName8",ItemDetails(12,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName9",ItemDetails(13,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName10",ItemDetails(14,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName11",ItemDetails(15,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName12",ItemDetails(16,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName13",ItemDetails(17,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName14",ItemDetails(18,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName15",ItemDetails(19,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	std::pair<std::string,ItemDetails>("someItemName16",ItemDetails(20,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
};


#include "modules/ocr/ocr.h"
#include "modules/utilities/utilities.h"
#include "modules/keybindings/keybindings.h"
#include "modules/gui/gui.h"
#include "modules/config/config.h"
#include "modules/relics/relics.h"