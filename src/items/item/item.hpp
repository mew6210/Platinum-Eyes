#pragma once
#include <string>
#include "../itemdetails/itemdetails.hpp"

class Item {
public:
	std::string preparedName = "";
	std::string rawName = "";
	ItemDetails itemDetails;

	Item(std::string prepared = "Undefined", std::string raw = "undefined", ItemDetails details = ItemDetails()):preparedName(prepared),rawName(raw),itemDetails(details) {}
	Item() : preparedName(""), rawName(""), itemDetails(ItemDetails()) {};
};

class RelicItem : public Item {
public:
	float percentage;

	RelicItem(std::string prepared = "Undefined", std::string raw = "undefined", ItemDetails details = ItemDetails(), float perc = 0.0f)
		: Item(prepared, raw, details), percentage(perc) {}
};