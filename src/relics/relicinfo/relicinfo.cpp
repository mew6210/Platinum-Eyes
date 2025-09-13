#include "relicinfo.hpp"


RelicInfo::RelicInfo(std::string name,
	std::vector<RelicItem> items,
	float price) :

	name(name),
	items(items),
	relicPrice(price)
{}

/*
- calculates relic's relicPrice, if items are provided
*/
void RelicInfo::calculateRelicPrice() {

	float price = 0.0;
	if (items.empty()) return;

	for (auto& item : items) {
		float averageItemPrice = 0;
		ItemDetails details = item.itemDetails;

		for (int& order : details.lowestPrices) {
			averageItemPrice += order;
		}
		averageItemPrice /= details.lowestPrices.size();
		averageItemPrice *= (item.percentage / 100);

		price += averageItemPrice;

	}
	relicPrice = price;
}
