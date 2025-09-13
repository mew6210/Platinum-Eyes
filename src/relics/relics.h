#pragma once
#include "../core/core.h"
#include <cpr/cpr.h>
#include "../utilities/trim/trim.hpp"

int parseRelicData();
std::array<std::string, 6> getRelicRawItems(std::string relic);
std::array<std::pair<std::string, std::string>, 6> getRelicItemDetails(std::string relic);
RelicInfo fetchRelicItemPrices(std::string relic);
std::string rarityToString(Rarity::level r);
std::string relicMenuTitleStringToRelicString(std::string& s);
void loadRelicDatabase(ToolConfig& config, std::pair<bool, bool>& updateOrders);
void printRelic(RelicInfo& relic);
std::vector<std::string> loadAllAvalibleItemsToVector();
std::pair<bool, bool> shouldUpdateDatabase(ToolConfig& config);
void replaceAmp(std::string& s);
