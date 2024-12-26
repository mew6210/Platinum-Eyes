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



void createItemBox(std::pair<std::string,ItemDetails> item) {

	const float TEXT_BASE_WIDTH = ImGui::CalcTextSize(item.first.c_str()).x;


	ImVec4 bgColor = { 0,0,0,1 };
	switch (item.second.rarity) {
	case Rarity::level::Common:bgColor = {189,145,119,128}; break;
	case Rarity::level::Uncommon: bgColor = {209,208,209,128}; break;
	case Rarity::level::Rare: bgColor = {236,225,117,128}; break;
	default: bgColor = { 0,0,0,255 }; break;
	}
	


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	window_flags |= ImGuiWindowFlags_MenuBar;
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(bgColor.x, bgColor.y, bgColor.z, bgColor.w));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::BeginChild(item.first.c_str(), ImVec2(TEXT_BASE_WIDTH + 50, 100), ImGuiChildFlags_Border, window_flags);

	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(item.first.c_str()))
		{

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	


	ImGui::Text("Average price: ");
	ImGui::SameLine();


	std::string formattedPrice = std::format("{:.2f}", item.second.averagePrice); 
	ImGui::Text(formattedPrice.c_str());
	ImGui::Text(getFormatedAveragePrices(item.second.lowestPrices).c_str());

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

		if (itemCount != 0) {

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
		else {
			ImGui::NewLine();
			ImGui::Text("couldnt find relic rewards :(");
		}
	}
	else if (state.itemDisplayFlag == ITEMTYPE_relicItems) {
		if (state.currentRelic.name != "") {
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
		ImGui::NewLine();
		
			std::string relicName = "Relic name: " + state.currentRelic.name;
			ImGui::Text(relicName.c_str());
			std::string averageRelicPrice = "Average relic price: " + std::format("{:.2f}", state.currentRelic.relicPrice);
			ImGui::Text(averageRelicPrice.c_str());
		}

	}

}



void reSizeSfmlWindow(sf::RenderWindow& w, WindowParameters& sfmlParameters) {

	w.setSize(sf::Vector2u( sfmlParameters.width, sfmlParameters.height ));
	w.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width - (sfmlParameters.width + 25), 25));
	
}


void customizeWindow(sf::RenderWindow& w,WindowParameters& sfmlParameters) {
	HWND hwnd = w.getSystemHandle();

	w.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width - (sfmlParameters.width+25), 25));
	
	w.setFramerateLimit(60);
	ImGui::SFML::Init(w);

	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	style |= WS_EX_TOOLWINDOW;
	SetWindowLongPtr(hwnd, GWL_EXSTYLE, style);
	setTransparency(hwnd);



}

void setImGuiStyle() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 20.0f;
	style.ChildRounding = 20.0f;
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 0.0f;
	

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear(); 
	
	io.Fonts->AddFontFromFileTTF("Lexend-Regular.ttf", 16.f);
	

	ImGui::SFML::UpdateFontTexture(); 


}





void createImGuiWindow(bool& isRunning,WindowParameters& imguiParameters,WindowParameters& sfmlParameters,bool& settingsOpen,AppState state, bool& shouldReSizeImGui) {

	
	int heightDiff = sfmlParameters.height - imguiParameters.height;
	int widthDiff = sfmlParameters.width - imguiParameters.width;

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_HorizontalScrollbar;
	ImGuiCond cond = ImGuiCond_Once;
	ImGui::SetNextWindowBgAlpha(0.7);
	if (shouldReSizeImGui) {
		heightDiff = sfmlParameters.height - imguiParameters.height;
		widthDiff = sfmlParameters.width - imguiParameters.width;
		ImGui::SetNextWindowSize(ImVec2(imguiParameters.width, imguiParameters.height),ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(widthDiff / 2, heightDiff / 2), ImGuiCond_Always);
		shouldReSizeImGui = !shouldReSizeImGui;
	}
	else {
		ImGui::SetNextWindowSize(ImVec2(imguiParameters.width, imguiParameters.height), cond);
		ImGui::SetNextWindowPos(ImVec2(widthDiff / 2, heightDiff / 2), cond);
	}
		
	ImGui::Begin("Warframe tool-main", &isRunning, flags);
	

	if (ImGui::SmallButton("Config")) {
		settingsOpen = !settingsOpen; //flip the settingsOpen
	}
	if (settingsOpen == true) {
		showSettingsMenu(&settingsOpen,state);
	}
}