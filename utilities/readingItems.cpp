
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


void replaceAnds(std::string& s) {

    int pos = s.find("&");

    if (pos != std::string::npos)
        s.replace(pos, 1, "and");
}



int getItem(std::string s, std::vector<std::string>& list, int iterator) {


    int length = s.length();

    list.push_back(s);


    return iterator + length + 2;
}


void determineRarity(ItemDetails& details, const json& text) {

    std::string id = text["id"];
    int ducatsPrice = 0;
    Rarity::level r = Rarity::level::Undefined;
    for (auto& item : text["items_in_set"]) {
        if (item["id"] == id) {
            ducatsPrice = item["ducats"];
        }
    }

    

    switch (ducatsPrice) {
    case 15: r = Rarity::level::Common; break;
    case 25: r = Rarity::level::Common; break;
    case 45: r = Rarity::level::Uncommon; break;
    case 65: r = Rarity::level::Uncommon; break;
    case 100: r = Rarity::level::Rare; break;
    default: r = Rarity::level::Undefined; break;
    }


    details.rarity = r;

}



std::vector<std::string> prepareItems(std::vector<std::string>& list) {

    std::vector<std::string> newlist;
    for (std::string item : list) {


        std::string newstring = replaceChar(item, ' ', "_");
        replaceAnds(newstring);
        for (auto& x : newstring) {
            x = tolower(x);
        }


        newlist.push_back(newstring);
    }

    return newlist;

}


std::string replaceChar(std::string s, char a, std::string b) {

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


ItemDetails getAveragePrice(const json& list) {
    float sum = 0;
    int count = 0;
    Rarity::level rarity = Rarity::level::Undefined;

    // Use a max-heap priority queue to keep track of the 5 smallest prices
    std::priority_queue<int> lowestPrices;

    for (const json& product : list) {
        if (product["user"]["status"] == "ingame" && product["order_type"] == "sell") {
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
        return ItemDetails(0, 0);
    }

    // Print the 5 lowest prices
    std::vector<int> lowestPricesVec;
    while (!lowestPrices.empty()) {
        lowestPricesVec.push_back(lowestPrices.top());
        lowestPrices.pop();
    }
    std::sort(lowestPricesVec.begin(), lowestPricesVec.end());


    


    ItemDetails pricing = ItemDetails((sum / count), lowestPricesVec,rarity);


    return pricing;
}


ItemDetails fetchItemPrice(const std::string& item) {


    cpr::Response r = cpr::Get(cpr::Url{ "https://api.warframe.market/v1/items/" + item + "/orders?include=item" }

    );
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string


    json data = json::parse(r.text);


    json products = data["payload"]["orders"];

    ItemDetails price = getAveragePrice(products);


    if(price.lowestPrices!=std::vector<int>{0,0,0,0,0})
    determineRarity(price, data["include"]["item"]);


    return price;


}


std::map<std::string, ItemDetails> getItemPricesMap(std::vector<std::string>& preparedItems) {


    std::map<std::string, ItemDetails> itemPrices;
    std::vector<std::future<std::pair<std::string, ItemDetails>>> futures;



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

        futures.push_back(std::async(std::launch::async, [item]() -> std::pair<std::string, ItemDetails> {
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
            s.append(std::to_string(lowestPrices[i]) + ",");
        }
        else {
            s.append(std::to_string(lowestPrices[i]));
        }


    }
    s.append(")");

    return s;

}



void printItemPrices(std::map<std::string, ItemDetails>& itemPrices) {
    std::cout << "Item prices: " << std::endl;
    std::cout << "<------------------------------------------------------------------------->" << std::endl;

    for (std::pair<std::string, ItemDetails> pair : itemPrices) {

        ItemDetails prices = pair.second;
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



std::map<std::string, ItemDetails> prepareItemsForRead(std::map<std::string, ItemDetails>& items) {

    std::map<std::string, ItemDetails> newList = {};

    for (auto& item : items) {
        std::string newstring = replaceChar(item.first, '_', " ");
        replaceAnds(newstring);


        newstring[0] = std::toupper(newstring[0]);
        newList.insert(std::pair<std::string, ItemDetails>(newstring, item.second));
    }

    return newList;

}


std::string fetchResultTable(std::string serverAddress,std::string fileToSend) {


    cpr::Response r = cpr::Get(cpr::Url{ serverAddress },
        cpr::Parameters{ {"filepath", fileToSend } });
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string
    std::cout << r.text << std::endl;

    return r.text;



}


std::map<std::string, ItemDetails> readItemsFromScreen(ToolConfig& config) {

    Timer timer;


    std::pair<int, int > coordinates= stringToIntPair(config["coordinatesOfScreenShotCenter"]);
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


std::map<std::string, ItemDetails> readItemsFromScreenWithoutScreenshot(ToolConfig& config) {


    Timer timer;



    std::string file_name_to_send = config["screenShotFilePath"];
    LPCTSTR file_name = file_name_to_send.c_str();


    timer.start_time();
    std::string address = config["ocrIp"] + ":" + config["ocrPort"] + "/ocr";


    std::string resultTable = fetchResultTable(address,file_name_to_send);


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
