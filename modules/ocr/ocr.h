#pragma once
#include "../../platinumEyes.h"


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


std::map<std::string, ItemDetails> readItemsFromScreenEasyocr(ToolConfig& config);
std::map<std::string, ItemDetails> readItemsFromScreenWithoutScreenshotEasyocr(ToolConfig& config);
std::map<std::string, ItemDetails> prepareItemsForRead(std::map<std::string, ItemDetails>& items);

//takes in name of the file without extension, since its obvious it is .bmp to .png
//for example 'niceItems.bmp' should be passed as 'niceItems'
int convertBMPtoPNG(std::string& path);
std::string readItemTesseract(cv::Mat& image, tesseract::TessBaseAPI& api, bool showImage);
std::vector<std::string> readScreenShotTesseract(tesseract::TessBaseAPI& api, size_t itemCount);
std::map<std::string, ItemDetails> readItemsFromScreenTesseract(ToolConfig& config, tesseract::TessBaseAPI& api);
std::map<std::string, ItemDetails> readItemsFromScreen(AppState state);

std::map<std::string, ItemDetails> readItemsFromScreenWithoutScreenShotTesseract(ToolConfig& config, tesseract::TessBaseAPI& api);
std::map<std::string, ItemDetails> readItemsFromScreenWithoutScreenShot(AppState state);
int tesseractInit(tesseract::TessBaseAPI& api, ToolConfig& config);


ItemDetails fetchItemPrice(const std::string& item);
std::string readRelicTesseract(tesseract::TessBaseAPI& api, const char* path, bool showImage);
std::string readRelicTitleTesseract(tesseract::TessBaseAPI& api, const char* path, bool showImage);
RelicInfo readItemsFromRelicTitleTesseract(tesseract::TessBaseAPI& api);
RelicInfo readItemsFromRelicTitleTesseractShifted(tesseract::TessBaseAPI& api);