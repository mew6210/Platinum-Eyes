#include "../warframe_tool.h"
#include <functional>
#include <imgui_stdlib.h>


void configParameter(std::string& s,int counter,const char* label) {

	std::string labelData(label);
	std::string labelText = labelData + ": ";
	ImGui::Text(labelText.c_str());
	ImGui::SameLine();

	std::string idNumber= std::to_string(counter);
	std::string id = "##label" + idNumber;
	
	ImGui::InputText(id.c_str(), &s);

	
}



struct rightPane {
	std::string description;
	std::function<void(ToolConfig&)> details;
};

void ocrServerSettings(std::string& s1, std::string& s2) {

	configParameter(s1,1,"ocrIp");
	configParameter(s2,2,"ocrPort");
}


void screenshotSettings(std::string& s1, std::string& s2, std::string& s3, std::string& s4) {

	configParameter(s1, 3, "screenShotFilePath");
	configParameter(s2, 4, "coordinatesOfScreenShotCenter");
	configParameter(s3, 5, "screenShotWidth");
	configParameter(s4, 6, "screenShotHeight");



}

void windowSizesSettings(std::string& s1, std::string& s2) {

	configParameter(s1, 7, "sfmlSize");
	configParameter(s2, 8, "imguiSize");


}

void keybindingsSettings(std::string& s1, std::string& s2) {
	
	ImGui::Text("Will be added later");

}



struct settingsStructure{
	static const int length = 4;
	std::vector<std::string> leftPanes = {"Ocr server settings","Screenshot parameters","Window sizes","Keybindings"};
	std::pair < std::string, std::function<void(std::string& s1, std::string& s2)>> ocrServer;
	std::pair < std::string, std::function<void(std::string& s1, std::string& s2, std::string& s3, std::string& s4)>> screenShotParameters;
	std::pair < std::string, std::function<void(std::string& s1, std::string& s2)>> windowSizes;
	std::pair < std::string, std::function<void(std::string& s1, std::string& s2)>> keyBindings;

};


void appendToSettingsStructure(int& should, settingsStructure& structure, AppState state) {

	if (should) {
		
		structure.ocrServer = 
			std::pair<std::string, std::function<void(std::string& s1, std::string& s2)>>
			("Configure settings related to communication between the ocr server(main.py). Ip is usually just your local pc ip, "
			"port is 5055 unless u changed something in main.py ",ocrServerSettings);
			


		structure.screenShotParameters=
			std::pair<std::string, std::function<void(std::string& s1, std::string& s2, std::string& s3, std::string& s4)>>
			( "Here u give the center of the screenshot, how wide and tall it should be."
			"U also include the path where u want to store the screenshot the app will be taking",screenshotSettings);


		structure.windowSizes=
			std::pair<std::string, std::function<void(std::string& s1, std::string& s2)>>
			( "Here u give how big the window size should be. Both the sfml(the one more to the back), and imgui (the one on top).",windowSizesSettings);


		structure.keyBindings=
			std::pair<std::string, std::function<void(std::string& s1, std::string& s2)>>
			( "Keybindings, every keybind is treated as 'Alt + <keybind>'",keybindingsSettings);

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


	static const std::string  ocrIpForRevert = newConfig["ocrIp"];
	static const std::string  ocrPortForRevert = newConfig["ocrPort"];
	static const std::string  screenShotFilePathForRevert = newConfig["screenShotFilePath"];
	static const std::string  coordinatesOfScreenShotCenterForRevert = newConfig["coordinatesOfScreenShotCenter"];
	static const std::string  screenShotWidthForRevert = newConfig["screenShotWidth"];
	static const std::string  screenShotHeightForRevert = newConfig["screenShotHeight"];
	static const std::string  sfmlSizeForRevert = newConfig["sfmlSize"];
	static const std::string  imguiSizeForRevert = newConfig["imguiSize"];
	static const std::string  keybind1ForRevert = newConfig["keybind1"];
	static const std::string  keybind2ForRevert = newConfig["keybind2"];


	static std::string  ocrIp = newConfig["ocrIp"];
	static std::string  ocrPort=newConfig["ocrPort"];
	static std::string  screenShotFilePath=newConfig["screenShotFilePath"];
	static std::string  coordinatesOfScreenShotCenter=newConfig["coordinatesOfScreenShotCenter"];
	static std::string  screenShotWidth=newConfig["screenShotWidth"];
	static std::string  screenShotHeight=newConfig["screenShotHeight"];
	static std::string  sfmlSize=newConfig["sfmlSize"];
	static std::string  imguiSize=newConfig["imguiSize"];
	static std::string  keybind1= newConfig["keybind1"];
	static std::string  keybind2 = newConfig["keybind2"];

	appendToSettingsStructure(should, structure, state);

	

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

					switch (selected) {
					case 0: ImGui::TextWrapped(structure.ocrServer.first.c_str()); break;
					case 1: ImGui::TextWrapped(structure.screenShotParameters.first.c_str()); break;
					case 2: ImGui::TextWrapped(structure.windowSizes.first.c_str()); break;
					case 3: ImGui::TextWrapped(structure.keyBindings.first.c_str()); break;
						
						
						//ImGui::TextWrapped(structure.rightPanes[selected].description.data());
					}

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Details"))
				{
					switch (selected) {

					case 0: structure.ocrServer.second(ocrIp,ocrPort); break;
					case 1: structure.screenShotParameters.second(screenShotFilePath, coordinatesOfScreenShotCenter, screenShotWidth, screenShotHeight); break;
					case 2: structure.windowSizes.second(sfmlSize, imguiSize); break;
					case 3: structure.keyBindings.second(keybind1,keybind2); break;


						//structure.rightPanes[selected].details(newConfig);

					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			
			if (ImGui::Button("Revert")) {

				ocrIp = ocrIpForRevert;
				ocrPort = ocrPortForRevert;
				screenShotFilePath = screenShotFilePathForRevert;
				coordinatesOfScreenShotCenter = coordinatesOfScreenShotCenterForRevert;
				screenShotWidth = screenShotWidthForRevert;
				screenShotHeight = screenShotHeightForRevert;
				sfmlSize = sfmlSizeForRevert;
				imguiSize = imguiSizeForRevert;
				keybind1 = keybind1ForRevert;
				keybind2 = keybind2ForRevert;

			}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {
			
				newConfig.setPropertyValue("ocrIp", ocrIp);
				newConfig.setPropertyValue("ocrPort", ocrPort);
				newConfig.setPropertyValue("screenShotFilePath", screenShotFilePath);
				newConfig.setPropertyValue("coordinatesOfScreenShotCenter", coordinatesOfScreenShotCenter);
				newConfig.setPropertyValue("screenShotWidth", screenShotWidth);
				newConfig.setPropertyValue("screenShotHeight", screenShotHeight);
				newConfig.setPropertyValue("sfmlSize", sfmlSize);
				newConfig.setPropertyValue("imguiSize", imguiSize);
				

				if (newConfig == state.config) {
					std::cout << "You haven't changed anything";
				}
				else {
					std::cout << "New configuration detected";

					state.config = newConfig;

					rewriteConfigFile(state.config);


				}






			}
			ImGui::EndGroup();
		}
	}
	ImGui::End();
}
