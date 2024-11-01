
#include "warframe_tool.h"
using namespace std;

HBITMAP takeScreenshot(int imageWidth, int imageHeight, int offsetX, int offsetY) {


    HRGN constraints = CreateRectRgn(200, 200, 800, 800);

    const int CONSTRAINTX1 = 200;
    const int CONSTRAINTY1 = 200;
    const int CONSTRAINTX2 = 800;
    const int CONSTRAINTY2 = 800;




    HDC hScreen = GetDCEx(NULL, constraints, DCX_NORESETATTRS);
    HDC hTarget = CreateCompatibleDC(hScreen);

    int width = GetDeviceCaps(hScreen, HORZRES);
    int height = GetDeviceCaps(hScreen, VERTRES);

    std::cout << "Width: " << width << endl;
    std::cout << "\nHeight: " << height << endl;



    //int imageWidth = 1200;
    //int imageHeight = 800;



    //int offsetX = 1000;
    //int offsetY = 200;


    HBITMAP bitmap = CreateCompatibleBitmap(hScreen, imageWidth, imageHeight);
    HGDIOBJ oldbitmap = SelectObject(hTarget, bitmap);

    BitBlt(hTarget, 0, 0, imageWidth, imageHeight, hScreen, offsetX, offsetY, SRCCOPY);



    SelectObject(hScreen, oldbitmap);
    ReleaseDC(NULL, hTarget);
    DeleteDC(hScreen);
    return bitmap;

}

HBITMAP takeScreenshot(int imageWidth, int imageHeight, Point p)
{

    HRGN constraints = CreateRectRgn(200, 200, 800, 800);

    const int CONSTRAINTX1 = 200;
    const int CONSTRAINTY1 = 200;
    const int CONSTRAINTX2 = 800;
    const int CONSTRAINTY2 = 800;


    HDC hScreen = GetDCEx(NULL, constraints, DCX_NORESETATTRS);
    HDC hTarget = CreateCompatibleDC(hScreen);

    int width = GetDeviceCaps(hScreen, HORZRES);
    int height = GetDeviceCaps(hScreen, VERTRES);

    std::cout << "\nScreenParameters: " << std::endl;
    std::cout << "Width: " << width << endl;
    std::cout << "Height: " << height << endl;



    //int imageWidth = 1200;
    //int imageHeight = 800;



    //int offsetX = 1000;
    //int offsetY = 200;

    int leftCornerX = p.x - imageWidth / 2;
    int leftCornerY = p.y - imageHeight / 2;


    HBITMAP bitmap = CreateCompatibleBitmap(hScreen, imageWidth, imageHeight);
    HGDIOBJ oldbitmap = SelectObject(hTarget, bitmap);

    BitBlt(hTarget, 0, 0, imageWidth, imageHeight, hScreen, leftCornerX, leftCornerY, SRCCOPY);



    SelectObject(hScreen, oldbitmap);
    ReleaseDC(NULL, hTarget);
    DeleteDC(hScreen);
    return bitmap;

}

BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
    HDC hDC;
    int iBits;
    WORD wBitCount;
    DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
    BITMAP Bitmap0;
    BITMAPFILEHEADER bmfHdr;
    BITMAPINFOHEADER bi;
    LPBITMAPINFOHEADER lpbi;
    HANDLE fh, hDib, hPal, hOldPal2 = NULL;
    hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);
    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else
        wBitCount = 24;
    GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap0.bmWidth;
    bi.biHeight = -Bitmap0.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 256;
    dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8
        * Bitmap0.bmHeight;
    hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = GetDC(NULL);
        hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }


    GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
        + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

    if (hOldPal2)
    {
        SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
    }

    fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    bmfHdr.bfType = 0x4D42; // "BM"
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);
    return TRUE;
}

void saveScreenshotToClipboard(HBITMAP bitmap) {
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, bitmap);
    CloseClipboard();
}

std::vector<std::string> extractItemsFromServer(std::string s) {

    

    std::vector<std::string> itemList;


    //is return string empty
    if (strcmp(s.c_str(), "")==0) {
        errorLog("return string is empty");
        itemList = {};
        return itemList;
    }



    //if "Internal Server Error exists in given string"
    if (s.find("Internal Server Error") != std::string::npos) {
        
        errorLog("Internal Server Error");
        itemList = {};
        return itemList;

    }




    int starting_point=s.find("return_table");


    starting_point = starting_point + 15;



    int length_to_cut = (s.length()-starting_point)-2;
   
    std::string itemArray = s.substr(starting_point, length_to_cut);
    int iterator = 0;
    char currentc = itemArray[iterator];
    


    while (currentc != ']') {
        currentc = itemArray[iterator];

        if (currentc == '[') {
            iterator++;
            continue;
        }

        if (currentc == '"') {
            int nearestquote = itemArray.find_first_of('"', iterator+1);
            

            int itemLength= nearestquote - (iterator+1);
           

            std::string itemstring = itemArray.substr(iterator+1, itemLength);
           

            int where_to_jump= getItem(itemstring, itemList,iterator);
            

            iterator = where_to_jump;
            continue;


        }





        iterator++;
    }
    
   



    return itemList;
}



int getItem(std::string s,std::vector<std::string>& list,int iterator) {


    int length = s.length();

    list.push_back(s);


    return iterator+length+2;
}



std::vector<std::string> prepareItems(std::vector<std::string>& list) {

    std::vector<std::string> newlist;
    for (std::string item : list) {

        
        std::string newstring = replaceChar(item,' ', "_");
        for (auto& x : newstring) {
            x = tolower(x);
        }


        newlist.push_back(newstring);
    }

    return newlist;

}


std::string replaceChar(std::string s,char a,std::string b) {

    std::string newstring;


    for (char c : s) {

        if (c == a) {
            newstring.append(b);
        }
        else {
            newstring.append(1, c);
        }


    }

    return newstring;

}


ProductPricing getAveragePrice(const json& list) {
    float sum = 0;
    int count = 0;

    // Use a max-heap priority queue to keep track of the 5 smallest prices
    std::priority_queue<int> lowestPrices;

    for (const json& product : list) {
        if (product["user"]["status"] == "ingame" && product["order_type"]=="sell") {
            int price = product["platinum"];
            sum += price;
            count++;

            // Add price to priority queue
            if (lowestPrices.size() < 5) {
                lowestPrices.push(price); // Add to heap if we have less than 5 items
            }
            else if (price < lowestPrices.top()) {
                lowestPrices.pop(); // Remove the highest of the lowest prices
                lowestPrices.push(price); // Push the new price
            }
        }
    }

    if (count == 0) {
        return ProductPricing(0,0);
    }

    // Print the 5 lowest prices
    std::vector<int> lowestPricesVec;
    while (!lowestPrices.empty()) {
        lowestPricesVec.push_back(lowestPrices.top());
        lowestPrices.pop();
    }
    std::sort(lowestPricesVec.begin(), lowestPricesVec.end());


    ProductPricing pricing = ProductPricing((sum / count), lowestPricesVec);

    return pricing;
}


ProductPricing fetchItemPrice(const std::string& item) {


    cpr::Response r = cpr::Get(cpr::Url{ "https://api.warframe.market/v1/items/" + item + "/orders" }

    );
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string


    json data = json::parse(r.text);

    
    json products = data["payload"]["orders"];

    ProductPricing price = getAveragePrice(products);

    return price;


}


std::map<std::string, ProductPricing> getItemPricesMap(std::vector<std::string>& preparedItems) {


    std::map<std::string, ProductPricing> itemPrices;
    std::vector<std::future<std::pair<std::string, ProductPricing>>> futures;



    for (std::string item : preparedItems) {
        
        /*
        cpr::Response r = cpr::Get(cpr::Url{ "https://api.warframe.market/v1/items/" + item + "/orders" }
            //,cpr::Parameters{ {"include", item} }

        );
        r.status_code;                  // 200
        r.header["content-type"];       // application/json; charset=utf-8
        r.text;                         // JSON text string


        json data = json::parse(r.text);

        std::cout << item << std::endl;
        json products = data["payload"]["orders"];

        ProductPricing price = getAveragePrice(products);
        */

        //itemPrices.insert(std::pair<std::string, ProductPricing>(item, price));

        futures.push_back(std::async(std::launch::async, [item]() -> std::pair<std::string, ProductPricing> {
                return { item, fetchItemPrice(item) };
            }
        )
        );






    }

    for (auto& future : futures) {
        auto result = future.get();
        itemPrices.insert(result);
    }

    

    return itemPrices;



}


std::string getFormatedAveragePrices(std::vector<int>& lowestPrices) {

    std::string s;
    s.append("(");
    for (int i = 0; i < lowestPrices.size(); i++) {
        if (i != lowestPrices.size() - 1) {
            s.append(to_string(lowestPrices[i]) + ",");
        }
        else {
            s.append(to_string(lowestPrices[i]));
        }
        

   }
    s.append(")");

    return s;

}



void printItemPrices(std::map<std::string, ProductPricing>& itemPrices) {
    std::cout << "Item prices: " << std::endl;
    std::cout << "<------------------------------------------------------------------------->" << std::endl;
   
    for (std::pair<std::string, ProductPricing> pair : itemPrices) {

        ProductPricing prices = pair.second;
        std::vector<int> lowestPrices = prices.lowestPrices;
        float averagePrice = prices.averagePrice;


        std::vector<int> emptyVector = { 0,0,0,0,0 };
        if (averagePrice == 0 && lowestPrices == emptyVector) {
            std::cout << pair.first << ": " << "COULDNT FIND ITEM ON THE MARKET" << std::endl;
        }
        else {
            std::cout << pair.first << ": " << averagePrice << getFormatedAveragePrices(lowestPrices) << std::endl;
        }

        
    }

}




std::map<std::string, ProductPricing> readItemsFromScreen(ToolConfig& config){

    Timer timer;
    


    Point p=stringToCoordinates(config["coordinatesOfScreenShotCenter"]);

    timer.start_time();
    HBITMAP bitmap = takeScreenshot(stoi(config["screenShotWidth"]), stoi(config["screenShotHeight"]), p);        
    timer.end_time();
    timer.say_time("take screenshot");



    timer.start_time();
    
    std::string file_name_to_send = config["screenShotFilePath"];
    LPCTSTR file_name = file_name_to_send.c_str();
    SaveHBITMAPToFile(bitmap, file_name);
    timer.end_time();
    timer.say_time("saving to file");
    DeleteObject(bitmap);


    //VARIABLES FOR 3440/1440:
    //IMAGEWIDTH:1290
    //IMAGEHEIGHT: 70
    //CENTERPOINT: 1720,570
    timer.start_time();
    std::string address = config["ocrIp"] + ":" + config["ocrPort"]+ "/ocr";
    std::cout << std::endl << "address: " << address<<std::endl;

 

        cpr::Response r = cpr::Get(cpr::Url{ address },
            cpr::Parameters{ {"filepath", file_name_to_send} });
        r.status_code;                  // 200
        r.header["content-type"];       // application/json; charset=utf-8
        r.text;                         // JSON text string
        std::cout << r.text << std::endl;
        

    

    timer.end_time();
    timer.say_time("getting response from a server");

    std::vector<std::string> items = extractItemsFromServer(r.text);

    std::vector<std::string> preparedItems = prepareItems(items);



    timer.start_time();
    std::map<std::string, ProductPricing> itemPrices = getItemPricesMap(preparedItems);

    timer.end_time();
    timer.say_time("FETCHING ORDERS");


    printItemPrices(itemPrices);


    return itemPrices;
}


std::map<std::string, ProductPricing> readItemsFromScreenWithoutScreenshot(ToolConfig& config) {

    
    Timer timer;
    


    std::string file_name_to_send = config["screenShotFilePath"];
    LPCTSTR file_name = file_name_to_send.c_str();

    //VARIABLES FOR 3440/1440:
    //IMAGEWIDTH:1290
    //IMAGEHEIGHT: 70
    //CENTERPOINT: 1720,570
    timer.start_time();
    std::string address = config["ocrIp"] + ":" + config["ocrPort"]+ "/ocr";
    //std::cout << std::endl << "address: " << address<<std::endl;

 

        cpr::Response r = cpr::Get(cpr::Url{ address },
            cpr::Parameters{ {"filepath", file_name_to_send} });
        r.status_code;                 
        r.header["content-type"];       
        r.text;                         
        std::cout << r.text << std::endl;
        

    

    timer.end_time();
    timer.say_time("getting response from a server");

    std::vector<std::string> items = extractItemsFromServer(r.text);

    std::vector<std::string> preparedItems = prepareItems(items);



    timer.start_time();
    std::map<std::string, ProductPricing> itemPrices = getItemPricesMap(preparedItems);

    timer.end_time();
    timer.say_time("FETCHING ORDERS");


    printItemPrices(itemPrices);


    return itemPrices;


}

bool checkIfConfigFileExists() {

    ifstream file;

    file.open(CONFIG_FILENAME);

    if (file) return true;
    else return false;



}




void createConfigFile() {

    ofstream config(CONFIG_FILENAME);


    for (std::string configProperty : CONFIGPROPERTIES) {
        config << configProperty << ": \n";
    }

    config.close();


}


void resolveConfigLine(ToolConfig& toolConfig, std::string& line,int it) {

    int startingPoint = 0;

    
    startingPoint = CONFIGPROPERTIES[it].length() + 1;

    int whereEnds = line.find_last_of("\n");

    std::string configProperty = line.substr(startingPoint, whereEnds - 1);





    std::string key = CONFIGPROPERTIES[it];
    toolConfig.setPropertyValue(key, configProperty);


   

}


ToolConfig readConfigFile() {

    ToolConfig toolConfig = ToolConfig();


    ifstream configFile(CONFIG_FILENAME);
    std::string line;
    int it= 0;
    while (getline(configFile, line)) {
        resolveConfigLine(toolConfig, line,it);
        it+= 1;
    }

    return toolConfig;


}

void errorLog(std::string s) {
    std::cout << "Error: " <<s<< "\n";
}



Point stringToCoordinates(std::string s) {

    int middlePos = s.find(",");

    std::string x = s.substr(0, middlePos);
    std::string y = s.substr(middlePos + 1, s.length() - x.length());




    return Point(std::stoi(x), std::stoi(y));

}




