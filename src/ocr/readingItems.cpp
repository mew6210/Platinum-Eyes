#include "ocr.h"


int levenshtein_distance(const std::string& str1, const std::string& str2) {
    int len_str1 = str1.length();
    int len_str2 = str2.length();

    // Create a 2D matrix to store distances
    std::vector<std::vector<int>> matrix(len_str1 + 1, std::vector<int>(len_str2 + 1));

    // Initialize the matrix with the base case values
    for (int i = 0; i <= len_str1; ++i) {
        matrix[i][0] = i;
    }
    for (int j = 0; j <= len_str2; ++j) {
        matrix[0][j] = j;
    }

    // Compute the Levenshtein distance
    for (int i = 1; i <= len_str1; ++i) {
        for (int j = 1; j <= len_str2; ++j) {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
            matrix[i][j] = std::min({ matrix[i - 1][j] + 1,        // Deletion
                                matrix[i][j - 1] + 1,        // Insertion
                                matrix[i - 1][j - 1] + cost  // Substitution
                });
        }
    }

    return matrix[len_str1][len_str2];
}

// Function for fuzzy search
std::vector<std::pair<std::string, int>> fuzzy_search(const std::string& query, const std::vector<std::string>& string_list, int threshold = 3) {
    std::vector<std::pair<std::string, int>> results;

    for (const auto& str : string_list) {
        int distance = levenshtein_distance(query, str);
        if (distance <= threshold) {  // If distance is within the threshold, it's a match
            results.push_back(std::make_pair(str, distance));
        }
    }

    return results;
}




int levenshtein_distance_weighted(const std::string& str1, const std::string& str2, int end_weight = 2, int begin_weight = 2) {
    int len_str1 = str1.length();
    int len_str2 = str2.length();

    // Create a 2D matrix to store distances
    std::vector<std::vector<int>> matrix(len_str1 + 1, std::vector<int>(len_str2 + 1));

    // Initialize the matrix with the base case values
    for (int i = 0; i <= len_str1; ++i) {
        matrix[i][0] = i * (i == len_str1 ? end_weight : begin_weight);
    }
    for (int j = 0; j <= len_str2; ++j) {
        matrix[0][j] = j * (j == len_str2 ? end_weight : begin_weight);
    }

    // Compute the Levenshtein distance
    for (int i = 1; i <= len_str1; ++i) {
        for (int j = 1; j <= len_str2; ++j) {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
            int del_cost = matrix[i - 1][j] + (i == len_str1 ? end_weight : 1);
            int ins_cost = matrix[i][j - 1] + (j == len_str2 ? end_weight : 1);
            matrix[i][j] = std::min({ del_cost, ins_cost, matrix[i - 1][j - 1] + cost });
        }
    }

    return matrix[len_str1][len_str2];
}



std::vector<std::pair<std::string, int>> fuzzy_search_weighted(const std::string& query, const std::vector<std::string>& string_list, int threshold = 3, int end_weight = 2, int begin_weight = 2) {
    std::vector<std::pair<std::string, int>> results;

    for (const auto& str : string_list) {
        int distance = levenshtein_distance_weighted(query, str, end_weight, begin_weight);
        if (distance <= threshold) {
            results.push_back(std::make_pair(str, distance));
        }
    }

    return results;
}




int checkIfItemsAreValid(std::vector<std::string>& items, std::vector<std::string>& allItems) {

    std::vector<int> distances;
    for (const auto& item : items) {
        std::vector<std::pair<std::string, int>> fuzzySearchResults = fuzzy_search(item, allItems, 10);

        
        if (!fuzzySearchResults.empty()) {
            auto minElement = std::min_element(fuzzySearchResults.begin(), fuzzySearchResults.end(),
                [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                    return a.second < b.second;
                });
            distances.push_back(minElement->second);
        }
        else {
            std::cout << "No results found for item: " << item << '\n';
        }
        
    }
    int lowThresholdCounter = 0;
    for (const auto& distance : distances) {
        if (distance <= fuzzy_threshold::THRESHOLD_LOW) lowThresholdCounter++;
    }



    return lowThresholdCounter;
}


void fixItems(std::vector<std::string>& items, std::vector<std::string>& allItems) {

    for (auto& item : items) {
        std::vector<std::pair<std::string, int>> fuzzySearchResults = fuzzy_search_weighted(item, allItems, fuzzy_threshold::THRESHOLD_MEDIUM);

        std::sort(fuzzySearchResults.begin(), fuzzySearchResults.end(), [](auto& left, auto& right) {
            return left.second < right.second;
            });



        if (!fuzzySearchResults.empty()) {
            auto minElement = std::min_element(fuzzySearchResults.begin(), fuzzySearchResults.end(),
                [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                    return a.second < b.second;
                });

            item = minElement->first;
            

        }
        else {
            std::cout << "Couldnt fix item: " << item << "\n";
        }

        

        
    }
}



void replaceAnds(std::string& s) {

    int pos = s.find("&");

    if (pos != std::string::npos)
        s.replace(pos, 1, "and");
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
        if (product["user"]["status"] == "ingame" && product["type"] == "sell") {
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


    cpr::Response r = cpr::Get(cpr::Url{ "https://api.warframe.market/v2/orders/item/" + item},
        cpr::Header{{"User-Agent","PlatinumEyes/1.0"}}
    );
    /*
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string
    */
    

    json data = json::parse(r.text);


    json products = data["data"];

    ItemDetails price = getAveragePrice(products);

    
    if(price.lowestPrices!=std::vector<int>{0,0,0,0,0})
    //determineRarity(price, data["include"]["item"]);
    //TODO: should be replaced with determining rarity by percentages in warframe pc drops table

    return price;


}


std::vector<Item> getItemPricesMap(std::vector<std::string>& preparedItems) {


    std::vector<Item> itemPrices;
    std::vector<std::future<Item>> futures;



    for (std::string item : preparedItems) {
       
        //skip items that dont need displaying, like '' or 'forma_blueprint'
        if (item=="") continue;     
        if (item == "forma_blueprint") continue;



        futures.push_back(std::async(std::launch::async, [item]() -> Item {
            return  Item(item,item, fetchItemPrice(item)) ;     //to change later
            }
        )
        );

    }

    for (auto& future : futures) {
        auto result = future.get();
        itemPrices.push_back(result);
    }



    return itemPrices;



}

//change naming later
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



void printItemPrices(std::vector<Item>& itemPrices) {
    std::cout << "Item prices: " << std::endl;

    for (Item item : itemPrices) {




        std::vector<int> emptyVector = { 0,0,0,0,0 };
        if (item.itemDetails.averagePrice == 0 && item.itemDetails.lowestPrices== emptyVector) {
            std::cout << item.preparedName<< ": " << "COULDNT FIND ITEM ON THE MARKET" << std::endl;
        }
        else {
            std::cout << item.preparedName<< ": " << item.itemDetails.averagePrice<< getFormatedAveragePrices(item.itemDetails.lowestPrices) << std::endl;
        }


    }

}



std::vector<Item> prepareItemsForRead(std::vector<Item>& items) {

    std::vector<Item> newList = {};

    for (auto& item : items) {
        std::string newstring = replaceChar(item.rawName, '_', " ");
        replaceAnds(newstring);


        //newstring[0] = std::toupper(newstring[0]);

        std::stringstream stream(newstring);
        std::string word = "";
        std::vector<std::string> words = {};
        while (stream >> word) {
            words.push_back(word);
        }

        
        std::string capitalizedString = "";
        for (auto& word : words)
        {
            word[0] = std::toupper(word[0]);
            capitalizedString += word + " ";

        }

        capitalizedString.pop_back();




        newList.push_back(Item(capitalizedString, item.rawName,item.itemDetails));
    }

    return newList;

}

