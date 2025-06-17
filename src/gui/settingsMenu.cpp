#include "gui.h"
#include <functional>
#include <imgui_stdlib.h>


using std::string, std::vector, std::pair;


void configParameter(string& s,int counter,const char* label) {

	string labelData(label);
	string labelText = labelData + ": ";
	ImGui::Text(labelText.c_str());
	ImGui::SameLine();

	string idNumber= std::to_string(counter);
	string id = "##label" + idNumber;
	
	ImGui::InputText(id.c_str(), &s);

	
}


void comboParameter(string& s1,const char* label,static const char* options[],int itemsCount) {
	
	static int selecteditem = 0;
	s1 = options[selecteditem];
	
	ImGui::Text("%s: ",label);
	ImGui::SameLine();
	bool check = ImGui::Combo(label, &selecteditem, options, itemsCount);
	if (check)
	{
		s1 = options[selecteditem];
	}
}


struct configParam {
	std::string& ref;
	const char* label;
};

void renderConfigParams(vector<configParam> params, int baseId){

	for (int i = 0; i < params.size(); i++) {
		configParameter(params[i].ref, baseId + i, params[i].label);
	}
}

void screenshotSettings(string& s1, string& s2, string& s3, string& s4) {

	renderConfigParams({

		{s1,"screenShotFilePath"},
		{s2,"coordinatesOfScreenShotCenter"},
		{s3,"screenShotWidth"},
		{s4,"screenShotHeight"}
		
		}, 4);
	


}

void windowSizesSettings(string& s1, string& s2) {

	renderConfigParams(
		{ 

			{s1,"sfmlSize"},
			{s2,"imguiSize"}

		}, 8);
	
}

void keybindingsSettings(string& s1, string& s2, string& s3, string& s4, string& s5, string& s6,string& s7) {
	
	renderConfigParams({

		{s1,"keyBind_ReadItemsFromScreen"},
		{s2,"keyBind_EscapeProgram"},
		{s3,"keyBind_ReadPreviousItems"},
		{s4,"keyBind_WindowVisibility"},
		{s5,"keyBind_BackupConfig"},
		{s6,"keyBind_ExampleItems"},
		{s7,"keyBind_ReadRelicTitle"}
	
		}, 10);

}

void fontsSettings(string& s1, string& s2) {

	renderConfigParams({ 

		{s1,"fontFile"},
		{s2,"fontSize"}
		
		}, 17);

}

void itemDatabaseSettings(string& s1) {
	static const char* options[]{ "Once per day","Always","Never" };
	comboParameter(s1,"updating type",options,IM_ARRAYSIZE(options));



}


struct SettingsSection {
	string title;
	string description;
	std::function<void()> render;
};


void handleConfigChanges(ToolConfig& newConfig, AppState& state) {

	if (newConfig == state.config) {
		std::cout << "You haven't changed anything\n";
	}
	else {
		std::cout << "New configuration detected\n";
		std::vector<string> differences = state.config.getDifferenceList(newConfig);



		state.config = newConfig;
		rewriteConfigFile(state.config);

		if (keyBindsChanged(differences)) {
			reRegisterHotkeys(state.config);
		}
		if (windowSizesChanged(differences)) {
			WindowParameters parameterssfml = getWindowSize("sfml", state.config);
			reSizeSfmlWindow(state.window, parameterssfml);
			WindowParameters parametersimgui = getWindowSize("imgui", state.config);

			state.sfmlSize = parameterssfml;
			state.imguiSize = parametersimgui;
			state.shouldReSizeImGui = true;
		}
		if (fontsChanged(differences)) {
			state.shouldUpdateFonts = true;
		}


	}


}

void renderLeftPanel(vector<SettingsSection>& sections,int& selected) {

	
		ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border);
		for (int i = 0; i < sections.size(); i++)
		{
			char* label = sections[i].title.data();

			sprintf(label, label);
			if (ImGui::Selectable(label, selected == i))
				selected = i;
		}
		ImGui::EndChild();
	


}

void renderRightPanel(vector<SettingsSection>& sections, int& selected) {
	
	ImGui::BeginGroup();
	ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
	ImGui::Text(sections[selected].title.data());
	ImGui::Separator();
	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Description"))
		{

			ImGui::TextWrapped(sections[selected].description.data());

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Details"))
		{

			sections[selected].render();

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::EndChild();
}


void showSettingsMenu(bool* p_open,AppState& state)
{
	static vector<SettingsSection> sections;
	static ToolConfig newConfig = state.config;

	#define INITREVERTVAR(k) static const string k##ForRevert = newConfig[#k]

	INITREVERTVAR(screenShotFilePath);
	INITREVERTVAR(coordinatesOfScreenShotCenter);
	INITREVERTVAR(screenShotWidth);
	INITREVERTVAR(screenShotHeight);

	INITREVERTVAR(sfmlSize);
	INITREVERTVAR(imguiSize);

	INITREVERTVAR(keyBind_ReadItemsFromScreen);
	INITREVERTVAR(keyBind_EscapeProgram);
	INITREVERTVAR(keyBind_ReadPreviousItems);
	INITREVERTVAR(keyBind_WindowVisibility);
	INITREVERTVAR(keyBind_BackupConfig);
	INITREVERTVAR(keyBind_ExampleItems);
	INITREVERTVAR(keyBind_ReadRelicTitle);

	INITREVERTVAR(fontFile);
	INITREVERTVAR(fontSize);

	INITREVERTVAR(updatingType);




	#define INITCONFIGVAR(k) static string k=newConfig[#k]
	
	
	INITCONFIGVAR(screenShotFilePath);
	INITCONFIGVAR(coordinatesOfScreenShotCenter);
	INITCONFIGVAR(screenShotWidth);
	INITCONFIGVAR(screenShotHeight);

	INITCONFIGVAR(sfmlSize);
	INITCONFIGVAR(imguiSize);

	INITCONFIGVAR(keyBind_ReadItemsFromScreen);
	INITCONFIGVAR(keyBind_EscapeProgram);
	INITCONFIGVAR(keyBind_ReadPreviousItems);
	INITCONFIGVAR(keyBind_WindowVisibility);
	INITCONFIGVAR(keyBind_BackupConfig);
	INITCONFIGVAR(keyBind_ExampleItems);
	INITCONFIGVAR(keyBind_ReadRelicTitle);

	INITCONFIGVAR(fontFile);
	INITCONFIGVAR(fontSize);

	INITCONFIGVAR(updatingType);
	

	sections = {
	{"Screenshot parameters"
	,"Here u give the center of the screenshot, how wide and tall it should be.You also include the path where u want to store the screenshot the app will be taking",	
		[]() {screenshotSettings(screenShotFilePath, coordinatesOfScreenShotCenter, screenShotWidth, screenShotHeight); } },

	{"Window sizes",
		"Here u give how big the window size should be. Both the sfml(the one more to the back), and imgui (the one on top)."
		,[]() {windowSizesSettings(sfmlSize, imguiSize); } },

		{"Keybindings",
		"Keybindings, every keybind is treated as 'Alt + <keybind>'\nIt's important to remember, that keybinds should be 1 character in length, except esc.",
		[]() {keybindingsSettings(keyBind_ReadItemsFromScreen, keyBind_EscapeProgram, keyBind_ReadPreviousItems, keyBind_WindowVisibility, keyBind_BackupConfig, keyBind_ExampleItems,keyBind_ReadRelicTitle); } },

		{"Fonts",
		"All about fonts, fontFile is your path to the .tff file in fonts folder. \nfontSize is the size of the font u want to have",
		[]() {fontsSettings(fontFile, fontSize); } },
		
		{"Item Database",
		"Settings related to how the app should fetch items droppable from relics (and relics themself) from the internet."
		,[]() {itemDatabaseSettings(updatingType); } }
	};


	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Settings", p_open, ImGuiWindowFlags_MenuBar))
	{
		// Left
		static int selected = 0;
		renderLeftPanel(sections,selected);

		ImGui::SameLine();

		// Right
		{
			renderRightPanel(sections,selected);
			
			if (ImGui::Button("Revert")) {

				#define REVERT(k) k = k##ForRevert;

				REVERT(screenShotFilePath)
				REVERT(coordinatesOfScreenShotCenter)
				REVERT(screenShotWidth)
				REVERT(screenShotHeight)
				REVERT(sfmlSize)
				REVERT(imguiSize)
				REVERT(keyBind_ReadItemsFromScreen)
				REVERT(keyBind_EscapeProgram)
				REVERT(keyBind_ReadPreviousItems)
				REVERT(keyBind_WindowVisibility)
				REVERT(keyBind_BackupConfig)
				REVERT(keyBind_ExampleItems)
				REVERT(keyBind_ReadRelicTitle)
				REVERT(fontFile)
				REVERT(fontSize)
				REVERT(updatingType)
				
			}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {
			
				#define SAVE(k) newConfig.setPropertyValue(#k, k);

				SAVE(screenShotFilePath);
				SAVE(coordinatesOfScreenShotCenter);
				SAVE(screenShotWidth);
				SAVE(screenShotHeight);
				SAVE(sfmlSize);
				SAVE(imguiSize);
				SAVE(keyBind_ReadItemsFromScreen);
				SAVE(keyBind_EscapeProgram);
				SAVE(keyBind_ReadPreviousItems);
				SAVE(keyBind_WindowVisibility);
				SAVE(keyBind_BackupConfig);
				SAVE(keyBind_ExampleItems);
				SAVE(keyBind_ReadRelicTitle);
				SAVE(fontFile);
				SAVE(fontSize);
				SAVE(updatingType);

				handleConfigChanges(newConfig, state);

			}
			ImGui::EndGroup();
		}
	}
	ImGui::End();
}
