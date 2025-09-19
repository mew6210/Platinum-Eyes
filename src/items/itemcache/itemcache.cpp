#include "itemcache.hpp"

namespace {
	std::vector<std::string> splitBySemicolon(const std::string& line) {
		std::vector<std::string> parts;
		std::stringstream ss(line);
		std::string token;

		while (std::getline(ss, token, ';')) {
			parts.push_back(token);
		}
		return parts;
	}

	std::vector<int> stringToFormattedLowestPrices(const std::string& s) {
		std::vector<int> prices;

		if (s.size() < 2 || s.front() != '(' || s.back() != ')') {
			return prices; // invalid format, return empty
		}

		std::string content = s.substr(1, s.size() - 2); // remove '(' and ')'
		std::stringstream ss(content);
		std::string token;

		while (std::getline(ss, token, ',')) {
			if (!token.empty()) {
				prices.push_back(std::stoi(token));
			}
		}

		return prices;
	}


	Rarity::level rarityFromString(const std::string& s) {
		if (s == "Common")   return Rarity::level::Common;
		if (s == "Uncommon") return Rarity::level::Uncommon;
		if (s == "Rare")     return Rarity::level::Rare;
		return Rarity::level::Undefined;
	}
	std::string floatToString(float value, int decimals = 2) {
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(decimals) << value;
		return oss.str();
	}

	float stringToFloat(const std::string& s) {
		try {
			return std::stof(s);
		}
		catch (const std::invalid_argument&) {
			return 0.0f; // or throw, depending on what you want
		}
		catch (const std::out_of_range&) {
			return 0.0f; // same here
		}
	}
	std::chrono::time_point<std::chrono::system_clock> parseTimestamp(const std::string& s) {
		long long seconds = std::stoll(s);
		return std::chrono::system_clock::time_point{ std::chrono::seconds(seconds) };
	}

}


void createItemCache() {
	std::ofstream cacheFile("data/itemcache.txt");
}

void saveToItemCache(const Item& item){
	
	std::ofstream cacheFile("data/itemcache.txt",std::ios_base::app);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
		item.itemDetails.timestamp.time_since_epoch()
	).count();
	cacheFile << item.rawName << ";" << floatToString(item.itemDetails.averagePrice) << ";" << getFormatedLowestPrices(item.itemDetails.lowestPrices) <<";"<<rarityToString(item.itemDetails.rarity) << ";" << seconds<< "\n";
	
}

std::optional<ItemDetails> readFromItemCache(const std::string& itemName){
	
	std::ifstream cacheFile("data/itemcache.txt");
	std::string line = "";
	while (std::getline(cacheFile, line)) {
		auto fields = splitBySemicolon(line);
		if (fields.size() == 0) return std::nullopt;
		if (fields[0] != itemName) continue;

		try {
			ItemDetails toReturn = ItemDetails(stringToFloat(fields[1]), stringToFormattedLowestPrices(fields[2]), rarityFromString(fields[3]));
			toReturn.timestamp = parseTimestamp(fields[4]);
			return toReturn;
		}
		catch (...) {
			errorLog(false,"failed to parse cache file");
			return std::nullopt;
		}
	}
	return std::nullopt;
}