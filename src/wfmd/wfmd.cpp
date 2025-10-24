#include "wfmd.hpp"

using std::vector, std::string;
using json = nlohmann::json;
int getDucatsFromSlug(const string& slug) {

	std::ifstream file("data/wfmItems.txt");

	string line = "";
	while (getline(file, line)) {

		if (line.starts_with(slug)) {
			size_t doubleCommaPos = line.find(":");
			string ducatsString = line.substr(doubleCommaPos+1);
			return std::stoi(ducatsString);
		}
	}
	return -1;
}

json fetchJsonBlobWFMAllItems() {

	cpr::Response r = cpr::Get(cpr::Url{ "https://api.warframe.market/v2/items" },
		cpr::Header{ {"User-Agent","PlatinumEyes/1.0"} ,
		 {"Accept-Encoding","gzip"} });

	if (r.status_code == 0) {
		errorLog(true,"Check your internet conenction, could not download warframe market database");
		return {};
	}

	if (r.status_code != 200) {
		errorLog(false,"Could not fetch items from warframe market");
		return {};
	}
	json data;
	try {
		data = json::parse(r.text);
	}
	catch (const nlohmann::json::parse_error& err) {
		errorLog(false,"Could not parse json blob from warframe market",string(err.what()));
		return {};
	}

	return data;


}


vector<WFMItem> parseJsonBlobToWFMItems(const json& data) {
	vector<WFMItem> items = {};
	json products;
	if (data.contains("data")) products = data["data"];
	else {
		errorLog(false,"Incorrect json format, couldn't find 'data' field");
		return {};
	}
	items.reserve(products.size());
	for (auto& product : products) {
		WFMItem item = {};

		if (product.contains("slug")) item.slug = product["slug"];
		if (product.contains("tags")) item.tags = product["tags"];
		if (product.contains("ducats")) item.ducats = product["ducats"];

		items.push_back(item);


	}
	return items;

}


vector<WFMItem>fetchWFMItems() {

	auto data = fetchJsonBlobWFMAllItems();
	return parseJsonBlobToWFMItems(data);

}


vector<WFMItem> filteredItems(const vector<WFMItem>& items) {

	vector<WFMItem> newItems = {};

	for (auto& item : items) {

		bool hasPrime = std::find(item.tags.begin(), item.tags.end(), "prime") != item.tags.end();
		bool hasComponent = std::find(item.tags.begin(), item.tags.end(), "component") != item.tags.end();
		bool hasBlueprint = std::find(item.tags.begin(), item.tags.end(), "blueprint") != item.tags.end();

		if ((hasPrime && hasComponent) || (hasPrime && hasBlueprint)) {
			newItems.push_back(item);
		}


	}

	return newItems;

}


void saveWFMItemsToFile(const vector<WFMItem>& items) {

	std::ofstream file("data/wfmItems.txt");

	for (auto& item : items) {
		file << item.slug;
		file << ":";
		file << item.ducats;
		file << "\n";
	}
}

void loadWFMD() {

	auto items = fetchWFMItems();
	items=filteredItems(items);
	saveWFMItemsToFile(items);

	if (items.size() != 0) successLog("Loaded Warframe Market Database");
	else errorLog(false,"Something went wrong during loading Warframe Market Database, displaying rarities might fail.");
}