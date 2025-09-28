#pragma once
#include <cpr/cpr.h>
#include "../ocr/ocr.h"
#include "../utilities/trim/trim.hpp"
#include "../utilities/utilities.h"
#include "../utilities/timer/timer.hpp"
#include "../config/toolconfig.hpp"
#include "../config/config.h"
#include "relicinfo/relicinfo.hpp"

struct CacheOptions;

int parseRelicData();
std::array<std::string, 6> getRelicRawItems(std::string relic);
std::array<std::pair<std::string, std::string>, 6> getRelicItemDetails(std::string relic);
RelicInfo fetchRelicItemPrices(std::string relicName, const CacheOptions& cacheOpt);
std::string rarityToString(Rarity::level r);
std::string relicMenuTitleStringToRelicString(std::string& s);
void loadRelicDatabase(ToolConfig& config, std::pair<bool, bool>& updateOrders);
void printRelic(RelicInfo& relic);
std::vector<std::string> loadAllAvalibleItemsToVector();
std::pair<bool, bool> shouldUpdateDatabase(ToolConfig& config);
void replaceAmp(std::string& s);
