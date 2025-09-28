#pragma once
#include <tesseract/baseapi.h>
#include "../utilities/timer/timer.hpp"
#include "../utilities/trim/trim.hpp"
#include "../core/core.h"
#include "point/point.hpp"
#include "native/nativeocr.hpp"
#include <sstream>
#include <optional>
#include <cpr/cpr.h>
#include "../clipboard/clipboard.hpp"
#include "../relics/relics.h"
#include "../wfmd/wfmd.hpp"
#include "../utilities/utilities.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

namespace fuzzy_threshold {
	const int THRESHOLD_LOW = 5;
	const int THRESHOLD_MEDIUM = 10;
	const int THRESHOLD_HIGH = 20;
};

struct CacheOptions;

std::string itemToSnakeCase(const std::string& s);
std::vector<std::string> itemsToSnakeCase(const std::vector<std::string>&);
std::string replaceChar(std::string s, char a, std::string b);
ItemDetails getAveragePrice(const nlohmann::json& list);
std::vector<Item> getItemPrices(std::vector<std::string>& preparedItems, const CacheOptions& cacheOpt);
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
RelicInfo readItemsFromRelicTitleTesseract(tesseract::TessBaseAPI& api, const CacheOptions& cacheOpt);
RelicInfo readItemsFromRelicTitleTesseractShifted(tesseract::TessBaseAPI& api, const CacheOptions& cacheOpt);


int checkIfItemsAreValid(std::vector<std::string>& items, std::vector<std::string>& allItems);



bool fixItems(std::vector<std::string>& items, std::vector<std::string>& allItems);
void replaceAnds(std::string& s);
std::string snakeToItem(const std::string& s);