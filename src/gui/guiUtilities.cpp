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

void customizeMainWindow(GraphicLayer& gui) {
	sf::WindowHandle wHandle = gui.window->getNativeHandle();
	gui.window->setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().size.x - (gui.sfmlSize.width+25), 25));
	gui.window->setFramerateLimit(gui.fpsVisible);
	
	if (!ImGui::SFML::Init(*gui.window)) {
		errorLog(true,"ImGui::SFML::Init() failed");
	}

	nativeWindowCustomization(wHandle);
}

void setNewFont(ToolConfig& config) {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	std::string filename = "assets/fonts/" + config["fontFile"];
	float size_pixels = std::stof(config["fontSize"]);
	io.Fonts->AddFontFromFileTTF(filename.c_str(), size_pixels);

	if (!ImGui::SFML::UpdateFontTexture()) {
		warningLog("failed to update font texture");
	}
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
	
	
	if (!ImGui::SFML::UpdateFontTexture()) {
		warningLog("failed to update font texture");
	}
}

void handleBetweenFrameImGuiUpdates(AppState& state) {	//only gui and config
	if (state.gui.shouldUpdateFonts) {
		setNewFont(state.config);
		state.gui.shouldUpdateFonts = false;
	}
}

void updateFps(ToolConfig& config,GraphicLayer& gui) {		//only gui and config

	int newVisibleFps = gui.fpsVisible;
	int newHiddenFps = gui.fpsHidden;
	
	try {
		newVisibleFps = stoi(config["fpsVisible"]);
	}
	catch (const std::exception& e) {
		warningLog("visible fps inputted wrong, ",e.what()," defaulting to 30");
		newVisibleFps = 30;
	}

	try {
		newHiddenFps = stoi(config["fpsHidden"]);
	}
	catch (const std::exception& e) {
		warningLog("hidden fps inputted wrong, ", e.what(), " defaulting to 30");
		newHiddenFps = 30;
	}
	
	if (gui.isVisible) {
		gui.window->setFramerateLimit(newVisibleFps);
	}
	else {
		gui.window->setFramerateLimit(newHiddenFps);
	}
	gui.fpsVisible = newVisibleFps;
	gui.fpsHidden = newHiddenFps;
}