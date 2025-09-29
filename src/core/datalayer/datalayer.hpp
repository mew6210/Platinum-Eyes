#pragma once
#include "../../items/item/item.hpp"
#include "../../relics/relicinfo/relicinfo.hpp"
#include "../../utilities/utilities.h"


struct DataLayer {

	std::vector<Item> items;
	RelicInfo currentRelic;
	ItemDisplayMode itemDisplayMode;
	std::vector<std::string> allItems;

};