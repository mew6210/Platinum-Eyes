#include "gui.h"

static ImVec4 getRarityColor(Rarity::level rarity) {
	switch (rarity) {
	case Rarity::level::Common: return { 69, 56, 49,255 }; break;
	case Rarity::level::Uncommon:  return { 66, 66, 66,255 }; break;
	case Rarity::level::Rare:  return { 94, 89, 33,255 }; break;
	default: return { 0,0,0,255 }; break;
	}
}

static bool isVecNotEmptyAndNotPlaceholder(std::vector<Item>& items) {
	return (items.size() != 0);
}

//itemBox
namespace {	

	ImGuiWindowFlags setItemBoxStyles(Rarity::level rarity) {
		auto bgColor = getRarityColor(rarity);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		//window_flags |= ImGuiWindowFlags_MenuBar;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(bgColor.x, bgColor.y, bgColor.z, bgColor.w));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		return window_flags;
	}

	void itemBoxName(std::string& itemName) {
		//static ImVec2 topPadding = { 0,3 };
		//ImGui::Dummy(topPadding);
		ImGui::TextWrapped(itemName.c_str());
		ImGui::Dummy(ImVec2(0, 5));
	}

	void itemTimestamp(const std::chrono::time_point<std::chrono::system_clock>& timestamp) {
		using namespace std::chrono;

		// compute elapsed time
		auto now = system_clock::now();
		auto diff = duration_cast<seconds>(now - timestamp).count();

		std::string timeStr;
		if (diff < 60) {
			timeStr = std::format("{} seconds ago", diff);
		}
		else if (diff < 3600) {
			timeStr = std::format("{} minutes ago", diff / 60);
		}
		else if (diff < 86400) {
			timeStr = std::format("{} hours ago", diff / 3600);
		}
		else {
			timeStr = std::format("{} days ago", diff / 86400);
		}

		ImGui::Text(timeStr.c_str());
	}

	void itemBoxAvgPrice(float& averagePrice) {
		ImGui::Text("Average price: ");
		ImGui::SameLine();
		std::string formattedPrice = std::format("{:.2f}", averagePrice);
		ImGui::Text(formattedPrice.c_str());
	}

	void itemBoxLowestOrders(std::vector<int>& lowestPrices) {
		ImGui::Text(getFormatedLowestPrices(lowestPrices).c_str());
	}

	void itemBoxChance(float& percentage) {
		std::string chanceString = "Chance: " + std::format("{:.2f}%%", percentage);
		ImGui::Text(chanceString.c_str());
	}

	void endItemBox() {
		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

	void createItemBox(std::string id, Item& item) {

		ImGuiWindowFlags flags = setItemBoxStyles(item.itemDetails.rarity);
		ImGui::BeginChild(id.c_str(), ImVec2(200, 100), ImGuiChildFlags_Border, flags);

		itemBoxName(item.preparedName);
		itemBoxAvgPrice(item.itemDetails.averagePrice);
		itemBoxLowestOrders(item.itemDetails.lowestPrices);
		itemTimestamp(item.itemDetails.timestamp);
		endItemBox();
	}

	void generateFissureItemsBoxesNormally(std::vector<Item>& items) {

		ImGui::Dummy(ImVec2(50.0, 0.0));
		int it = 0;
		for (auto& item : items) {
			createItemBox(std::to_string(it), item);
			ImGui::SameLine();

			if (it != items.size() - 1) {
				ImGui::Dummy(ImVec2(50.0, 0.0));
				ImGui::SameLine();
			}
			it++;
		}
	}

	void drawFissureItemsBoxes(DataLayer& data) {

		if (isVecNotEmptyAndNotPlaceholder(data.items)) {
			generateFissureItemsBoxesNormally(data.items);
			return;
		}
		else {
			ImGui::Text("Couldn't find requested text on screen:(\nLook to console for more info");
		}
	}

}

//itemRelicBox
namespace {

	void createRelicItemBox(RelicItem& item, ImVec2& screenSize) {

		auto bgColor = getRarityColor(item.itemDetails.rarity);
		auto flags = setItemBoxStyles(item.itemDetails.rarity);
		ImGui::BeginChild(item.rawName.c_str(), ImVec2(screenSize.x / 4.f, screenSize.y / 3.5f), ImGuiChildFlags_Border, flags);

		itemBoxName(item.preparedName);
		itemBoxChance(item.percentage);
		itemBoxLowestOrders(item.itemDetails.lowestPrices);

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}


	bool isRelicNotNull(RelicInfo& relic) {
		return (relic.name != "" && relic.relicPrice != 0.0);
	}
	void imGuiRelicInfo(RelicInfo& relic) {
		std::string relicName = "Relic name: " + relic.name;
		ImGui::Text(relicName.c_str());
		std::string averageRelicPrice = "Average relic price: " + std::format("{:.2f}", relic.relicPrice);
		ImGui::Text(averageRelicPrice.c_str());
	}
	void imGuiRelicItemsBoxes(RelicInfo& relic) {
		ImVec2 screenSize = ImGui::GetWindowSize();
		size_t count = relic.items.size();

		int it = 1;

		for (auto& item : relic.items) {
			createRelicItemBox(item, screenSize);
			if (it != 3 && it != count) {

				ImGui::SameLine();
				ImGui::Dummy({ screenSize.x / 20,0 });
				ImGui::SameLine();
			}
			it++;
		}
		imGuiRelicInfo(relic);
	}

	void drawRelicItemsBoxes(DataLayer& data) {
		if (isRelicNotNull(data.currentRelic)) {
			imGuiRelicItemsBoxes(data.currentRelic);
		}
		else {
			ImGui::Text("Couldn't find relic title :(");
		}
	}



}

void drawData(DataLayer& data) {
	switch (data.itemDisplayMode) {
	case ItemDisplayMode::StartingScreenDisplay:ImGui::Text("Waiting for your input..."); break;
	case ItemDisplayMode::FissureDisplay:drawFissureItemsBoxes(data); break;
	case ItemDisplayMode::RelicDisplay:drawRelicItemsBoxes(data); break;
	}
}