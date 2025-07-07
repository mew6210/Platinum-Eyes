#include "native.hpp"


/*
	things that are native:
	-taking a screenshot
	-registering and unregistering keybinds
	-handling keybinds events
	-customizing windows in a specific way
	
*/


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

void listenAndHandleEvents(AppState& state) {


	if (PeekMessage(&state.msg, NULL, 0, 0, PM_REMOVE)) {

		handleEvents(state);
		TranslateMessage(&state.msg);
		DispatchMessage(&state.msg);
	}

}

void nativeWindowCustomization(AppState& state,sf::WindowHandle wHandle) {

	HWND hwnd = wHandle;
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	style |= WS_EX_TOOLWINDOW;
	SetWindowLongPtr(hwnd, GWL_EXSTYLE, style);
	setTransparency(hwnd);



}


void registerNativeHotkeys(std::map<int,KeyBind>& keyBindings) {

	for (auto& p : keyBindings) {

		if (RegisterHotKey(
			NULL,
			p.first,
			MOD_ALT | MOD_NOREPEAT,
			p.second.getKey()))
		{
			successLog("Successfully registered hotkey: Alt + " + VirtualKeyCodeToString(p.second.getKey()) + " for: " + p.second.getDescription());
		}
		else {
			warningLog("Failed to register hotkey: Alt + " + VirtualKeyCodeToString(p.second.getKey()) + " for: " + p.second.getDescription());

		}

	}



}

void unregisterNativeHotkeys(std::map<int, KeyBind>& keyBindings) {


	for (auto& p : keyBindings) {
		if (UnregisterHotKey(NULL, p.first)) {
			successLog("Succesfully unregistered Alt + " + VirtualKeyCodeToString(p.second.getKey()));
		}
		else {
			errorLog("Failed to unregister Alt + " + VirtualKeyCodeToString(p.second.getKey()), false);
		}
	}



}


#endif




#if __linux__



void listenAndHandleEvents(){
	errorLog("currently not implemented",true);

}
void nativeWindowCustomization(){
	errorLog("currently not implemented",true);

}
void registerNativeHotkeys(){
	errorLog("currently not implemented",true);

}
void unregisterNativeHotkeys() {
	errorLog("currently not implemented", true);

}












#endif