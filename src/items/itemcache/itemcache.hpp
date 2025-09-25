#pragma once
#include "../item/item.hpp"
#include "../itemdetails/itemdetails.hpp"
#include "wood.h"
#include <optional>
#include <fstream>
#include "../../utilities/utilities.h"
#include <filesystem>
#include "../../config/toolconfig.hpp"

std::chrono::seconds parseTimeString(const std::string& time);

struct CacheOptions {

	bool shouldCache;
	std::chrono::seconds cacheDuration;

	CacheOptions(const bool& shouldCache,const std::string& cacheDurationString):shouldCache(shouldCache),cacheDuration(parseTimeString(cacheDurationString)){}

};

void createItemCache();
void saveToItemCache(const Item& item);
std::optional<ItemDetails> readFromItemCache(const std::string& itemName,const std::chrono::seconds& cacheDuration);