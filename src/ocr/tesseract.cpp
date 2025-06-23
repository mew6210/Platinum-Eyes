//#include "../core.h"
#include "ocr.h"
#include <sstream>

using std::vector, std::string, std::pair;


vector<string> itemsSmallerThan2 = { "bo" };      //all the items that are smaller in name size than 2, should probably do it programatically somehow, but ill do it later TODO


string filterResults(const string& script, const char* charMap, size_t mapSize) {
    std::unordered_set<char> allowedChars(charMap, charMap + mapSize);
    string filteredScript;

    for (char c : script) {
        char upperC = std::toupper(c);  // Convert to uppercase
        if (allowedChars.find(upperC) != allowedChars.end()) {
            filteredScript += c;  // Keep the original case
        }
    }

    return filteredScript;
}

string removeShortWords(string& item) {

    std::stringstream ss(item);

    string word;

    vector<string> words;

    while (getline(ss, word, ' ')) {
        words.push_back(word);
    }



    string returnItem;


    for (auto& word : words) {


        if (word.size() >2) {
            returnItem.append(word);
            returnItem.append(" ");
        }
        else if (std::find(itemsSmallerThan2.begin(), itemsSmallerThan2.end(), word) != itemsSmallerThan2.end()) {              //check if its an item with length smaller than 2, or just some rubbish from tesseract
            returnItem.append(word);
            returnItem.append(" ");
        }
    }
   


    return returnItem;

}



vector<string> readFissureItems(tesseract::TessBaseAPI& api,size_t itemCount,const string& fileName) {
    Timer timer = Timer();
    cv::Mat img = cv::imread(fileName);
    if (img.empty()) {
        errorLog("Failed to load image.",true);
        
    }


        
    switch (itemCount) {
    case 4:
    {
        timer.start();
        cv::Mat first_item
            = img(cv::Range(30, img.rows), cv::Range(0, img.cols / 4 - 1));

        cv::Mat second_item
            = img(cv::Range(30, img.rows), cv::Range(img.cols / 2 - img.cols / 4, img.cols / 2 - 1));

        cv::Mat third_item
            = img(cv::Range(30, img.rows), cv::Range(img.cols / 2, img.cols / 2 + img.cols / 4 - 1));

        cv::Mat fourth_item
            = img(cv::Range(30, img.rows), cv::Range((img.cols / 2 + img.cols / 4), img.cols - 1));

        std::vector<string> items;


        items.push_back(readItemTesseract(first_item, api,false));
        items.push_back(readItemTesseract(second_item, api,false));
        items.push_back(readItemTesseract(third_item, api,false));
        items.push_back(readItemTesseract(fourth_item, api,false));
        timer.stop();
        timer.print("performing tesseract ocr for 4 images");

        return items;
        break;

    }
    case 3: {
        timer.start();
        int space = (img.cols / 4) / 2;
        cv::Mat first_item
            = img(cv::Range(30, img.rows), cv::Range(space, (img.cols / 4 - 1)+space));

        cv::Mat second_item
            = img(cv::Range(30, img.rows), cv::Range((img.cols / 2 - img.cols / 4), (img.cols / 2 - 1))+space);

        cv::Mat third_item
            = img(cv::Range(30, img.rows), cv::Range((img.cols / 2), (img.cols / 2 + img.cols / 4 - 1))+space);
        
        std::vector<string> items;


        items.push_back(readItemTesseract(first_item, api,false));
        items.push_back(readItemTesseract(second_item, api,false));
        items.push_back(readItemTesseract(third_item, api,false));
        timer.stop();
        timer.print("performing tesseract ocr for 3 images");

        return items;





        
        } ;
    case 2: { 
        timer.start();
        cv::Mat first_item
            = img(cv::Range(30, img.rows), cv::Range(0, img.cols / 2));

        cv::Mat second_item
            = img(cv::Range(30, img.rows), cv::Range(img.cols / 2, img.cols));

        std::vector<string> items;


        items.push_back(readItemTesseract(first_item, api, false));
        items.push_back(readItemTesseract(second_item, api, false));

        timer.stop();
        timer.print("performing tesseract ocr for 2 images");

        return items;

    
    };
    case 1:  
    {

        cv::Mat first_item
            = img(cv::Range(30, img.rows), cv::Range(0, img.cols));
        std::vector<string> items;

        items.push_back(readItemTesseract(first_item, api, false));
        return items;
    }



    case 0:
    {


        vector<string> items;
        return items;

    }
    }
    

  



}




//TODO: THESE 3 FUNCTIONS DO EXACTLY THE SAME THING BUT WITH DIFFERENT FILTER MASKS
string readRelicTesseract(tesseract::TessBaseAPI& api, const char* path,bool showImage) {
    cv::Mat image = cv::imread(path);

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    //cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    //cv::Canny(image, image, 100, 200);




    static string name = "aaa";
    api.SetImage((uchar*)image.data, image.size().width, image.size().height, image.channels(), image.step1());
    string text = api.GetUTF8Text();

    const char charMap[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ','\n',         // Uppercase letters
    '0','1','2','3','4','5','6','7','8','9'
    
    
    };
    size_t mapSize = sizeof(charMap) / sizeof(charMap[0]);

    string result = filterResults(text, charMap, mapSize);
    if (showImage)cv::imshow(name, image);
    name.append("a");
    //result = replaceChar(result, '\n', " ");
    //trim(result);

    //result = removeShortWords(result);
    //trim(result);


    std::cout << result << "\n";

    return result;


}


string readRelicTitleTesseract(tesseract::TessBaseAPI& api, const char* path, bool showImage) {
    cv::Mat image = cv::imread(path);

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::dilate(image, image, kernel, cv::Point(-1, -1), 1);
    cv::erode(image, image, kernel, cv::Point(-1, -1), 1);


    cv::medianBlur(image, image, 3);
    cv::bitwise_not(image, image);
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);


    static string name = "aaa";
    api.SetImage((uchar*)image.data, image.size().width, image.size().height, image.channels(), image.step1());
    string text = api.GetUTF8Text();

    const char charMap[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ',         // Uppercase letters
    '0','1','2','3','4','5','6','7','8','9'


    };
    size_t mapSize = sizeof(charMap) / sizeof(charMap[0]);

    string result = filterResults(text, charMap, mapSize);
    if (showImage)cv::imshow(name, image);
    name.append("a");


    if (result != "")
        std::cout << "Reading result: " << result << "\n";
    else  errorLog("No text found in its designated area",false);


    return result;


}




string readItemTesseract(cv::Mat& image, tesseract::TessBaseAPI& api,bool showImage) {


    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    //cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    //cv::adaptiveThreshold(image, image, 255, cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
    


    //dialate,erode
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::dilate(image, image, kernel, cv::Point(-1, -1), 1);
    cv::erode(image, image, kernel, cv::Point(-1, -1), 1);

    //median blur
    cv::medianBlur(image, image, 3);
    cv::bitwise_not(image, image);
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    //cv::Canny(image, image, 100, 300);




    static string name = "aaa";
    api.SetImage((uchar*)image.data, image.size().width, image.size().height, image.channels(), image.step1());
    string text = api.GetUTF8Text();

    const char charMap[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ','\n'  // Uppercase letters
    };
    size_t mapSize = sizeof(charMap) / sizeof(charMap[0]);

    string result = filterResults(text, charMap, mapSize);
    if(showImage)cv::imshow(name,image);
    name.append("a");
    result = replaceChar(result, '\n', " ");
    trim(result);
    
    result = removeShortWords(result);
    trim(result);


    std::cout <<"Reading result: " << result << "\n";

    return result;


}


std::tuple<int, int, int> calculatePositionAndWidth(int width, int height) {
    // Aspect ratio
    double aspectRatio = static_cast<double>(width) / height;

    // Linear interpolation constants for x_ratio
    double m_x = 0.1541;
    double c_x = 0.356;
    double x_ratio = m_x * aspectRatio + c_x;

    // y_ratio is approximately constant
    double y_ratio = 0.255;

    // Calculate x and y positions
    int x_new = static_cast<int>(std::round(width * x_ratio));
    int y_new = static_cast<int>(std::round(height * y_ratio));

    // Linear interpolation constants for screenshot width ratio
    double m_width = -0.0759;
    double c_width = 0.4234;
    double screenshot_width_ratio = m_width * aspectRatio + c_width;

    // Calculate screenshot width
    int screenshot_width = static_cast<int>(std::round(width * screenshot_width_ratio));

    return { x_new, y_new, screenshot_width };
}

/*
 - takes a screenshot and saves it to the fileName
*/
void takeScreenshotAndSaveToFile(const int width, const int height,const Point p,string& fileName) {
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
void takeScreenshotAndSaveToFile(const int width, const int height, const int px, const int py, string& fileName) {
    Timer timer = Timer();

    timer.start();
    HBITMAP bitmap = takeScreenshot(width, height, px,py);
    timer.stop();
    timer.print("take screenshot");




    SaveHBITMAPToFile(bitmap, fileName.c_str());
    DeleteObject(bitmap);

}




//your mom
RelicInfo readItemsFromRelicTitleTesseract(tesseract::TessBaseAPI& api) {

    Timer timer;


    int px, py;
    HDC hScreen = GetDCEx(NULL, NULL, DCX_NORESETATTRS);
    int width = GetDeviceCaps(hScreen, HORZRES);
    int height = GetDeviceCaps(hScreen, VERTRES);

    auto [coordinatex, coordinatey, titleWidth] = calculatePositionAndWidth(width, height);

    
    px = coordinatex;
    py = coordinatey;

    string fileName = "relicTitleScreenshot.bmp";
    takeScreenshotAndSaveToFile(titleWidth,40,px-5,py-5,fileName);



    string relicRead = readRelicTitleTesseract(api, fileName.c_str(), false);
    string relicParsed = relicMenuTitleStringToRelicString(relicRead);
    RelicInfo relic = FetchRelicItemPrices(relicParsed);

    return relic;


}


int calculateShiftWidth(int width, int height) {
    // Aspect ratio
    double aspectRatio = static_cast<double>(width) / height;

    // Linear interpolation constants for shift width ratio
    double m_shift = -0.0616;
    double c_shift = 0.256;
    double shift_width_ratio = m_shift * aspectRatio + c_shift;

    // Calculate and return the shift width
    return static_cast<int>(std::round(width * shift_width_ratio));
}


RelicInfo readItemsFromRelicTitleTesseractShifted(tesseract::TessBaseAPI& api) {


    Timer timer;

    size_t itemCount = 4;

    int px, py;
    HDC hScreen = GetDCEx(NULL, NULL, DCX_NORESETATTRS);
    int width = GetDeviceCaps(hScreen, HORZRES);
    int height = GetDeviceCaps(hScreen, VERTRES);

    auto [coordinatex, coordinatey, titleWidth] = calculatePositionAndWidth(width, height);
    int x_shift = calculateShiftWidth(width,height);



    px = coordinatex;
    py = coordinatey;

    //std::cout << "position: " << px << "," << py << " width: " << titleWidth;
    //std::cout << "shift: " << x_shift;
    //std::cout << "new position: " << (px + x_shift) - 5;
    
    string fileName = "relicTitleScreenshot.bmp";
    takeScreenshotAndSaveToFile(titleWidth - titleWidth / 5,40,(px-x_shift)-5,py-5,fileName);
   

    string relicRead = readRelicTitleTesseract(api, fileName.c_str(), false);
    string relicParsed = relicMenuTitleStringToRelicString(relicRead);
    RelicInfo relic = FetchRelicItemPrices(relicParsed);

    return relic;



}





bool arePricesEmpty(std::map<string, ItemDetails>& itemPrices) {

    for (auto& item : itemPrices) {
        if (item.second.averagePrice != 0) {
            return false;
        }
    }
    return true;



}

vector<Item> screenshotToItems(AppState& state,const string& fileName) {

    Timer timer = Timer();

    size_t itemCount = 4;

    vector<string> readResults = readFissureItems(state.tesseractApi, itemCount, fileName);
    vector<string> preparedItems = prepareItems(readResults);
 
  
    while (checkIfItemsAreValid(preparedItems, state.allAvalibleItems) == 0) {
        
        if (itemCount == 0) break;
        
        itemCount--;
        readResults = readFissureItems(state.tesseractApi, itemCount, fileName);
        preparedItems = prepareItems(readResults);
    }

    if (itemCount == 0) {
        errorLog("Could not find fissure rewards items on screen", false);
        return {};
    }

    fixItems(preparedItems, state.allAvalibleItems);

    preparedItems = prepareItems(preparedItems);

    timer.start();
    vector<Item> itemPrices = getItemPricesMap(preparedItems);
    timer.stop("Fetching item prices from warframe market");




    itemPrices = prepareItemsForRead(itemPrices);
    printItemPrices(itemPrices);

    saveItemsToClipboard(itemPrices);

    return itemPrices;
}


std::vector<Item> readFissureRewardsScreen(AppState state) {


    std::pair<int, int > coordinates = stringToIntPair(state.config["coordinatesOfScreenShotCenter"]);
    Point p;
    p.x = coordinates.first;
    p.y = coordinates.second;

   
    string fileName = state.config["screenShotFilePath"];
    takeScreenshotAndSaveToFile(stoi(state.config["screenShotWidth"]), stoi(state.config["screenShotHeight"]),p,fileName);


    auto itemPrices = screenshotToItems(state, fileName);


    return itemPrices;
}





std::vector<Item> readPreviousFissureRewardsScreen(AppState state) {

    string fileName = state.config["screenShotFilePath"];
    auto itemPrices = screenshotToItems(state,fileName);

    return itemPrices;
}



void tesseractInit(tesseract::TessBaseAPI& api) {

    
        if (api.Init(nullptr, "eng_fast")) {
            errorLog("Could not initialize tesseract",false);
            exit(0);
        }
        else {
            successLog("Successfully initialized tesseract");
        }
        
    }
   
    

