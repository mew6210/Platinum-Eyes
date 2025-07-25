#include "../core/core.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)


void listenAndHandleEvents(AppState& state);
void nativeWindowCustomization(AppState& state, sf::WindowHandle wHandle);
void registerNativeHotkeys(std::map<int, KeyBind>& keyBindings);
void unregisterNativeHotkeys(std::map<int, KeyBind>& keyBindings);
int getNativeScreenResolution();
std::string VirtualKeyCodeToString(UCHAR virtualKey);

HBITMAP takeScreenshot(int imageWidth, int imageHeight, int offsetX, int offsetY);
HBITMAP takeScreenshot(int imageWidth, int imageHeight, Point);
BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
void saveScreenshotToClipboard(HBITMAP bitmap);
HGLOBAL loadStringToMemory(std::string& text);
void saveItemsToClipboard(AppState& state, std::vector<Item>& items);



#endif

#if __linux__


void listenAndHandleEvents();
void nativeWindowCustomization();
void registerNativeHotkeys();
void unregisterNativeHotkeys();



void takeScreenshot();
void takeScreenshot();
void SaveHBITMAPToFile();
void saveScreenshotToClipboard();
void loadStringToMemory();
void saveItemsToClipboard();


#endif