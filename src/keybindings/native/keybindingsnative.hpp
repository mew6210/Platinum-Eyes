#include <string>
#include "../../core/core.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
void initializeKeyBindingsMap(std::map<int, KeyBind>& keyBindings, ToolConfig& config);
void registerNativeHotkeys(std::map<int, KeyBind>& keyBindings);
void unregisterNativeHotkeys(std::map<int, KeyBind>& keyBindings);
std::string VirtualKeyCodeToString(UCHAR virtualKey);


#endif


#if __linux__
void initializeKeyBindingsMap(std::map<int, KeyBind>& keyBindings, ToolConfig& config);
void registerNativeHotkeys(std::map<int, KeyBind>& keyBindings);
void unregisterNativeHotkeys(std::map<int, KeyBind>& keyBindings);


#endif