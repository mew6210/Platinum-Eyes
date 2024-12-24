#include "../platinumEyes.h"
#include <regex>



std::vector<std::string> explode(const std::string& s, const char& c)
{
    std::string buff{ "" };
    std::vector<std::string> v;

    for (auto n : s)
    {
        if (n != c) buff += n; else
            if (n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if (buff != "") v.push_back(buff);

    return v;
}
std::pair<std::string,std::string> readRelicItem(std::string& line,int& cursor) {

    std::pair<std::string, std::string> item;
    int item1start = line.find("td", cursor);
    item1start += 3;
    cursor = item1start;
    int item1end = line.find("<", cursor);
    std::string item1 = line.substr(cursor, item1end - item1start);
    cursor += item1.size();
    item.first = item1;

    int item2start = line.find("<td>",cursor);
    item2start += 4;
    cursor = item2start;
    int item2end = line.find("<",cursor);
    std::string item2 = line.substr(cursor, item2end - item2start);
    item.second = item2;
    cursor += item2.size();
    cursor += 5;



    return item;
}


void replaceAmps(std::string& s) {

    int pos = s.find("&amp;");

    if (pos != std::string::npos)
        s.replace(pos, 5, "&");
}



int parseRelicData() {
    // Input HTML-like file
    std::ifstream inputFile("relictable.html");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return 1;
    }

    // Output file to save parsed data
    std::ofstream outputFile("relictable.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error creating output file!" << std::endl;
        return 1;
    }
    std::vector<int> howMany;
    std::string line = "";
    std::vector < std::pair < std::string, std::string>> items;
    while (getline(inputFile,line)) {
        replaceAmps(line);
        int cursor = 1;
        if (line.starts_with("    <tr class=")) continue;
        if (line.find("<th colspan=")!=-1) {

            int start = line.find("<th colspan=");
            cursor = start+16;
            int relicnamepos = line.find("<", cursor);

            std::string relicname = line.substr(cursor, relicnamepos - cursor);
            cursor += relicname.size();
            outputFile << relicname<<"\n";
            
            /*
            int item1start = line.find("td", cursor);
            item1start += 3;
            cursor =item1start;
            int item1end = line.find("<", cursor + 5);
            std::string item1 = line.substr(cursor, item1end - item1start);
            std::cout << item1<<"\n";
            */
            std::pair<std::string, std::string> item = readRelicItem(line, cursor);
            outputFile << "\t" << item.first << "---" << item.second << "\n";

            for (int i = 0; i < 5; i++) {

                item = readRelicItem(line, cursor);
                outputFile << "\t" << item.first << "---" << item.second << "\n";
            }


        }

        
    }



    inputFile.close();
    outputFile.close();
    return 0;
}


std::array<std::string,6> getRelicRawItems(std::string relic) {

    std::array<std::string, 6> arr;

    std::ifstream inputFile("relictable.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        exit(-1);
    }
    std::string line = "";
    int i = 0;
    int counter = 1;
    bool save = false;
    while (std::getline(inputFile, line)) {

        if (line == relic) {
            save = true;
            continue;
        }
        


        if (save == true&&counter!=7) {
            counter++;
            trim(line);

            arr[i] = line;
            i++;
        }
        if (counter == 7)break;
           
        

    }

    return arr;

}


std::array<std::pair<std::string, std::string>, 6> getRelicItemDetails(std::string relic) {

    std::array<std::string, 6> items = getRelicRawItems(relic);

    std::array<std::pair<std::string, std::string>, 6> itemsSeparated;

    int i = 0;

    for (auto& item : items) {
        std::vector<std::string> separation = explode(item, '---');

        std::pair<std::string, std::string> pair;
        pair.first = separation[0];
        pair.second = separation[1];

        itemsSeparated[i] = pair;


        i++;
    }

    return itemsSeparated;


}


float getPercantageFromString(std::string s) {

    int start = s.find("(");
    int end = s.find(")");
    start += 1;
    end -= 1;

    std::string perc = s.substr(start, end-start);


    float result = std::stof(perc);

    return result;

}




std::string rarityToString(Rarity::level r) {

    switch (r) {
    case Rarity::Common:return "Common";
    case Rarity::Uncommon:return "Uncommon";
    case Rarity::Rare:return "Rare";
    case Rarity::Undefined:return "Undefined";

    }



}




void fixRarity(RelicInfo& relic) {

    int grade = 0;

    if (relic.name.find("Intact") != -1) grade = 1;
    if (relic.name.find("Exceptional") != -1) grade = 2;
    if (relic.name.find("Flawless") != -1) grade = 3;
    if (relic.name.find("Radiant") != -1) grade = 4;


    

    switch (grade) {

    case 1: { 
        
        for (auto& item : relic.items) {

            if (std::get<1>(item) == 25.33) {
                std::get<2>(item).rarity = Rarity::Common;
            }
            if (std::get<1>(item) == 11.0) {
                std::get<2>(item).rarity = Rarity::Uncommon;
            }
            if (std::get<1>(item) == 2.0) {
                std::get<2>(item).rarity = Rarity::Rare;
            }





        }
        
        
        
        
        break; };
    case 2: { 
        
        for (auto& item : relic.items) {

            if (std::get<1>(item) == 23.33) {
                std::get<2>(item).rarity = Rarity::Common;
            }
            if (std::get<1>(item) == 13.0) {
                std::get<2>(item).rarity = Rarity::Uncommon;
            }
            if (std::get<1>(item) == 4.0) {
                std::get<2>(item).rarity = Rarity::Rare;
            }





        }
        
        
        break; };
    case 3: { 
        
        for (auto& item : relic.items) {

            if (std::get<1>(item) == 20.0) {
                std::get<2>(item).rarity = Rarity::Common;
            }
            if (std::get<1>(item) == 17.0) {
                std::get<2>(item).rarity = Rarity::Uncommon;
            }
            if (std::get<1>(item) == 6.0) {
                std::get<2>(item).rarity = Rarity::Rare;
            }





        }
        
        
        
        break; };
    case 4: { 
        
        for (auto& item : relic.items) {

            if (std::get<1>(item) == 16.67) {
                std::get<2>(item).rarity = Rarity::Common;
            }
            if (std::get<1>(item) == 20.0) {
                std::get<2>(item).rarity = Rarity::Uncommon;
            }
            if (std::get<1>(item) == 10.0) {
                std::get<2>(item).rarity = Rarity::Rare;
            }





        }
        
        
        break; };
    case 0: { break; }


    }





}



float calculateRelicPrice(RelicInfo& relic) {

    float price = 0.0;

    for (auto& item : relic.items) {
        float averageItemPrice = 0;
        ItemDetails details = std::get<2>(item);

        for (int& order : details.lowestPrices) {
            averageItemPrice += order;
        }
        averageItemPrice /= details.lowestPrices.size();

        averageItemPrice *= (std::get<1>(item)/100);

        price += averageItemPrice;

    }




    return price;

}


std::string relicMenuTitleStringToRelicString(std::string& s) {

    std::stringstream ss(s);
    std::string word;


    std::vector<std::string> words;

    while (std::getline(ss, word, ' ')) {
        words.push_back(word);
    }
  

    std::string clearedString;

    for (std::string& word_in_words : words) {


        if (word_in_words != "" 
            && word_in_words != " " 
            && word_in_words != "\n"
            && word_in_words !="Rewards"
            && word_in_words !="Possible"
            
            ) {
            clearedString.append(word_in_words);
            clearedString.append(" ");

        }

    }

    int pos = 0;
    std::string resultString = "";


    if ((pos = clearedString.find("Exceptional")) != -1) {
        clearedString.insert(pos, "(");
        clearedString.insert(pos + 12, ")");
        resultString = clearedString;
    }


    else if ((pos = clearedString.find("Flawless")) != -1) {
        clearedString.insert(pos, "(");
        clearedString.insert(pos + 9, ")");
        resultString = clearedString;
    }


    else if ((pos = clearedString.find("Radiant")) != -1) {
        clearedString.insert(pos, "(");
        clearedString.insert(pos + 8, ")");
        resultString = clearedString;
    }

    else {
        clearedString.append("(Intact)");
        resultString = clearedString;
    }
    trim(resultString);


    return resultString;
}


RelicInfo FetchRelicItemPrices(std::string relic) {         //TODO: THIS HAS TO BE REFACTORED LATER LIKE OMFG WHAT IS HAPPENING HERE

    std::array<std::pair<std::string, std::string>, 6> items = getRelicItemDetails(relic);
    std::vector<std::string> itemVector;
    std::vector<std::string> itemVector2;
    
    for (auto& item : items) {
        itemVector.push_back(item.first);
    }




    std::vector<std::string> preparedItems=prepareItems(itemVector);



    std::vector<std::string> preparedVector2 = prepareItems(itemVector);
    std::vector<std::pair<std::string, std::string>> preparedVector2OfPairs;
    int i = 0;
    for (auto& item : preparedVector2) {
        preparedVector2OfPairs.push_back({item,items[i].second});
        i++;
    }

    std::vector<std::future<std::pair<std::string, ItemDetails>>> futures;


    for (auto& item : preparedItems) {
        if (item == "") continue;
        if (item == "forma_blueprint") continue;



        futures.push_back(std::async(std::launch::async, [item]() -> std::pair<std::string, ItemDetails> {
            return { item, fetchItemPrice(item) };
            }
        )
        );
    }

    std::map<std::string, ItemDetails> results;



    for (auto& future : futures) {
        auto result = future.get();
        results.insert(result);
    }

    std::vector<std::tuple<std::string, float, ItemDetails>> appendedInfo;


    for (auto& item : results) {    

        std::string percentageString="";
        for (auto& item2 : preparedVector2OfPairs) {
            if (item.first == item2.first) {            
                percentageString = item2.second;
            }
        }


        appendedInfo.push_back(std::make_tuple(item.first,getPercantageFromString(percentageString),item.second));
    }

    RelicInfo relic1;
    relic1.items = appendedInfo;
    relic1.name = relic;
    relic1.relicPrice = calculateRelicPrice(relic1);
    fixRarity(relic1);

    return relic1;



}