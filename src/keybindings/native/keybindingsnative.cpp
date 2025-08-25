#include "keybindingsnative.hpp"


#define KB_ReadItemsFromScreen 1
#define KB_EscapeProgram 2
#define KB_ReadPreviousItems 3
#define KB_WindowVisibility 4
#define KB_BackupConfig 5
#define KB_ExampleItems 6
#define KB_ReadRelicTitle 7

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)



void initializeKeyBindingsMap(std::map<int, KeyBind>& keyBindings, ToolConfig& config) {


    keyBindings = {
    { KB_ReadItemsFromScreen, KeyBind(StringToVirtualKeyCode(config["keyBind_ReadItemsFromScreen"]), "Read items from current screen") },
    { KB_EscapeProgram,KeyBind(StringToVirtualKeyCode(config["keyBind_EscapeProgram"]),"Escape program") },
    { KB_ReadPreviousItems,KeyBind(StringToVirtualKeyCode(config["keyBind_ReadPreviousItems"]),"Read previously shown items") },
    { KB_WindowVisibility,KeyBind(StringToVirtualKeyCode(config["keyBind_WindowVisibility"]),"Toggle window visibility") },
    { KB_BackupConfig,KeyBind(StringToVirtualKeyCode(config["keyBind_BackupConfig"]),"Save current config to copy file") },
    { KB_ExampleItems,KeyBind(StringToVirtualKeyCode(config["keyBind_ExampleItems"]),"Show example items") },
    { KB_ReadRelicTitle,KeyBind(StringToVirtualKeyCode(config["keyBind_ReadRelicTitle"]),"Relic Title Screenshot")}
    };


}

void registerNativeHotkeys(std::map<int, KeyBind>& keyBindings) {

	for (auto& p : keyBindings) {

		if (RegisterHotKey(
			NULL,
			p.first,
			MOD_ALT | MOD_NOREPEAT,
			p.second.getKey()))
		{
			successLog("Successfully registered hotkey: Alt + ", VirtualKeyCodeToString(p.second.getKey()), " - ", p.second.getDescription());
		}
		else {
			warningLog("Failed to register hotkey: Alt + ", VirtualKeyCodeToString(p.second.getKey()) + " - ", p.second.getDescription());

		}

	}



}

void unregisterNativeHotkeys(std::map<int, KeyBind>& keyBindings) {


	for (auto& p : keyBindings) {
		if (UnregisterHotKey(NULL, p.first)) {
			successLog("Succesfully unregistered Alt + ", VirtualKeyCodeToString(p.second.getKey()));
		}
		else {
			errorLog(false, "Failed to unregister Alt + ", VirtualKeyCodeToString(p.second.getKey()));
		}
	}



}


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



#endif


#if __linux__
void initializeKeyBindingsMap(std::map<int, KeyBind>& keyBindings, ToolConfig& config) {
    errorLog(true, "not implemented yet");
}

void registerNativeHotkeys(std::map<int, KeyBind>& keyBindings) {
	errorLog(true, "currently not implemented");

}
void unregisterNativeHotkeys(std::map<int, KeyBind>& keyBindings) {
	errorLog(true, "currently not implemented");

}


#endif