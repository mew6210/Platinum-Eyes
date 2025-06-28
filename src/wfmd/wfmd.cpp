#include "wfmd.hpp"

using std::vector, std::string;

int getDucatsFromSlug(const string& slug) {

	std::ifstream file("wfmItems.txt");

	string line = "";
	while (getline(file, line)) {

		if (line.starts_with(slug)) {
			int doubleCommaPos = line.find(":");
			string ducatsString = line.substr(doubleCommaPos+1);
			return std::stoi(ducatsString);
		}

	}



}


vector<WFMItem>fetchWFMItems() {

	vector<WFMItem> items = {};

    cpr::Response r = cpr::Get(cpr::Url{ "https://api.warframe.market/v2/items"},
        cpr::Header{ {"User-Agent","PlatinumEyes/1.0"} ,
		 {"Accept-Encoding","gzip"}
		
		
		}

    );


    json data = json::parse(r.text);


    json products = data["data"];

	for (auto& product : products) {
		WFMItem item = {};

		if (product.contains("slug")) item.slug = product["slug"];
		if (product.contains("tags")) item.tags = product["tags"];
		if (product.contains("ducats")) item.ducats = product["ducats"];
		
		items.push_back(item);


	}


	return items;

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


void saveWFMItemsToFile(vector<WFMItem>& items) {

	std::ofstream file("wfmItems.txt");

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
	

}