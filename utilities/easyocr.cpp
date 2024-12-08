#include "../platinumEyes.h"





std::vector<std::string> extractItemsFromServer(std::string s) {



    std::vector<std::string> itemList;


    //is return string empty
    if (strcmp(s.c_str(), "") == 0) {
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




    int starting_point = s.find("return_table");


    starting_point = starting_point + 15;



    int length_to_cut = (s.length() - starting_point) - 2;

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
            int nearestquote = itemArray.find_first_of('"', iterator + 1);


            int itemLength = nearestquote - (iterator + 1);


            std::string itemstring = itemArray.substr(iterator + 1, itemLength);


            int where_to_jump = getItem(itemstring, itemList, iterator);


            iterator = where_to_jump;
            continue;


        }





        iterator++;
    }





    return itemList;
}


int getItem(std::string s, std::vector<std::string>& list, int iterator) {


    int length = s.length();

    list.push_back(s);


    return iterator + length + 2;
}





std::string fetchResultTable(std::string serverAddress, std::string fileToSend) {


    cpr::Response r = cpr::Get(cpr::Url{ serverAddress },
        cpr::Parameters{ {"filepath", fileToSend } });
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string
    std::cout << r.text << std::endl;

    return r.text;



}


std::map<std::string, ItemDetails> readItemsFromScreenEasyocr(ToolConfig& config) {

    Timer timer;


    std::pair<int, int > coordinates = stringToIntPair(config["coordinatesOfScreenShotCenter"]);
    Point p;
    p.x = coordinates.first;
    p.y = coordinates.second;

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


    timer.start_time();
    std::string address = config["ocrIp"] + ":" + config["ocrPort"] + "/ocr";
    std::cout << std::endl << "address: " << address << std::endl;


    std::string resultTable = fetchResultTable(address, file_name_to_send);



    timer.end_time();
    timer.say_time("getting response from a server");

    std::vector<std::string> items = extractItemsFromServer(resultTable);

    std::vector<std::string> preparedItems = prepareItems(items);



    timer.start_time();
    std::map<std::string, ItemDetails> itemPrices = getItemPricesMap(preparedItems);

    timer.end_time();
    timer.say_time("FETCHING ORDERS");


    prepareItemsForRead(itemPrices);
    printItemPrices(itemPrices);


    return itemPrices;
}


std::map<std::string, ItemDetails> readItemsFromScreenWithoutScreenshotEasyocr(ToolConfig& config) {


    Timer timer;



    std::string file_name_to_send = config["screenShotFilePath"];
    LPCTSTR file_name = file_name_to_send.c_str();


    timer.start_time();
    std::string address = config["ocrIp"] + ":" + config["ocrPort"] + "/ocr";


    std::string resultTable = fetchResultTable(address, file_name_to_send);


    timer.end_time();
    timer.say_time("getting response from a server");

    std::vector<std::string> items = extractItemsFromServer(resultTable);

    std::vector<std::string> preparedItems = prepareItems(items);



    timer.start_time();
    std::map<std::string, ItemDetails> itemPrices = getItemPricesMap(preparedItems);

    timer.end_time();
    timer.say_time("FETCHING ORDERS");

    itemPrices = prepareItemsForRead(itemPrices);
    printItemPrices(itemPrices);


    return itemPrices;


}
