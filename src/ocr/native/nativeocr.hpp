#include <string>
#include "../../core/core.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

void takeNativeScreenshotAndSaveToFile(const int width, const int height, const Point p, std::string& fileName);
void takeNativeScreenshotAndSaveToFile(const int width, const int height, const int px, const int py, std::string& fileName);

#endif


#if __linux__

void takeNativeScreenshotAndSaveToFile(const int width, const int height, const Point p, std::string& fileName);
void takeNativeScreenshotAndSaveToFile(const int width, const int height, const int px, const int py, std::string& fileName);

#endif