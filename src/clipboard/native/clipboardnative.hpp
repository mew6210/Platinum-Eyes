#include "../../core/core.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
void saveScreenshotToClipboard(HBITMAP bitmap);
HGLOBAL loadStringToMemory(std::string& text);
void saveItemsToClipboard(AppState& state, std::vector<Item>& items);
#endif


#if __linux__

void saveScreenshotToClipboard(HBITMAP bitmap);
void saveItemsToClipboard(AppState& state, std::vector<Item>& items);
#endif