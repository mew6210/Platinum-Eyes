#include "../item/item.hpp"
#include "../itemdetails/itemdetails.hpp"
#include "wood.h"
#include <optional>
#include <fstream>
#include "../../ocr/ocr.h"

void createItemCache();
void saveToItemCache(const Item& item);
std::optional<ItemDetails> readFromItemCache(const std::string& itemName);