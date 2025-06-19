#pragma once
#include "../core/core.h"


namespace fuzzy_threshold {
	const int THRESHOLD_LOW = 5;
	const int THRESHOLD_MEDIUM = 10;
	const int THRESHOLD_HIGH = 20;
};



BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);

HBITMAP takeScreenshot(int imageWidth = 1200, int imageHeight = 800, int offsetX = 200, int offsetY = 200);
HBITMAP takeScreenshot(int imageWidth = 1200, int imageHeight = 800, Point center = Point(0, 0));
void saveScreenshotToClipboard(HBITMAP bitmap);
void saveItemsToClipboard(std::vector<Item>& items);

std::vector<std::string> prepareItems(std::vector<std::string>&);
std::string replaceChar(std::string s, char a, std::string b);
ItemDetails getAveragePrice(const json& list);
std::vector<Item> getItemPricesMap(std::vector<std::string>& preparedItems);
std::string getFormatedAveragePrices(std::vector<int>& lowestPrices);
void printItemPrices(std::vector<Item>& itemPrices);


std::vector<Item> prepareItemsForRead(std::vector<Item>& items);


std::string readItemTesseract(cv::Mat& image, tesseract::TessBaseAPI& api, bool showImage);
std::vector<std::string> readFissureItems(tesseract::TessBaseAPI& api, size_t itemCount,std::string& fileName);
std::vector<Item> readFissureRewardsScreen(AppState state);


std::vector<Item> readPreviousFissureRewardsScreen(AppState state);

void tesseractInit(tesseract::TessBaseAPI& api);


ItemDetails fetchItemPrice(const std::string& item);
std::string readRelicTesseract(tesseract::TessBaseAPI& api, const char* path, bool showImage);
std::string readRelicTitleTesseract(tesseract::TessBaseAPI& api, const char* path, bool showImage);
RelicInfo readItemsFromRelicTitleTesseract(tesseract::TessBaseAPI& api);
RelicInfo readItemsFromRelicTitleTesseractShifted(tesseract::TessBaseAPI& api);


int checkIfItemsAreValid(std::vector<std::string>& items, std::vector<std::string>& allItems);



void fixItems(std::vector<std::string>& items, std::vector<std::string>& allItems);