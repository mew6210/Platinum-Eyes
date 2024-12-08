#include "../platinumEyes.h"
#include <sstream>


std::vector<std::string> itemsSmallerThan2 = { "bo" };      //all the items that are smaller in name size than 2, should probably do it programatically somehow, but ill do it later


unsigned decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, const std::vector<unsigned char>& bmp) {
    static const unsigned MINHEADER = 54; //minimum BMP header size

    if (bmp.size() < MINHEADER) return -1;
    if (bmp[0] != 'B' || bmp[1] != 'M') return 1; //It's not a BMP file if it doesn't start with marker 'BM'
    unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
    //read width and height from BMP header
    w = bmp[18] + bmp[19] * 256;
    h = bmp[22] + bmp[23] * 256;
    int signedHeight = bmp[22] + (bmp[23] << 8) + (bmp[24] << 16) + (bmp[25] << 24);
    h = (signedHeight < 0) ? -signedHeight : signedHeight;
    //read number of channels from BMP header
    if (bmp[28] != 24 && bmp[28] != 32) return 2; //only 24-bit and 32-bit BMPs are supported.
    unsigned numChannels = bmp[28] / 8;

    //The amount of scanline bytes is width of image times channels, with extra bytes added if needed
    //to make it a multiple of 4 bytes.
    unsigned scanlineBytes = w * numChannels;
    if (scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

    unsigned dataSize = scanlineBytes * h;

    std::cout << "Bmp size: " << bmp.size() << "\n";
    std::cout << "Pixel Offset: " << pixeloffset << "\n";
    std::cout << "Data size: " << dataSize << "\n";
    std::cout << "w: " << w << "\n";
    std::cout << "h: " << h << "\n";

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
    lodepng::load_file(bmp, "screenshot.bmp");
    std::vector<unsigned char> image;
    unsigned w, h;
    unsigned error = decodeBMP(image, w, h, bmp);

    if (error) {
        std::cout << "BMP decoding error " << error << std::endl;
        return 0;
    }

    std::vector<unsigned char> png;
    error = lodepng::encode(png, image, w, h);

    if (error) {
        std::cout << "PNG encoding error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 0;
    }

    lodepng::save_file(png, "screenshot.png");
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



std::vector<std::string> readScreenShotTesseract(tesseract::TessBaseAPI& api,size_t itemCount) {
    Timer timer = Timer();
    timer.start();
    std::string path = "screenshot.bmp";
    int error = convertBMPtoPNG(path);

    myAssert(error != 0, "Error converting bmp to png");

    cv::Mat img = cv::imread("screenshot.png");
    if (img.empty()) {
        std::cerr << "Failed to load image.\n";
        exit(-1);
    }
    timer.stop();
    timer.print("converting bmp to png");

    
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

          cv::Mat first_item
              = img(cv::Range(30, img.rows), cv::Range(0, img.cols));
          std::vector<std::string> items;

          items.push_back(readItemTesseract(first_item, api, false));
          return items;



    }

  



}




std::string readItemTesseract(cv::Mat& image, tesseract::TessBaseAPI& api,bool showImage) {


    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    //cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    //cv::Canny(image, image, 100, 200);




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


    std::cout <<result<< "\n";

    return result;


}

bool arePricesEmpty(std::map<std::string, ItemDetails>& itemPrices) {

    for (auto& item : itemPrices) {
        if (item.second.averagePrice != 0) {
            return false;
        }
    }
    return true;



}




std::map<std::string, ItemDetails> readItemsFromScreenTesseract(ToolConfig& config, tesseract::TessBaseAPI& api) {

    Timer timer;

    size_t itemCount = 4;

    std::pair<int, int > coordinates = stringToIntPair(config["coordinatesOfScreenShotCenter"]);
    Point p;
    p.x = coordinates.first;
    p.y = coordinates.second;

    timer.start();
    HBITMAP bitmap = takeScreenshot(stoi(config["screenShotWidth"]), stoi(config["screenShotHeight"]), p);
    timer.stop();
    timer.print("take screenshot");




    timer.start();
    std::string file_name_to_send = config["screenShotFilePath"];
    LPCTSTR file_name = file_name_to_send.c_str();
    SaveHBITMAPToFile(bitmap, file_name);
    timer.stop();
    timer.print("saving to file");
    DeleteObject(bitmap);




    std::vector<std::string> items = readScreenShotTesseract(api,itemCount);

    timer.start();
    std::vector<std::string> preparedItems = prepareItems(items);
    timer.stop();
    timer.print("preparing items");

  
    timer.start();
    std::map<std::string, ItemDetails> itemPrices = getItemPricesMap(preparedItems);


    while (arePricesEmpty(itemPrices) && itemCount != 0) {
        itemCount--;
        items = readScreenShotTesseract(api, itemCount);
        preparedItems = prepareItems(items);
        itemPrices = getItemPricesMap(preparedItems);
    }
    timer.stop();
    timer.print("fetching item prices");

    itemPrices = prepareItemsForRead(itemPrices);
    printItemPrices(itemPrices);


    return itemPrices;
}






std::map<std::string, ItemDetails> readItemsFromScreenWithoutScreenShotTesseract(ToolConfig& config, tesseract::TessBaseAPI& api) {

   
    Timer timer=Timer();
    size_t itemCount = 4;
    std::vector<std::string> items = readScreenShotTesseract(api,itemCount);

    timer.start();
    std::vector<std::string> preparedItems = prepareItems(items);
    timer.stop();
    timer.print("preparing items");


    timer.start();
    std::map<std::string, ItemDetails> itemPrices = getItemPricesMap(preparedItems);

    while (arePricesEmpty(itemPrices) && itemCount!=0) {
        itemCount--;
        items = readScreenShotTesseract(api, itemCount);
        preparedItems = prepareItems(items);
        itemPrices = getItemPricesMap(preparedItems);
    }
    timer.stop();
    timer.print("fetching item prices");



    


    itemPrices = prepareItemsForRead(itemPrices);
    printItemPrices(itemPrices);


    return itemPrices;
}



int tesseractInit(tesseract::TessBaseAPI& api, ToolConfig& config) {

    if (config["ocrType"] == "tesseract") {
        if (api.Init(nullptr, "eng")) {
            std::cerr << "Could not initialize Tesseract.\n";
            return 1;
        }
        else {
            std::cout << "Successfully initialized tesseract\n";
        }
        return OCR_tesseract;
    }
    else {
        return OCR_easyocr;
    }
    return 0;

}