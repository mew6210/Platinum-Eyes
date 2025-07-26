#include "nativeocr.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)


/*
 - takes a screenshot and saves it to the fileName
*/
void takeNativeScreenshotAndSaveToFile(const int width, const int height, const Point p, std::string& fileName){
    Timer timer = Timer();

    timer.start();
    HBITMAP bitmap = takeScreenshot(width, height, p);
    timer.stop();
    timer.print("take screenshot");


    SaveHBITMAPToFile(bitmap, fileName.c_str());
    DeleteObject(bitmap);

}

/*
 - takes a screenshot and saves it to the fileName
*/
void takeNativeScreenshotAndSaveToFile(const int width, const int height, const int px, const int py, std::string& fileName){
    Timer timer = Timer();

    timer.start();
    HBITMAP bitmap = takeScreenshot(width, height, px, py);
    timer.stop();
    timer.print("take screenshot");




    SaveHBITMAPToFile(bitmap, fileName.c_str());
    DeleteObject(bitmap);
}


#endif


#if __linux__

void takeNativeScreenshotAndSaveToFile(const int width, const int height, const Point p, std::string& fileName){
    errorLog(true,"not yet implemented");

}
void takeNativeScreenshotAndSaveToFile(const int width, const int height, const int px, const int py, std::string& fileName){
    errorLog(true, "not yet implemented");

}

#endif