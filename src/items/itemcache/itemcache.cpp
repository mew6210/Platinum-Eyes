#include "itemcache.hpp"

std::filesystem::path cacheFilePath = "data/itemcache.txt";

using std::chrono::time_point, std::chrono::system_clock;
using std::filesystem::path;
using std::string;

namespace {

	std::vector<string> splitByDelimeter(const string& line,const char& c) {
		std::vector<string> parts;
		std::stringstream ss(line);
		string token;

		while (std::getline(ss, token, c)) {
			parts.push_back(token);
		}
		return parts;
	}

	string floatToString(float value, int decimals = 2) {
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(decimals) << value;
		return oss.str();
	}

	float stringToFloat(const string& s) {
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
	time_point<system_clock> parseTimestamp(const string& s) {
		long long seconds = std::stoll(s);
		return system_clock::time_point{ std::chrono::seconds(seconds) };
	}


	std::chrono::seconds processTimeToken(const string& token) {

		if (token.size() < 2) {
			errorLog(false, "timeToken should have at least 2 characters");
			return std::chrono::seconds{ 0 };
		}

		long long value = 0;
		try {
			value = std::stoll(token.substr(0, token.size() - 1));
		}
		catch (...) {
			errorLog(false, "failed to parse timeToken, remember a token should have a 1-letter modifier at the end e.g '15m' or '12s'");
			return std::chrono::seconds{ 0 };
		}

		int multiplier = 0;
		switch (token.back()) {
		case 'd': multiplier = 86400; break;
		case 'h': multiplier = 3600; break;
		case 'm': multiplier = 60; break;
		case 's': multiplier = 1; break;
		default: multiplier = 0; break;
		}

		if (multiplier == 0) {
			errorLog(false, "unidentified time modifier, avalible ones are : 'd' 'h' 'm' 's'");
		}

		return std::chrono::seconds{ value * multiplier };
	}
}


std::chrono::seconds parseTimeString(const string& time) {

	auto timeTokens = splitByDelimeter(time, ' ');
	std::chrono::seconds timeSum{ 0 };
	for (const auto& token : timeTokens) {
		std::chrono::seconds time = processTimeToken(token);
		timeSum += time;
	}
	return timeSum;
}

void createItemCache() {
	std::ofstream cacheFile(cacheFilePath,std::ios::app);
}

void saveToItemCache(const Item& item){
	
	std::ofstream cacheFile(cacheFilePath,std::ios_base::app);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
		item.itemDetails.timestamp.time_since_epoch()
	).count();
	cacheFile << item.rawName << ";" << floatToString(item.itemDetails.averagePrice) << ";" << getFormatedLowestPrices(item.itemDetails.lowestPrices) <<";"<<rarityToString(item.itemDetails.rarity) << ";" << seconds<< "\n";
}

void checkLineTimestamp(std::vector<string>& lines,string& line,const time_point<system_clock>& timestamp, const std::chrono::seconds& cacheDuration) {
	auto time_now = std::chrono::system_clock::now();
	if(time_now - timestamp < cacheDuration) lines.push_back(line);
}

void rewriteItemCache(const std::vector<string>& lines) {
	std::ofstream cacheFile(cacheFilePath, std::ios_base::trunc);
	for (const auto& line : lines) {
		cacheFile << line << "\n";
	}
	cacheFile.close();
}

void deleteOldCacheEntries(const path& cacheFilePath,const std::chrono::seconds& cacheDuration) {
	std::ifstream cacheFile(cacheFilePath);
	std::vector<string> lines;
	string line = "";
	while (std::getline(cacheFile, line)) {
		auto fields = splitByDelimeter(line,';');
		if (fields.size() < 5) continue;
		try {
			auto timestamp = parseTimestamp(fields[4]);
			checkLineTimestamp(lines, line, timestamp,cacheDuration);
		}
		catch (...) {
			errorLog(false,"failed to parse cache file");
			continue;
		}
	}
	cacheFile.close();
	rewriteItemCache(lines);
}

std::optional<ItemDetails> readFromItemCache(const string& itemName, const std::chrono::seconds& cacheDuration){
	
	std::vector<string> lines;
	std::ifstream cacheFile(cacheFilePath);
	deleteOldCacheEntries(cacheFilePath,cacheDuration);
	string line = "";
	while (std::getline(cacheFile, line)) {
		auto fields = splitByDelimeter(line,';');
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