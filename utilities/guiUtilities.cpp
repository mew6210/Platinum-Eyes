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



std::map<int, std::string> createIntStringMap(std::map<std::string, ProductPricing> items) {

	std::map<int, std::string> intstringmap;

	int it = 0;
	for (auto& p : items) {
		intstringmap.insert(std::pair<int, std::string>(it, p.first));
		it++;
	}

	return intstringmap;
}



void createItemBox(std::pair<std::string,ProductPricing> item) {

	const float TEXT_BASE_WIDTH = ImGui::CalcTextSize(item.first.c_str()).x;


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	window_flags |= ImGuiWindowFlags_MenuBar;
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


	std::string formattedPrice = std::format("{:.2f}", item.second.averagePrice); // s == "3.14"
	ImGui::Text(formattedPrice.c_str());
	ImGui::Text(getFormatedAveragePrices(item.second.lowestPrices).c_str());

	ImGui::EndChild();
	ImGui::PopStyleVar();
}




void generateImGuiTable(std::map<std::string, ProductPricing>& items) {



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
