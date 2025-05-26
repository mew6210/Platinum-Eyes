//#include "../platinumEyes.h"
#include "ocr.h"
#include <sstream>


std::vector<std::string> itemsSmallerThan2 = { "bo" };      //all the items that are smaller in name size than 2, should probably do it programatically somehow, but ill do it later TODO


unsigned decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, const std::vector<unsigned char>& bmp) {
    static const unsigned MINHEADER = 54; //minimum BMP header size

    if (bmp.size() < MINHEADER) return -1;
    if (bmp[0] != 'B' || bmp[1] != 'M') return 1; //It's not a BMP file if it doesn't start with marker 'BM'
    unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
    //read width and height from BMP header
    w = bmp[18] + bmp[19] * 256;
    h = bmp[22] + bmp[23] * 256;
    int signedHeight = bmp[22] + (bmp[23] << 8) + (bmp[24] << 16) + (bmp[25] << 24);
    h = (signedHeight < 0) ? -signedHeight : signedHeight;              //without this everything breaks btw
    //read number of channels from BMP header
    if (bmp[28] != 24 && bmp[28] != 32) return 2; //only 24-bit and 32-bit BMPs are supported.
    unsigned numChannels = bmp[28] / 8;

    //The amount of scanline bytes is width of image times channels, with extra bytes added if needed
    //to make it a multiple of 4 bytes.
    unsigned scanlineBytes = w * numChannels;
    if (scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

    unsigned dataSize = scanlineBytes * h;

    if (bmp.size() < dataSize + pixeloffset) return 3; //BMP file too small to contain all pixels

    image.resize(w * h * 4);

    /*
    There are 3 differences between BMP and the raw image buffer for LodePNG:
    -it's upside down
    -it's in BGR instead of RGB format (or BRGA instead of RGBA)
    -each scanline has padding bytes to make it a multiple of 4 if needed
    The 2D for loop below does all these 3 conversions at once.
    */
    for (unsigned y = 0; y < h; y++) {
        for (unsigned x = 0; x < w; x++) {
            // Pixel start byte position in the BMP
            unsigned bmpos = pixeloffset + y * scanlineBytes + numChannels * x;
            // Pixel start byte position in the new raw image
            unsigned newpos = 4 * y * w + 4 * x;

            if (numChannels == 3) {
                image[newpos + 0] = bmp[bmpos + 2]; // R
                image[newpos + 1] = bmp[bmpos + 1]; // G
                image[newpos + 2] = bmp[bmpos + 0]; // B
                image[newpos + 3] = 255;            // A
            }
            else {
                image[newpos + 0] = bmp[bmpos + 2]; // R
                image[newpos + 1] = bmp[bmpos + 1]; // G
                image[newpos + 2] = bmp[bmpos + 0]; // B
                image[newpos + 3] = bmp[bmpos + 3]; // A
            }
        }
    }
    return 0;
}


int convertBMPtoPNG(std::string& path) {

    Timer timer = Timer();
    timer.start();
    std::vector<unsigned char> bmp;

    lodepng::load_file(bmp, path+".bmp");
    std::vector<unsigned char> image;
    unsigned w, h;
    unsigned error = decodeBMP(image, w, h, bmp);

    if (error) {
        errorLog("BMP decoding error " + error);
        return 0;
    }

    std::vector<unsigned char> png;
    error = lodepng::encode(png, image, w, h);

    if (error) {
        errorLog("PNG encoding error "+std::to_string(error)+": " + std::string(lodepng_error_text(error)));
        //std::cout << "PNG encoding error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 0;
    }

    lodepng::save_file(png, path+".png");
    timer.stop();
    timer.print("converting bmp to png");



}

std::string filterResults(const std::string& script, const char* charMap, size_t mapSize) {
    std::unordered_set<char> allowedChars(charMap, charMap + mapSize);
    std::string filteredScript;

    for (char c : script) {
        char upperC = std::toupper(c);  // Convert to uppercase
        if (allowedChars.find(upperC) != allowedChars.end()) {
            filteredScript += c;  // Keep the original case
        }
    }

    return filteredScript;
}

std::string removeShortWords(std::string& item) {

    std::stringstream ss(item);

    std::string word;

    std::vector<std::string> words;

    while (getline(ss, word, ' ')) {
        words.push_back(word);
    }



    std::string returnItem;


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



std::vector<std::string> readFissureItems(tesseract::TessBaseAPI& api,size_t itemCount,std::string& fileName) {
    Timer timer = Timer();

    cv::Mat img = cv::imread(fileName);
    if (img.empty()) {
        std::cerr << "Failed to load image.\n";
        exit(-1);
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

        std::vector<std::string> items;


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
        
        std::vector<std::string> items;


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

        std::vector<std::string> items;


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
        std::vector<std::string> items;

        items.push_back(readItemTesseract(first_item, api, false));
        return items;
    }



    case 0:
    {


        std::vector<std::string> items;
        return items;

    }
    }
    

  



}




//TODO: THESE 3 FUNCTIONS DO EXACTLY THE SAME THING BUT WITH DIFFERENT FILTER MASKS
std::string readRelicTesseract(tesseract::TessBaseAPI& api, const char* path,bool showImage) {
    cv::Mat image = cv::imread(path);

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    //cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    //cv::Canny(image, image, 100, 200);




    static std::string name = "aaa";
    api.SetImage((uchar*)image.data, image.size().width, image.size().height, image.channels(), image.step1());
    std::string text = api.GetUTF8Text();

    const char charMap[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ','\n',         // Uppercase letters
    '0','1','2','3','4','5','6','7','8','9'
    
    
    };
    size_t mapSize = sizeof(charMap) / sizeof(charMap[0]);

    std::string result = filterResults(text, charMap, mapSize);
    if (showImage)cv::imshow(name, image);
    name.append("a");
    //result = replaceChar(result, '\n', " ");
    //trim(result);

    //result = removeShortWords(result);
    //trim(result);


    std::cout << result << "\n";

    return result;


}


std::string readRelicTitleTesseract(tesseract::TessBaseAPI& api, const char* path, bool showImage) {
    cv::Mat image = cv::imread(path);

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::dilate(image, image, kernel, cv::Point(-1, -1), 1);
    cv::erode(image, image, kernel, cv::Point(-1, -1), 1);


    cv::medianBlur(image, image, 3);
    cv::bitwise_not(image, image);
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);


    static std::string name = "aaa";
    api.SetImage((uchar*)image.data, image.size().width, image.size().height, image.channels(), image.step1());
    std::string text = api.GetUTF8Text();

    const char charMap[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ',         // Uppercase letters
    '0','1','2','3','4','5','6','7','8','9'


    };
    size_t mapSize = sizeof(charMap) / sizeof(charMap[0]);

    std::string result = filterResults(text, charMap, mapSize);
    if (showImage)cv::imshow(name, image);
    name.append("a");


    if (result != "")
        std::cout << "Reading result: " << result << "\n";
    else  errorLog("No text found in its designated area");


    return result;


}




std::string readItemTesseract(cv::Mat& image, tesseract::TessBaseAPI& api,bool showImage) {


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




    static std::string name = "aaa";
    api.SetImage((uchar*)image.data, image.size().width, image.size().height, image.channels(), image.step1());
    std::string text = api.GetUTF8Text();

    const char charMap[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ','\n'  // Uppercase letters
    };
    size_t mapSize = sizeof(charMap) / sizeof(charMap[0]);

    std::string result = filterResults(text, charMap, mapSize);
    if(showImage)cv::imshow(name,image);
    name.append("a");
    result = replaceChar(result, '\n', " ");
    trim(result);
    
    result = removeShortWords(result);
    trim(result);


    std::cout <<"Reading result: -" << result << "\n";

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

//filename passed with .bmp extension, in the meantime changes the fileName to .png
void takeScreenshotAndSaveToFile(const int width, const int height,const Point p,std::string& fileName) {
    Timer timer = Timer();

    timer.start();
    HBITMAP bitmap = takeScreenshot(width, height, p);
    timer.stop();
    timer.print("take screenshot");


    timer.start();

    
    SaveHBITMAPToFile(bitmap, fileName.c_str());
    timer.stop();
    timer.print("saving to file");
    DeleteObject(bitmap);

    

    fileName.pop_back();
    fileName.pop_back();
    fileName.pop_back();
    fileName.pop_back();

    int error = convertBMPtoPNG(fileName);

    myAssert(error != 0, "Error converting bmp to png");

    cv::Mat img = cv::imread("relicTitleScreenshot.png");
    if (img.empty()) {
        std::cerr << "Failed to load image.\n";
        exit(-1);
    }

    fileName += ".png";



}


void takeScreenshotAndSaveToFile(const int width, const int height, const int px, const int py, std::string& fileName) {
    Timer timer = Timer();

    timer.start();
    HBITMAP bitmap = takeScreenshot(width, height, px,py);
    timer.stop();
    timer.print("take screenshot");


    timer.start();


    SaveHBITMAPToFile(bitmap, fileName.c_str());
    timer.stop();
    timer.print("saving to file");
    DeleteObject(bitmap);



    fileName.pop_back();
    fileName.pop_back();
    fileName.pop_back();
    fileName.pop_back();

    int error = convertBMPtoPNG(fileName);

    myAssert(error != 0, "Error converting bmp to png");

    cv::Mat img = cv::imread("relicTitleScreenshot.png");
    if (img.empty()) {
        std::cerr << "Failed to load image.\n";
        exit(-1);
    }

    fileName += ".png";



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

    std::string fileName = "relicTitleScreenshot.bmp";
    takeScreenshotAndSaveToFile(titleWidth,40,px-5,py-5,fileName);



    std::string relicRead = readRelicTitleTesseract(api, fileName.c_str(), false);
    std::string relicParsed = relicMenuTitleStringToRelicString(relicRead);
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


//your mom
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
    
    std::string fileName = "relicTitleScreenshot.bmp";
    takeScreenshotAndSaveToFile(titleWidth - titleWidth / 5,40,(px-x_shift)-5,py-5,fileName);
   

    std::string relicRead = readRelicTitleTesseract(api, fileName.c_str(), false);
    std::string relicParsed = relicMenuTitleStringToRelicString(relicRead);
    RelicInfo relic = FetchRelicItemPrices(relicParsed);

    return relic;



}





bool arePricesEmpty(std::map<std::string, ItemDetails>& itemPrices) {

    for (auto& item : itemPrices) {
        if (item.second.averagePrice != 0) {
            return false;
        }
    }
    return true;



}


//your mom
std::vector<Item> readFissureRewardsScreen(AppState state) {


    Timer timer;

    size_t itemCount = 4;

    std::pair<int, int > coordinates = stringToIntPair(state.config["coordinatesOfScreenShotCenter"]);
    Point p;
    p.x = coordinates.first;
    p.y = coordinates.second;

   
    std::string fileName = state.config["screenShotFilePath"];
    takeScreenshotAndSaveToFile(stoi(state.config["screenShotWidth"]), stoi(state.config["screenShotHeight"]),p,fileName);


    std::vector<std::string> items = readFissureItems(state.tesseractApi, itemCount,fileName);

    timer.start();
    std::vector<std::string> preparedItems = prepareItems(items);
    timer.stop();
    timer.print("preparing items");


    while (checkIfItemsAreValid(preparedItems, state.allAvalibleItems) == 0) {
        itemCount--;
        items = readFissureItems(state.tesseractApi, itemCount,fileName);
        preparedItems = prepareItems(items);
    }

    fixItems(preparedItems, state.allAvalibleItems);

    preparedItems = prepareItems(preparedItems);

    timer.start();
    std::vector<Item> itemPrices = getItemPricesMap(preparedItems);

    timer.stop();
    timer.print("fetching item prices");


    itemPrices = prepareItemsForRead(itemPrices);
    printItemPrices(itemPrices);

    saveItemsToClipboard(itemPrices);


    return itemPrices;
}





//your mom
std::vector<Item> readPreviousFissureRewardsScreen(AppState state) {

   
    Timer timer=Timer();
    size_t itemCount = 4;

    std::string fileName = state.config["screenShotFilePath"];
    std::vector<std::string> items = readFissureItems(state.tesseractApi,itemCount,fileName);

    timer.start();
    std::vector<std::string> preparedItems = prepareItems(items);
    timer.stop();
    timer.print("preparing items");
    

    while (checkIfItemsAreValid(preparedItems, state.allAvalibleItems) == 0) {
        itemCount--;
        items = readFissureItems(state.tesseractApi, itemCount,fileName);
        preparedItems = prepareItems(items);
    }
    
    fixItems(preparedItems, state.allAvalibleItems);

    preparedItems = prepareItems(preparedItems);

    timer.start();
    std::vector<Item> itemPrices = getItemPricesMap(preparedItems);

    timer.stop();
    timer.print("fetching item prices");



    


    itemPrices = prepareItemsForRead(itemPrices);
    printItemPrices(itemPrices);
    saveItemsToClipboard(itemPrices);


    return itemPrices;
}



void tesseractInit(tesseract::TessBaseAPI& api) {

    
        if (api.Init(nullptr, "eng_fast")) {
            errorLog("Could not initialize tesseract");
            exit(0);
        }
        else {
            successLog("Successfully initialized tesseract");
        }
        
    }
   
    

