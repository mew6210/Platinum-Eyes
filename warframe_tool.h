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
using json = nlohmann::json;



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




// TODO: W tym miejscu przywołaj dodatkowe nagłówki wymagane przez program.





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




std::map<std::string, ProductPricing> readItemsFromScreen(); 



