#pragma once
#include "../../platinumEyes.h"


namespace fuzzy_threshold {
	const int THRESHOLD_LOW = 5;
	const int THRESHOLD_MEDIUM = 10;
	const int THRESHOLD_HIGH = 20;
};



BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);

HBITMAP takeScreenshot(int imageWidth = 1200, int imageHeight = 800, int offsetX = 200, int offsetY = 200);
HBITMAP takeScreenshot(int imageWidth = 1200, int imageHeight = 800, Point center = Point(0, 0));
void saveScreenshotToClipboard(HBITMAP bitmap);


std::vector<std::string> prepareItems(std::vector<std::string>&);
std::string replaceChar(std::string s, char a, std::string b);
ItemDetails getAveragePrice(const json& list);
std::map<std::string, ItemDetails> getItemPricesMap(std::vector<std::string>& preparedItems);
std::string getFormatedAveragePrices(std::vector<int>& lowestPrices);
void printItemPrices(std::map<std::string, ItemDetails>& itemPrices);


std::map<std::string, ItemDetails> prepareItemsForRead(std::map<std::string, ItemDetails>& items);

//takes in name of the file without extension, since its obvious it is .bmp to .png
//for example 'niceItems.bmp' should be passed as 'niceItems'
int convertBMPtoPNG(std::string& path);
std::string readItemTesseract(cv::Mat& image, tesseract::TessBaseAPI& api, bool showImage);
std::vector<std::string> readFissureItems(tesseract::TessBaseAPI& api, size_t itemCount);
std::map<std::string, ItemDetails> readFissureRewardsScreen(AppState state);


std::map<std::string, ItemDetails> readPreviousFissureRewardsScreen(AppState state);

void tesseractInit(tesseract::TessBaseAPI& api);


ItemDetails fetchItemPrice(const std::string& item);
std::string readRelicTesseract(tesseract::TessBaseAPI& api, const char* path, bool showImage);
std::string readRelicTitleTesseract(tesseract::TessBaseAPI& api, const char* path, bool showImage);
RelicInfo readItemsFromRelicTitleTesseract(tesseract::TessBaseAPI& api);
RelicInfo readItemsFromRelicTitleTesseractShifted(tesseract::TessBaseAPI& api);


int checkIfItemsAreValid(std::vector<std::string>& items, std::vector<std::string>& allItems);



void fixItems(std::vector<std::string>& items, std::vector<std::string>& allItems);