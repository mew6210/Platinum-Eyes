#include "../core.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

void listenAndHandleEvents(AppState& state);
void handleNativeEvents(AppState& state, std::map<int, KeyBind> keybindings);

#endif




#if __linux__


void listenAndHandleEvents(AppState& state);
void handleNativeEvents(AppState& state, std::map<int, KeyBind> keybindings);


#endif
