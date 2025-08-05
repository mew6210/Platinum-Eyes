#include "ocr.h"

using std::string, std::vector, std::pair;


//should never be used outside readingItems.cpp
namespace {

    int levenshtein_distance(const string& str1, const string& str2) {
        int len_str1 = str1.length();
        int len_str2 = str2.length();

        // Create a 2D matrix to store distances
        vector<vector<int>> matrix(len_str1 + 1, vector<int>(len_str2 + 1));

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
    vector<pair<string, int>> fuzzy_search(const string& query, const vector<string>& string_list, int threshold = 3) {
        vector<pair<string, int>> results;

        for (const auto& str : string_list) {
            int distance = levenshtein_distance(query, str);
            if (distance <= threshold) {  // If distance is within the threshold, it's a match
                results.push_back(std::make_pair(str, distance));
            }
        }

        return results;
    }




    int levenshtein_distance_weighted(const string& str1, const string& str2, int end_weight = 2, int begin_weight = 2) {
        int len_str1 = str1.length();
        int len_str2 = str2.length();

        // Create a 2D matrix to store distances
        vector<vector<int>> matrix(len_str1 + 1, vector<int>(len_str2 + 1));

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



    vector<pair<string, int>> fuzzy_search_weighted(const string& query, const vector<string>& string_list, int threshold = 3, int end_weight = 2, int begin_weight = 2) {
        vector<pair<string, int>> results;

        for (const auto& str : string_list) {
            int distance = levenshtein_distance_weighted(query, str, end_weight, begin_weight);
            if (distance <= threshold) {
                results.push_back(std::make_pair(str, distance));
            }
        }

        return results;
    }



    /*
    -uses fuzzy search to find best match for needle item, in haystack allItems
    -uses threshold MEDIUM
    -if doesn't find anything thats closer than threshold MEDIUM then returns nullopt.
    */
    std::optional<pair<string, int>> findBestMatch(const string& item, vector<string>& allItems) {

        vector<pair<string, int>> fuzzySearchResults = fuzzy_search(item, allItems, fuzzy_threshold::THRESHOLD_MEDIUM);


        if (!fuzzySearchResults.empty()) {

            auto minElement = std::min_element(
                fuzzySearchResults.begin(),
                fuzzySearchResults.end(),
                [](const pair<string, int>& a, const pair<string, int>& b) {
                    return a.second < b.second;
                });

            return *minElement;
        }
        else {
            warningLog("no results found for item: " + item);
            return std::nullopt;
        }

    }


    
    int countItemsInsideAThreshold(const vector<int>& distances, int threshold) {


        int inThresholdCounter = 0;
        for (const auto& distance : distances) {
            if (distance <= threshold) inThresholdCounter++;
        }

        return inThresholdCounter;

    }


}




/*
-checks how many items are 'close enough' to be recognized as a valid item string
-it returns 0(inclusive)-however many items are provided as an int
-also alerts if there is no resutls for a given string of item
*/
int checkIfItemsAreValid(vector<string>& items, vector<string>& allItems) {

    vector<int> distances;
    for (const auto& item : items) {
        auto bestMatch = findBestMatch(item, allItems);
        if (bestMatch) {
            distances.push_back(bestMatch->second);
        } 
    }

    int inThresholdCount = (countItemsInsideAThreshold(distances, fuzzy_threshold::THRESHOLD_LOW));


    return inThresholdCount;
}


/*
- tries to find the closest items by fuzzy searching
- returns true if it succeeded at least once
- returns false if it failed
*/
bool fixItems(vector<string>& items, vector<string>& allItems) {

    for (auto& item : items) {
        vector<pair<string, int>> fuzzySearchResults = fuzzy_search_weighted(item, allItems, fuzzy_threshold::THRESHOLD_HIGH);

        std::sort(
            fuzzySearchResults.begin(), 
            fuzzySearchResults.end(), 
            [](auto& left, auto& right) {
            return left.second < right.second;
            });



        if (!fuzzySearchResults.empty()) {
            auto minElement = std::min_element(
                fuzzySearchResults.begin(), 
                fuzzySearchResults.end(),
                [](const pair<string, int>& a, const pair<string, int>& b) {
                    return a.second < b.second;
                });

            item = minElement->first;
            

        }

        else if (items.size() == 1 && fuzzySearchResults.empty()) {
            return false;
        }
        
    }
    return true;
}



void replaceAnds(string& s) {

    int pos = s.find("&");

    if (pos != string::npos)
        s.replace(pos, 1, "and");
}



void determineRarity(ItemDetails& details,const int& ducats) {



    Rarity::level r = Rarity::level::Undefined;

    switch (ducats) {
    case 15: r = Rarity::level::Common; break;
    case 25: r = Rarity::level::Common; break;
    case 45: r = Rarity::level::Uncommon; break;
    case 65: r = Rarity::level::Uncommon; break;
    case 100: r = Rarity::level::Rare; break;
    default: r = Rarity::level::Undefined; break;
    }


    details.rarity = r;

}



string itemToSnakeCase(const string& s) {
    string snakedItem = replaceChar(s, ' ', "_");
    replaceAnds(snakedItem);
    for (auto& x : snakedItem) {
        x = tolower(x);
    }
    return snakedItem;
}

/*



*/
vector<string> itemsToSnakeCase(const vector<string>& list) {

    vector<string> snakedItems;
    for (const string& item : list) {
        string snakedItem = itemToSnakeCase(item);
        snakedItems.push_back(snakedItem);
    }
    return snakedItems;
}


string replaceChar(string s, char a, string b) {

    string newstring;


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
    vector<int> lowestPricesVec;
    while (!lowestPrices.empty()) {
        lowestPricesVec.push_back(lowestPrices.top());
        lowestPrices.pop();
    }
    std::sort(lowestPricesVec.begin(), lowestPricesVec.end());


    


    ItemDetails pricing = ItemDetails((sum / count), lowestPricesVec,rarity);


    return pricing;
}


ItemDetails fetchItemPrice(const string& item) {


    cpr::Response r = cpr::Get(cpr::Url{ "https://api.warframe.market/v2/orders/item/" + item},
        cpr::Header{{"User-Agent","PlatinumEyes/1.0"}}
    );

    json data = json::parse(r.text);

    json products = data["data"];

    ItemDetails price = getAveragePrice(products);

    
    if (price.lowestPrices != vector<int>{0, 0, 0, 0, 0}) {

        int ducats = getDucatsFromSlug(item);
        determineRarity(price, ducats);
    }
   

    return price;
}


vector<Item> getItemPricesMap(vector<string>& preparedItems) {


    vector<Item> itemPrices;
    vector<std::future<Item>> futures;



    for (string item : preparedItems) {
       
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
string getFormatedAveragePrices(vector<int>& lowestPrices) {

    string s;
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



void printItemPrices(vector<Item>& itemPrices) {
    std::cout << "Item prices: " << std::endl;

    for (Item item : itemPrices) {
        vector<int> emptyVector = { 0,0,0,0,0 };
        if (item.itemDetails.averagePrice == 0 && item.itemDetails.lowestPrices== emptyVector) {
            std::cout << item.preparedName<< ": " << "COULDNT FIND ITEM ON THE MARKET" << std::endl;
        }
        else {
            std::cout << item.preparedName<< ": " << item.itemDetails.averagePrice<< getFormatedAveragePrices(item.itemDetails.lowestPrices) << std::endl;
        }
    }
}



vector<Item> prepareItemsForRead(vector<Item>& items) {

    vector<Item> newList = {};

    for (auto& item : items) {
        string newstring = replaceChar(item.rawName, '_', " ");
        replaceAnds(newstring);


        //newstring[0] = std::toupper(newstring[0]);

        std::stringstream stream(newstring);
        string word = "";
        vector<string> words = {};
        while (stream >> word) {
            words.push_back(word);
        }

        
        string capitalizedString = "";
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

