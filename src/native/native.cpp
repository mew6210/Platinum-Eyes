#include "native.hpp"


/*
	things that are native:
	-taking a screenshot
	-registering and unregistering keybinds
	-handling keybinds events
	-customizing windows in a specific way
	
*/


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)


std::string VirtualKeyCodeToString(UCHAR virtualKey)
{
	UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

	CHAR szName[128];
	int result = 0;
	switch (virtualKey)
	{
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
	case VK_RCONTROL: case VK_RMENU:
	case VK_LWIN: case VK_RWIN: case VK_APPS:
	case VK_PRIOR: case VK_NEXT:
	case VK_END: case VK_HOME:
	case VK_INSERT: case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		scanCode |= KF_EXTENDED;
	default:
		result = GetKeyNameTextA(scanCode << 16, szName, 128);
	}
	if (result == 0)
		throw std::system_error(std::error_code(GetLastError(), std::system_category()),
			"WinAPI Error occured.");
	return szName;
}


int StringToVirtualKeyCode(std::string s) {

	for (auto& c : s) {
		c = std::tolower(c);
	}

	if (s == "esc") {
		return VK_ESCAPE;
	}

	HKL keyboard = LoadKeyboardLayoutA("00000409", KLF_ACTIVATE);        //primary english US keyboard layout

	return VkKeyScanEx(s[0], keyboard);
}


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
			successLog("Successfully registered hotkey: Alt + ",VirtualKeyCodeToString(p.second.getKey())," for: ",p.second.getDescription());
		}
		else {
			warningLog("Failed to register hotkey: Alt + ",VirtualKeyCodeToString(p.second.getKey()) + " for: ",p.second.getDescription());

		}

	}



}

void unregisterNativeHotkeys(std::map<int, KeyBind>& keyBindings) {


	for (auto& p : keyBindings) {
		if (UnregisterHotKey(NULL, p.first)) {
			successLog("Succesfully unregistered Alt + " ,VirtualKeyCodeToString(p.second.getKey()));
		}
		else {
			errorLog(false,"Failed to unregister Alt + " ,VirtualKeyCodeToString(p.second.getKey()));
		}
	}



}


int getNativeScreenResolution() {


	HDC hScreen = GetDCEx(NULL, NULL, DCX_NORESETATTRS);
	int width = GetDeviceCaps(hScreen, HORZRES);
	int height = GetDeviceCaps(hScreen, VERTRES);
	DeleteDC(hScreen);
	int screenResolution = getScreenResolution(width, height);
	return screenResolution;

}


#endif




#if __linux__



void listenAndHandleEvents(){
	errorLog(true,"currently not implemented");

}
void nativeWindowCustomization() {
	errorLog(true,"currently not implemented");

}
void registerNativeHotkeys() {
	errorLog(true,"currently not implemented");

}
void unregisterNativeHotkeys() {
	errorLog(true,"currently not implemented");

}












#endif