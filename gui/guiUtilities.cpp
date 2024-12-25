#include "../platinumEyes.h"



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




void generateImGuiTable(std::map<std::string, ItemDetails>& items) {


	int itemCount = items.size();
	ImGui::Dummy(ImVec2(50.0, 0.0));
	
	int it = 0;
	for (auto& item : items) {
		createItemBox(item);
		ImGui::SameLine();

		if (it != items.size() - 1) {
			ImGui::Dummy(ImVec2(50.0, 0.0));
			ImGui::SameLine();
		}

		it++;
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


}





void createImGuiWindow(bool& isRunning,WindowParameters& imguiParameters,WindowParameters& sfmlParameters,bool& settingsOpen,AppState state, bool& shouldReSizeImGui) {


	

	setImGuiStyle();
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