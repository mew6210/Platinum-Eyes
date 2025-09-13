#include <string>
#include <vector>
#include "../../items/item/item.hpp"


class RelicInfo {
public:
	std::string name;
	std::vector<RelicItem> items;
	float relicPrice;
	RelicInfo(std::string name,std::vector<RelicItem> items,float price);

	void calculateRelicPrice();
};