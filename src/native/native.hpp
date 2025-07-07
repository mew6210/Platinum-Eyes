#include "../core/core.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#include <dwmapi.h>

void listenAndHandleEvents(AppState& state);
void nativeWindowCustomization(AppState& state, sf::WindowHandle wHandle);
void registerNativeHotkeys(std::map<int, KeyBind>& keyBindings);
void unregisterNativeHotkeys(std::map<int, KeyBind>& keyBindings);
#endif

#if __linux__
#include <X11/Xlib.h>


void listenAndHandleEvents();
void nativeWindowCustomization();
void registerNativeHotkeys();
void unregisterNativeHotkeys();


#endif