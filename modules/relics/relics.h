#pragma once
#include "../../platinumEyes.h"


int parseRelicData();
std::array<std::string, 6> getRelicRawItems(std::string relic);
std::array<std::pair<std::string, std::string>, 6> getRelicItemDetails(std::string relic);
RelicInfo FetchRelicItemPrices(std::string relic);
std::string rarityToString(Rarity::level r);
std::string relicMenuTitleStringToRelicString(std::string& s);
void loadRelicDatabase(ToolConfig& config);
void printRelic(RelicInfo& relic);
