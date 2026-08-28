#include "gui.h"


ImGuiWindowFlags getImGuiWindowFlags() {
	return
		ImGuiWindowFlags_None
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_HorizontalScrollbar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void resizeImGuiWindow(GraphicLayer& gui) {

	auto heightDiff = gui.sfmlSize.height - gui.imguiSize.height;
	auto widthDiff = gui.sfmlSize.width - gui.imguiSize.width;

	ImGui::SetNextWindowSize(
		ImVec2(
			static_cast<float>(gui.imguiSize.width),
			static_cast<float>(gui.imguiSize.height)),
		ImGuiCond_Always
	);

	ImGui::SetNextWindowPos(
		ImVec2(
			static_cast<float>(widthDiff / 2),
			static_cast<float>(heightDiff / 2)),
		ImGuiCond_Always
	);

	gui.shouldResizeImGui = !gui.shouldResizeImGui;
}

void createMainWindow(bool& isRunning, AppState& state) {		//only gui???

	auto flags = getImGuiWindowFlags();
	ImGui::SetNextWindowBgAlpha(0.7f);

	if (state.gui.shouldResizeImGui) {
		resizeImGuiWindow(state.gui);
	}

	ImGui::Begin("Warframe tool-main", &isRunning, flags);

	if (ImGui::SmallButton("Config")) {
		state.gui.settingsVisible = !state.gui.settingsVisible; //flip the settingsOpen
	}
	if (state.gui.settingsVisible == true) {
		showSettingsWindow(&state.gui.settingsVisible, state);
	}
}