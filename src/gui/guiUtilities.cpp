#include "gui.h"



HRESULT setTransparency(HWND hWnd)
{
	HRESULT hr = S_OK;

	DWM_BLURBEHIND bb = { 0 };

	bb.dwFlags = DWM_BB_ENABLE;
	bb.fEnable = true;
	bb.hRgnBlur = NULL;

	hr = DwmEnableBlurBehindWindow(hWnd, &bb);
	return hr;
}



std::map<int, std::string> createIntStringMap(std::map<std::string, ItemDetails> items) {

	std::map<int, std::string> intstringmap;

	int it = 0;
	for (auto& p : items) {
		intstringmap.insert(std::pair<int, std::string>(it, p.first));
		it++;
	}

	return intstringmap;
}



void createItemBox(Item item) {

	const float TEXT_BASE_WIDTH = ImGui::CalcTextSize(item.preparedName.c_str()).x;


	ImVec4 bgColor = { 0,0,0,1 };
	switch (item.itemDetails.rarity) {
	case Rarity::level::Common:bgColor = {189,145,119,128}; break;
	case Rarity::level::Uncommon: bgColor = {209,208,209,128}; break;
	case Rarity::level::Rare: bgColor = {236,225,117,128}; break;
	default: bgColor = { 0,0,0,255 }; break;
	}
	


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	window_flags |= ImGuiWindowFlags_MenuBar;
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(bgColor.x, bgColor.y, bgColor.z, bgColor.w));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::BeginChild(item.preparedName.c_str(), ImVec2(TEXT_BASE_WIDTH + 50, 100), ImGuiChildFlags_Border, window_flags);

	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(item.preparedName.c_str()))
		{

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	


	ImGui::Text("Average price: ");
	ImGui::SameLine();


	std::string formattedPrice = std::format("{:.2f}", item.itemDetails.averagePrice); 
	ImGui::Text(formattedPrice.c_str());
	ImGui::Text(getFormatedAveragePrices(item.itemDetails.lowestPrices).c_str());

	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	
}

void createRelicItemBox(std::tuple<std::string,float,ItemDetails> item,ImVec2 screenSize){

	


	ImVec4 bgColor = { 0,0,0,1 };
	switch (std::get<2>(item).rarity) {
	case Rarity::level::Common:bgColor = { 189,145,119,128 }; break;
	case Rarity::level::Uncommon: bgColor = { 209,208,209,128 }; break;
	case Rarity::level::Rare: bgColor = { 236,225,117,128 }; break;
	default: bgColor = { 0,0,0,255 }; break;
	}





	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(bgColor.x, bgColor.y, bgColor.z, bgColor.w));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::BeginChild(std::get<0>(item).c_str(), ImVec2(screenSize.x/4, screenSize.y/3.5), ImGuiChildFlags_Border, window_flags);



	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(std::get<0>(item).c_str()))
		{

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	std::string averagePrice = "Average price: ";
	std::string formattedPrice = std::format("{:.2f}", std::get<2>(item).averagePrice);
	ImGui::Text((averagePrice+formattedPrice).c_str());
	ImGui::SameLine();

	std::string chanceString = "Chance: " + std::format("{:.2f}%%", std::get<1>(item));
	ImGui::Text(chanceString.c_str());


	std::string latestString = "Lowest: " + getFormatedAveragePrices(std::get<2>(item).lowestPrices);
	ImGui::Text(latestString.c_str());

	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();




}


void generateImGuiTable(AppState state) {

	if (state.itemDisplayFlag == ITEMTYPE_fissureItems) {
		
		int itemCount = state.items.size();

		if (itemCount != 0&& state.items[0].rawName != "placeholder") {

			ImGui::Dummy(ImVec2(50.0, 0.0));

			int it = 0;
			for (auto& item : state.items) {
				createItemBox(item);
				ImGui::SameLine();

				if (it != state.items.size() - 1) {
					ImGui::Dummy(ImVec2(50.0, 0.0));
					ImGui::SameLine();
				}

				it++;
			}
		}
		else if (itemCount==1&& state.items[0].rawName =="placeholder") {
			ImGui::Text("Waiting for your input...");
		}
		else {
			ImGui::Text("Couldn't find relic rewards :(");
		}
	}
	else if (state.itemDisplayFlag == ITEMTYPE_relicItems) {
		if (state.currentRelic.name != "" && state.currentRelic.relicPrice!=0.0) {
		ImVec2 screenSize = ImGui::GetWindowSize();
		int count=state.currentRelic.items.size();

		int it = 1;

		for (auto& item : state.currentRelic.items) {
			createRelicItemBox(item,screenSize);
			if (it != 3&&it!=count) {
				
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



void reSizeSfmlWindow(sf::RenderWindow& w, WindowParameters& sfmlParameters) {

	w.setSize(sf::Vector2u( sfmlParameters.width, sfmlParameters.height ));
	w.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width - (sfmlParameters.width + 25), 25));
	
}


void customizeWindow(AppState& state) {
	HWND hwnd = state.window.getSystemHandle();

	state.window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width - (state.sfmlSize.width+25), 25));
	
	state.window.setFramerateLimit(state.fpsVisible);
	ImGui::SFML::Init(state.window);

	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	style |= WS_EX_TOOLWINDOW;
	SetWindowLongPtr(hwnd, GWL_EXSTYLE, style);
	setTransparency(hwnd);



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
	style.WindowRounding = 20.0f;
	style.ChildRounding = 20.0f;
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 0.0f;
	

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear(); 
	std::string filename = "assets/fonts/" + config["fontFile"];
	float size_pixels = std::stof(config["fontSize"]);
	io.Fonts->AddFontFromFileTTF(filename.c_str(), size_pixels);
	
	ImGui::SFML::UpdateFontTexture(); 


}





void createImGuiWindow(bool& isRunning,AppState state) {

	
	int heightDiff = state.sfmlSize.height - state.imguiSize.height;
	int widthDiff = state.sfmlSize.width - state.imguiSize.width;

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_HorizontalScrollbar;
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

void handleBetweenFrameImGuiUpdates(AppState state) {

	if (state.shouldUpdateFonts) {
		setNewFont(state.config);
		state.shouldUpdateFonts = false;
	}


}

void updateFps(AppState& state) {

	//TODO: ADD STOI ERROR HANDLING
	int newVisibleFps = state.fpsVisible;
	int newHiddenFps = state.fpsHidden;
	
	try {
		newVisibleFps = stoi(state.config["fpsVisible"]);
	}
	catch (const std::exception& e) {
		warningLog("visibleFps inputted is not a number, defaulting to 30");
		newVisibleFps = 30;
	}

	try {
		newHiddenFps = stoi(state.config["fpsHidden"]);
	}
	catch (const std::exception& e) {
		warningLog("hiddenFps inputted is not a number, defaulting to 30");
	}
	


	if (state.isVisible) {
		state.window.setFramerateLimit(newVisibleFps);
	}
	else {
		state.window.setFramerateLimit(newHiddenFps);
	}
	state.fpsVisible = newVisibleFps;
	state.fpsHidden = newHiddenFps;

}