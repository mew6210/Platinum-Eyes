#include "gui.h"

using std::string;

std::map<int, string> createIntStringMap(std::map<string, ItemDetails> items) {

	std::map<int, string> intstringmap;

	int it = 0;
	for (auto& p : items) {
		intstringmap.insert(std::pair<int, string>(it, p.first));
		it++;
	}

	return intstringmap;
}


namespace {

	ImVec4 getRarityColor(Rarity::level rarity) {
		switch (rarity) {
		case Rarity::level::Common: return { 69, 56, 49,255 }; break;
		case Rarity::level::Uncommon:  return { 66, 66, 66,255 }; break;
		case Rarity::level::Rare:  return { 94, 89, 33,255 }; break;
		default: return { 0,0,0,255 }; break;
		}
	}

 	ImGuiWindowFlags setItemBoxStyles(Rarity::level rarity){
		auto bgColor = getRarityColor(rarity);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		//window_flags |= ImGuiWindowFlags_MenuBar;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(bgColor.x, bgColor.y, bgColor.z, bgColor.w));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		return window_flags;
	}

	void itemBoxName(string& itemName) {
		static ImVec2 topPadding = { 0,3 };
		ImGui::Dummy(topPadding);
		ImGui::TextWrapped(itemName.c_str());
	}


	void itemBoxAvgPrice(float& averagePrice) {
		ImGui::Text("Average price: ");
		ImGui::SameLine();
		string formattedPrice = std::format("{:.2f}", averagePrice);
		ImGui::Text(formattedPrice.c_str());
	}

	void itemBoxLowestOrders(std::vector<int>& lowestPrices) {
		ImGui::Text(getFormatedAveragePrices(lowestPrices).c_str());
	}

	void endItemBox() {
		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

}
void createItemBox(std::string id, Item& item) {
	
	ImGuiWindowFlags flags = setItemBoxStyles(item.itemDetails.rarity);
	ImGui::BeginChild(id.c_str(), ImVec2(200, 100), ImGuiChildFlags_Border, flags);
	
	itemBoxName(item.preparedName);
	itemBoxAvgPrice(item.itemDetails.averagePrice);
	itemBoxLowestOrders(item.itemDetails.lowestPrices);
	endItemBox();
}

namespace {
 	void itemBoxChance(float& percentage) {
		std::string chanceString = "Chance: " + std::format("{:.2f}%%", percentage);
		ImGui::Text(chanceString.c_str());
	}
}
void createRelicItemBox(RelicItem& item,ImVec2& screenSize){

	auto bgColor = getRarityColor(item.itemDetails.rarity);
	auto flags = setItemBoxStyles(item.itemDetails.rarity);
	ImGui::BeginChild(item.rawName.c_str(), ImVec2(screenSize.x/4, screenSize.y/3.5), ImGuiChildFlags_Border, flags);

	itemBoxName(item.preparedName);
	itemBoxChance(item.percentage);
	itemBoxLowestOrders(item.itemDetails.lowestPrices);

	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}


namespace {

	bool isVecNotEmptyAndNotPlaceholder(std::vector<Item>& items) {
		return (items.size() != 0 && items[0].rawName != "placeholder");
	}
	bool isVecPlaceholder(std::vector<Item>& items) {
		return items.size() == 1 && items[0].rawName == "placeholder";
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

	void generateFissureItemsBoxes(AppState& state){

		if (isVecPlaceholder(state.items)) {
			ImGui::Text("Waiting for your input...");
			return;
		}
		if (isVecNotEmptyAndNotPlaceholder(state.items)) {
			generateFissureItemsBoxesNormally(state.items);
			return;
		}
		ImGui::Text("Couldn't find requested text on screen:(\nLook to console for more info");
	}
	//TODO: REFACTOR
	void generateRelicItemsBoxes(AppState& state) {
		if (state.currentRelic.name != "" && state.currentRelic.relicPrice != 0.0) {
			ImVec2 screenSize = ImGui::GetWindowSize();
			int count = state.currentRelic.items.size();

			int it = 1;

			for (auto& item : state.currentRelic.items) {
				createRelicItemBox(item, screenSize);
				if (it != 3 && it != count) {

					ImGui::SameLine();
					ImGui::Dummy({ screenSize.x / 20,0 });
					ImGui::SameLine();
				}
				it++;
			}

			std::string relicName = "Relic name: " + state.currentRelic.name;
			ImGui::Text(relicName.c_str());
			std::string averageRelicPrice = "Average relic price: " + std::format("{:.2f}", state.currentRelic.relicPrice);
			ImGui::Text(averageRelicPrice.c_str());
		}
		else {
			ImGui::Text("Couldn't find relic title :(");
		}
	}
}
void generateImGuiTable(AppState& state) {
	if (state.itemDisplayFlag == ITEMTYPE_fissureItems) {
		generateFissureItemsBoxes(state);
	}
	else if (state.itemDisplayFlag == ITEMTYPE_relicItems) {
		generateRelicItemsBoxes(state);
	}
}

void reSizeSfmlWindow(sf::RenderWindow& w, WindowParameters& sfmlParameters) {

	w.setSize(sf::Vector2u( sfmlParameters.width, sfmlParameters.height ));
	w.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().size.x - (sfmlParameters.width + 25), 25));
}

void customizeWindow(AppState& state) {
	sf::WindowHandle wHandle = state.window->getNativeHandle();
	state.window->setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().size.x - (state.sfmlSize.width+25), 25));
	state.window->setFramerateLimit(state.fpsVisible);
	
	if (!ImGui::SFML::Init(*state.window)) {
		errorLog(true,"ImGui::SFML::Init() failed");
	}

	nativeWindowCustomization(state,wHandle);
}

void setNewFont(ToolConfig& config) {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	std::string filename = "assets/fonts/" + config["fontFile"];
	float size_pixels = std::stof(config["fontSize"]);
	io.Fonts->AddFontFromFileTTF(filename.c_str(), size_pixels);

	ImGui::SFML::UpdateFontTexture();
}

void setImGuiStyle(ToolConfig& config) {
	ImGuiStyle& style = ImGui::GetStyle();
	//style.WindowRounding = 20.0f;
	style.ChildRounding = 20.0f;
	//style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 0.0f;
	
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear(); 
	std::string filename = "assets/fonts/" + config["fontFile"];
	float size_pixels = std::stof(config["fontSize"]);
	io.Fonts->AddFontFromFileTTF(filename.c_str(), size_pixels);
	
	ImGui::SFML::UpdateFontTexture(); 
}

void createImGuiWindow(bool& isRunning,AppState& state) {

	int heightDiff = state.sfmlSize.height - state.imguiSize.height;
	int widthDiff = state.sfmlSize.width - state.imguiSize.width;

	ImGuiWindowFlags flags = ImGuiWindowFlags_None;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_HorizontalScrollbar;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoMove;
	ImGuiCond cond = ImGuiCond_Once;
	ImGui::SetNextWindowBgAlpha(0.7);
	if (state.shouldReSizeImGui) {
		heightDiff = state.sfmlSize.height - state.imguiSize.height;
		widthDiff = state.sfmlSize.width - state.imguiSize.width;
		ImGui::SetNextWindowSize(ImVec2(state.imguiSize.width, state.imguiSize.height),ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(widthDiff / 2, heightDiff / 2), ImGuiCond_Always);
		state.shouldReSizeImGui = !state.shouldReSizeImGui;
	}
	else {
		ImGui::SetNextWindowSize(ImVec2(state.imguiSize.width, state.imguiSize.height), cond);
		ImGui::SetNextWindowPos(ImVec2(widthDiff / 2, heightDiff / 2), cond);
	}
		
	ImGui::Begin("Warframe tool-main", &isRunning, flags);
	

	if (ImGui::SmallButton("Config")) {
		state.settingsVisible= !state.settingsVisible; //flip the settingsOpen
	}
	if (state.settingsVisible == true) {
		showSettingsMenu(&state.settingsVisible,state);
	}
}

void handleBetweenFrameImGuiUpdates(AppState& state) {
	if (state.shouldUpdateFonts) {
		setNewFont(state.config);
		state.shouldUpdateFonts = false;
	}
}

void updateFps(AppState& state) {

	int newVisibleFps = state.fpsVisible;
	int newHiddenFps = state.fpsHidden;
	
	try {
		newVisibleFps = stoi(state.config["fpsVisible"]);
	}
	catch (const std::exception& e) {
		warningLog("visible fps inputted wrong, ",e.what()," defaulting to 30");
		newVisibleFps = 30;
	}

	try {
		newHiddenFps = stoi(state.config["fpsHidden"]);
	}
	catch (const std::exception& e) {
		warningLog("hidden fps inputted wrong, ", e.what(), " defaulting to 30");
		newHiddenFps = 30;
	}
	
	if (state.isVisible) {
		state.window->setFramerateLimit(newVisibleFps);
	}
	else {
		state.window->setFramerateLimit(newHiddenFps);
	}
	state.fpsVisible = newVisibleFps;
	state.fpsHidden = newHiddenFps;
}