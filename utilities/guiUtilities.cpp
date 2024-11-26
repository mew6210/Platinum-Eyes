#include "../warframe_tool.h"



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

void showSettingsMenu(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Settings", p_open, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close", "Ctrl+W")) { *p_open = false; }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Left
		static int selected = 0;
		{
			ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
			for (int i = 0; i < 100; i++)
			{
				// FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
				char label[128];
				sprintf(label, "MyObject %d", i);
				if (ImGui::Selectable(label, selected == i))
					selected = i;
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();

		// Right
		{
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("MyObject: %d", selected);
			ImGui::Separator();
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Description"))
				{
					ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Details"))
				{
					ImGui::Text("ID: 0123456789");
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			if (ImGui::Button("Revert")) {}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {}
			ImGui::EndGroup();
		}
	}
	ImGui::End();
}




void createImGuiWindow(bool& isRunning,WindowParameters& imguiParameters,WindowParameters& sfmlParameters,bool& settingsOpen) {

	setImGuiStyle();
	int heightDiff = sfmlParameters.height - imguiParameters.height;
	int widthDiff = sfmlParameters.width - imguiParameters.width;

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_HorizontalScrollbar;
	ImGuiCond cond = ImGuiCond_Once;
	ImGui::SetNextWindowBgAlpha(0.7);
	ImGui::SetNextWindowSize(ImVec2(imguiParameters.width,imguiParameters.height), cond);
	ImGui::SetNextWindowPos(ImVec2(widthDiff/2, heightDiff/2), cond);
	
	ImGui::Begin("Warframe tool-main", &isRunning, flags);
	
	if (ImGui::SmallButton("Config")) {
		settingsOpen = !settingsOpen; //flip the settingsOpen
	}
	if (settingsOpen == true) {
		showSettingsMenu(&settingsOpen);
	}
}