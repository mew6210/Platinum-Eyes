#pragma once
#include <vector>
#include <chrono>

namespace Rarity {

	enum class level {
		Common = 1,
		Uncommon = 2,
		Rare = 3,
		Undefined = -1
	};

}


class ItemDetails {
public:

	float averagePrice = 0;
	std::vector<int> lowestPrices;
	Rarity::level rarity;
	std::chrono::time_point<std::chrono::system_clock> timestamp;

	ItemDetails() : averagePrice(0), lowestPrices({ 0, 0, 0, 0, 0 }), rarity(Rarity::level::Undefined)	{
		timestamp = std::chrono::system_clock::now();
	}

	ItemDetails(float avg, std::vector<int> lowestprices, Rarity::level r) {
		averagePrice = avg;
		lowestPrices = lowestprices;
		rarity = r;
		timestamp = std::chrono::system_clock::now();
	}
	ItemDetails(float avg, int lowestprices) {
		averagePrice = 0;
		lowestPrices = std::vector<int>{
		0,0,0,0,0
		};
		rarity = Rarity::level::Undefined;
		timestamp = std::chrono::system_clock::now();
	}
};
