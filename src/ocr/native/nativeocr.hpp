#include <string>
#include "../../core/core.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

void takeNativeScreenshotAndSaveToFile(const int width, const int height, const Point p, std::string& fileName);
void takeNativeScreenshotAndSaveToFile(const int width, const int height, const int px, const int py, std::string& fileName);
HBITMAP takeScreenshot(int imageWidth, int imageHeight, int offsetX, int offsetY);
HBITMAP takeScreenshot(int imageWidth, int imageHeight, Point);
void SaveHBITMAPToFile();


#endif


#if __linux__

void takeNativeScreenshotAndSaveToFile(const int width, const int height, const Point p, std::string& fileName);
void takeNativeScreenshotAndSaveToFile(const int width, const int height, const int px, const int py, std::string& fileName);
void takeScreenshot(int imageWidth, int imageHeight, int offsetX, int offsetY);
void takeScreenshot(int imageWidth, int imageHeight, Point);



#endif