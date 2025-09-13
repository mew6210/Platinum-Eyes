#include <string>
#include <vector>
#include <fstream>
#include "wood.h"
#include "nlohmann/json.hpp"
#include "cpr/cpr.h"

struct WFMItem {
	std::string slug = "";
	std::vector<std::string> tags = {};
	int ducats = 0;
};

void loadWFMD();

int getDucatsFromSlug(const std::string& slug);