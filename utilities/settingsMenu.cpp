#include "../warframe_tool.h"
#include <functional>
#include <imgui_stdlib.h>


void configParameter(ToolConfig config,std::string s) {




	std::string displayString = s + ": " + config[s];

	ImGui::Text(displayString.c_str());

	
}



struct rightPane {
	std::string description;
	std::function<void(ToolConfig&)> details;
};

void ocrServerSettings(ToolConfig& config) {

	configParameter(config, "ocrIp");
	configParameter(config, "ocrPort");



}


void screenshotSettings(ToolConfig& config) {
	configParameter(config, "screenShotFilePath");
	configParameter(config, "coordinatesOfScreenShotCenter");
	configParameter(config, "screenShotWidth");
	configParameter(config, "screenShotHeight");

}

void windowSizesSettings(ToolConfig& config) {
	configParameter(config, "sfmlSize");
	configParameter(config, "imguiSize");

}

void keybindingsSettings(ToolConfig& config) {
	
	ImGui::Text("TODO: KEYBINDS");

}



struct settingsStructure{
	static const int length = 4;
	std::vector<std::string> leftPanes = {"Ocr server settings","Screenshot parameters","Window sizes","Keybindings"};
	std::vector<rightPane> rightPanes;
};


void appendToSettingsStructure(int& should, settingsStructure& structure, AppState state) {

	if (should) {

		structure.rightPanes.push_back({ "Configure settings related to communication between the ocr server(main.py). Ip is usually just your local pc ip, "
			"port is 5055 unless u changed something in main.py ",ocrServerSettings});
		structure.rightPanes.push_back({ "Here u give the center of the screenshot, how wide and tall it should be." 
			"U also include the path where u want to store the screenshot the app will be taking",screenshotSettings});
		structure.rightPanes.push_back({ "Here u give how big the window size should be. Both the sfml(the one more to the back), and imgui (the one on top).",windowSizesSettings});
		structure.rightPanes.push_back({ "Keybindings, every keybind is treated as 'Alt + <keybind>'",keybindingsSettings});

		should = 0;
	}
	else {
		return;
	}


}




void showSettingsMenu(bool* p_open,AppState state)
{
	static settingsStructure structure;
	static int should = 1;
	static ToolConfig newConfig = state.config;





	appendToSettingsStructure(should, structure, state);

	myAssert(structure.leftPanes.size() ==structure.rightPanes.size(),"left pane components and right pane components should amount to the same number");
	

	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
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
			ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border);
			for (int i = 0; i < structure.length; i++)
			{
				char* label = structure.leftPanes[i].data();

				sprintf(label, label);
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
			ImGui::Text(structure.leftPanes[selected].data());
			ImGui::Separator();
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Description"))
				{
					ImGui::TextWrapped(structure.rightPanes[selected].description.data());
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Details"))
				{
					structure.rightPanes[selected].details(newConfig);
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
