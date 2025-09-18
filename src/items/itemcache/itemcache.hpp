#include "../item/item.hpp"
#include "../itemdetails/itemdetails.hpp"
#include "wood.h"



void createItemCache();
void saveToItemCache(const Item& item);
ItemDetails readFromItemCache(const std::string& itemName);