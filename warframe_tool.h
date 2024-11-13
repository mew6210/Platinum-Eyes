// warframe_tool.h: plik dołączany dla standardowych systemowych plików dołączanych,
// lub pliki dołączane specyficzne dla projektu.

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


//CONSTANTS
const std::string CONFIG_FILENAME = "tool_config.txt";
const std::string CONFIGPROPERTIES[] = { "ocrIp","ocrPort","screenShotFilePath","coordinatesOfScreenShotCenter","screenShotWidth","screenShotHeight" };


void errorLog(std::string s);





enum Rarity {
	Common,
	Uncommon,
	Rare,
	Undefined
};



class Point {
public:
 	int x = 0;
	int y = 0;



	Point(int a,int b) {

		x = a;
		y=b;


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
	Rarity rarity;

	ItemDetails() : averagePrice(0), lowestPrices({ 0, 0, 0, 0, 0 }),rarity(Undefined) {}

	ItemDetails(float avg,std::vector<int> lowestprices,Rarity r) {
		averagePrice = avg;
		lowestPrices = lowestprices;
		rarity = r;

	}
	ItemDetails(float avg, int lowestprices) {
		averagePrice = 0;
		lowestPrices = std::vector<int>{
		0,0,0,0,0
		};
		rarity = Undefined;
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





Point stringToCoordinates(std::string s);



void generateImGuiTable(std::map<std::string, ItemDetails>& items);


HRESULT setTransparency(HWND hWnd);

void registerHotkeys();
void checkKeyPressed(MSG& msg, std::map<std::string, ItemDetails>& currentItems, ToolConfig& config,bool& runningState,bool& visibilityState, sf::RenderWindow& windowState);

std::map<std::string, ItemDetails> prepareItemsForRead(std::map<std::string, ItemDetails>& items);


void customizeWindow(sf::RenderWindow& w);

void createImGuiWindow(bool& isRunning);