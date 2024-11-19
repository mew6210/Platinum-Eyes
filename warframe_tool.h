#pragma once

#include <iostream>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#define NOMINMAX

#include <Windows.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cpr/cpr.h>
#include <vector>
#include <nlohmann/json.hpp>
#include <ranges>
#include "trim.cpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGl.hpp>
#include<SFML/Graphics/RenderWindow.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <dwmapi.h>

using json = nlohmann::json;


void errorLog(std::string s);

const std::string CONFIG_FILENAME = "tool_config.txt";
const std::string COPY_FILENAME = "tool_config_old.txt";
const std::string CONFIGPROPERTIES[] = { "ocrIp","ocrPort","screenShotFilePath","coordinatesOfScreenShotCenter","screenShotWidth","screenShotHeight","sfmlSize","imguiSize"};
const int SFMLWINDOWSIZEX = 1200;
const int SFMLWINDOWSIZEY = 300;

const int IMGUIWINDOWSIZEX = SFMLWINDOWSIZEX - 100;
const int IMGUIWINDOWSIZEY = SFMLWINDOWSIZEY - 100;




namespace Rarity {

	enum level{
		Common,
		Uncommon,
		Rare,
		Undefined
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

public:
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();


	void start_time() {
		begin = std::chrono::steady_clock::now();
	}
	void end_time() {
		end = std::chrono::steady_clock::now();
	}

	void say_time(std::string s) {
		std::cout << s<<": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "milliseconds "<< std::endl;
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
	
};


struct WindowParameters {

	int width=0;
	int height=0;

	WindowParameters(int width, int height) :width(width), height(height) {};
	WindowParameters() {};
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


	AppState(
		std::map<std::string, ItemDetails>& i,
		ToolConfig& c,
		sf::RenderWindow& w,
		bool& r,
		bool& v,
		MSG& m,
		WindowParameters& sfmlS,
		WindowParameters& imguiS

	) :items(i), config(c), window(w), running(r), isVisible(v), msg(m),sfmlSize(sfmlS),imguiSize(imguiS) {};

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






BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);

HBITMAP takeScreenshot(int imageWidth = 1200, int imageHeight = 800, int offsetX = 200, int offsetY = 200);
HBITMAP takeScreenshot(int imageWidth = 1200, int imageHeight = 800, Point center=Point(0,0));
void saveScreenshotToClipboard(HBITMAP bitmap);


std::vector<std::string> extractItemsFromServer(std::string s);
int getItem(std::string s, std::vector<std::string>& list,int iterator);


std::vector<std::string> prepareItems(std::vector<std::string>&);
std::string replaceChar(std::string s, char a, std::string b);


ItemDetails getAveragePrice(const json& list);



std::map<std::string, ItemDetails> getItemPricesMap(std::vector<std::string>& preparedItems);
std::string getFormatedAveragePrices(std::vector<int>& lowestPrices);


void printItemPrices(std::map<std::string, ItemDetails>& itemPrices);




std::map<std::string, ItemDetails> readItemsFromScreen(ToolConfig& config); 

std::map<std::string, ItemDetails> readItemsFromScreenWithoutScreenshot(ToolConfig& config);

bool checkIfConfigFileExists();


void createConfigFile();

ToolConfig readConfigFile();





std::pair<int,int>stringToIntPair(std::string s);



void generateImGuiTable(std::map<std::string, ItemDetails>& items);


HRESULT setTransparency(HWND hWnd);

void registerHotkeys();

void checkKeyPressed(AppState state);



std::map<std::string, ItemDetails> prepareItemsForRead(std::map<std::string, ItemDetails>& items);


void customizeWindow(sf::RenderWindow& w,WindowParameters& state );

void createImGuiWindow(bool& isRunning,WindowParameters& imguiParameters,WindowParameters& sfmlParameters);
void unregisterHotkeys();
void copyConfigToOldFile();

int StringToVirtualKeyCode(std::string s);

WindowParameters getWindowSize(std::string s,ToolConfig& toolconfig);
