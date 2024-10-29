// warframe_tool.h: plik dołączany dla standardowych systemowych plików dołączanych,
// lub pliki dołączane specyficzne dla projektu.

#pragma once

#include <iostream>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
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

using json = nlohmann::json;


//CONSTANTS

const std::string CONFIG_FILENAME = "tool_config.txt";
const std::string configProperties[] = { "ocrIp","ocrPort","screenShotFilePath","coordinatesOfScreenShotCenter","screenShotWidth","screenShotHeight" };





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
		std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "milliseconds " <<" Element: " <<s << std::endl;
	}




};



class ProductPricing{

public:

	float averagePrice = 0;
	std::vector<int> lowestPrices;


	ProductPricing(float avg,std::vector<int> lowestprices) {
		averagePrice = avg;
		lowestPrices = lowestprices;


	}
	ProductPricing(float avg, int lowestprices) {
		averagePrice = 0;
		lowestPrices = std::vector<int>{
		0,0,0,0,0
		};
	}

};



class ToolConfig {
public:



	std::string ocrIp;
	std::string ocrPort;
	std::string screenShotFilePath;
	Point coordinatesOfScreenShotCenter = Point(0, 0);
	int screenShotWidth;
	int screenShotHeight;







	void setOcrIp(std::string s) {
		trim(s);
		ocrIp = s;
	}

	void setOcrPort(std::string s) {
		trim(s);
		ocrPort = s;
	}

	void setScreenShotFilePath(std::string s) {
		trim(s);
		screenShotFilePath = s;
	}

	void setCoordinatesOfScreenShotCenter(std::string s) {

		int middlePos=s.find(",");

		std::string x = s.substr(0, middlePos);
		std::string y = s.substr(middlePos+1, s.length() - x.length());

		coordinatesOfScreenShotCenter = Point(std::stoi(x), std::stoi(y));

	}

	void setScreenShotWidth(std::string s) {
		screenShotWidth = stoi(s);
	}

	void setScreenShotHeight(std::string s) {
		screenShotHeight = stoi(s);
	}

	


	ToolConfig() {
		ocrIp = '0';
		 ocrPort = '0';
		 screenShotFilePath = '0';
		 coordinatesOfScreenShotCenter = Point(0,0);
		 screenShotWidth = '0';
		 screenShotHeight = '0';
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


ProductPricing getAveragePrice(const json& list);



std::map<std::string, ProductPricing> getItemPricesMap(std::vector<std::string>& preparedItems);
std::string getFormatedAveragePrices(std::vector<int>& lowestPrices);


void printItemPrices(std::map<std::string, ProductPricing>& itemPrices);




std::map<std::string, ProductPricing> readItemsFromScreen(ToolConfig& config); 


bool checkIfConfigFileExists();


void createConfigFile();

ToolConfig readConfigFile();
