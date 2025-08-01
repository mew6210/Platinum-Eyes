#pragma once
#include "../core/core.h"
#include "native/nativeocr.hpp"
#include <optional>

namespace fuzzy_threshold {
	const int THRESHOLD_LOW = 5;
	const int THRESHOLD_MEDIUM = 10;
	const int THRESHOLD_HIGH = 20;
};



std::vector<std::string> prepareItems(std::vector<std::string>&);
std::string replaceChar(std::string s, char a, std::string b);
ItemDetails getAveragePrice(const json& list);
std::vector<Item> getItemPricesMap(std::vector<std::string>& preparedItems);
std::string getFormatedAveragePrices(std::vector<int>& lowestPrices);
void printItemPrices(std::vector<Item>& itemPrices);


std::vector<Item> prepareItemsForRead(std::vector<Item>& items);


std::pair<std::string, bool> readItemTesseract(cv::Mat& image, tesseract::TessBaseAPI& api, bool showImage);
std::vector<std::string> readFissureItems(tesseract::TessBaseAPI& api, size_t itemCount,std::string& fileName);
std::vector<Item> readFissureRewardsScreen(AppState& state);


std::vector<Item> readPreviousFissureRewardsScreen(AppState& state);

void tesseractInit(tesseract::TessBaseAPI& api);


ItemDetails fetchItemPrice(const std::string& item);
std::string readRelicTesseract(tesseract::TessBaseAPI& api, const char* path, bool showImage);
std::string readRelicTitleTesseract(tesseract::TessBaseAPI& api, const char* path, bool showImage);
RelicInfo readItemsFromRelicTitleTesseract(tesseract::TessBaseAPI& api);
RelicInfo readItemsFromRelicTitleTesseractShifted(tesseract::TessBaseAPI& api);


int checkIfItemsAreValid(std::vector<std::string>& items, std::vector<std::string>& allItems);



bool fixItems(std::vector<std::string>& items, std::vector<std::string>& allItems);
void replaceAnds(std::string& s);