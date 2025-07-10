#include "relics.h"
#include <regex>
#include <ctime>

using std::vector, std::string, std::pair,std::ofstream,std::ifstream;

vector<string> explode(const string& s, const char& c)
{
    string buff{ "" };
    vector<string> v;

    for (auto n : s)
    {
        if (n != c) buff += n; else
            if (n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if (buff != "") v.push_back(buff);

    return v;
}



namespace {
    string getHtmlLineItemName(const string& line,int& cursor) {

        if (line.find("td", cursor) == string::npos) errorLog(true,"Failed parsing html line");
        size_t item1start = line.find("td", cursor);
        item1start += 3;
        cursor = item1start;
        if (line.find("<", cursor) == string::npos) errorLog(true,"Failed parsing html line");
        size_t item1end = line.find("<", cursor);
        std::string item1 = line.substr(cursor, item1end - item1start);
        cursor += item1.size();
        return item1;
    }
    string getHtmlLineItemPercantage(const string& line, int& cursor) {


        if (line.find("<td>", cursor) == string::npos) errorLog(true,"Failed parsing html line");
        size_t item2start = line.find("<td>", cursor);
        item2start += 4;
        cursor = item2start;
        if (line.find("<", cursor) == string::npos) errorLog(true,"Failed parsing html line");
        size_t item2end = line.find("<", cursor);
        std::string item2 = line.substr(cursor, item2end - item2start);
        return item2;

    }
}

/*
    -parses html line into a pair of strings, one for the name and one for the rarity. Example: 
    <tr><td>Akstiletto Prime Barrel</td><td>Uncommon (11.00%)</td></tr> -> <Aksiletto Prime Barrel,Uncommon (11.00%)>
    -then modifies cursor, to be set at the end of this expression, so that when used in a loop will find all occurences of such items in a line.
*/
pair<string,string> parseItemFromHtmlLine(const string& line,int& cursor) {

    string itemName = getHtmlLineItemName(line, cursor);
    string itemPercentage = getHtmlLineItemPercantage(line, cursor);

    cursor += itemPercentage.size();
    cursor += 5;

    return {itemName,itemPercentage};
}


int monthToInt(const string& s) {

    if (s == "January") {
        return 1;
    }
    if (s == "February") {
        return 2;
    }
    if (s == "March") {
        return 3;
    }
    if (s == "April") {
        return 4;
    }
    if (s == "May") {
        return 5;
    }
    if (s == "June") {
        return 6;
    }
    if (s == "July") {
        return 7;
    }
    if (s == "August") {
        return 8;
    }
    if (s == "September") {
        return 9;
    }
    if (s == "October") {
        return 10;
    }
    if (s == "November") {
        return 11;
    }
    if (s == "December") {
        return 12;
    }

    return -1;


}

struct Date {

    int day;
    int month;
    int year;

    bool operator<(const Date& other) const {
        if (year != other.year) {
            return year < other.year;
        }
        if (month != other.month) {
            return month < other.month;
        }
        return day < other.day;
    }

    // Overload > operator
    bool operator>(const Date& other) const {
        if (year != other.year) {
            return year > other.year;
        }
        if (month != other.month) {
            return month > other.month;
        }
        return day > other.day;
    }

};




Date getCurrentDate() {

    std::time_t currentTime = std::time(nullptr);
    std::tm* const dateInfo = std::localtime(&currentTime);

    return Date{ dateInfo->tm_mday,(dateInfo->tm_mon) + 1,1900+(dateInfo->tm_year) };
}

/*
- converts string date, that is found on warframe pc drops into a Date struct
- example: "22 May, 2025" -> <22,5,2025>

TODO: Add throwing exceptions if std::stoi fails.
*/
Date stringToDate(const string& s) {

    std::stringstream ss(s);

    string word;

    vector<string> words;

    while (getline(ss, word, ' ')) {
        words.push_back(word);
    }

    if (words[1].back() == ',') {
        words[1].pop_back();
    }


    int day = std::stoi(words[0]);
    int month = -1;
    if (words[1].length() >= 3) {
        month = monthToInt(words[1]);
    }
    else {
        month = std::stoi(words[1]);
    }

    int year = std::stoi(words[2]);






    return Date{ day,month,year };

}



void replaceAmp(string& s) {

    string amp = "&amp;";

    size_t pos = s.find(amp);

    if (pos != string::npos)
        s.replace(pos, amp.length(), "&");
}

bool doesDatabaseExist() {

    ifstream txtDatabase;
    
    txtDatabase.open("relictable_lith.txt");

    return txtDatabase.is_open();

  
}


bool is_first_launch(ToolConfig& config) {

    if (config["data_LastTimeLaunched"] == "0 0 0" && config["data_LatestUpdate"] == "0 0 0") {
        successLog("First launch, hello!");
        return true;
    
    } 
    else return false;


}

bool checkUpdatingType(ToolConfig& config) {

    std::string updatingType = config["updatingType"];


    if (updatingType == "Never") {
        return false;
    }
    if (updatingType == "Once per day") {

        if (stringToDate(config["data_LastTimeLaunched"]) < getCurrentDate()) {
            successLog("New day, checking for downloads");
            return true;
        }
        else {
            successLog("Same day, no need to check for downloads");
            return false;
        }



    }
    if (updatingType == "Each Launch") {
        return true;
    }
    return true;


}




//first is if it should update, and second is how it should be updated
//if its false, then it should check the recency of the update, if true, then the update should be forced no matter what
pair<bool,bool> shouldUpdateDatabase(ToolConfig& config) {
    
    

    if (is_first_launch(config)) return pair<bool,bool>(true,true);
    if (!doesDatabaseExist()) return pair<bool,bool>(true,true);
    if (checkUpdatingType(config)) return pair<bool,bool>(true,false);


    return pair<bool,bool>(false,false);
}

void fetchRelicTable() {

    std::ifstream inputFile("droptable-raw.html");
    if (!inputFile.is_open()) {
        errorLog(false,"Error opening input file!");
        exit(1);
    }
    std::ofstream outputFile("relictable.html");
    if (!outputFile.is_open()) {
        errorLog(false,"Error opening input file!");
        exit(0);
    }

    string line = "";
    bool read_flag = false;
    bool close_flag = false;
    while (std::getline(inputFile, line)) {

        if (line == "<h3 id=\"relicRewards\">Relics:</h3>") {
            read_flag = true;
            continue;
        
        } 
        if (line == "<h3 id=\"keyRewards\">Keys:</h3>") {
            read_flag = false;
            close_flag = true;
        } 
            
        
        if (read_flag) {
            outputFile << line << "\n";
        }
        if (close_flag) {
            break;  
        }



    }
    inputFile.close();
    outputFile.close();



}




Date getNewestUpdateDate() {

    Date updateDate;

    ifstream inputFile("droptable-raw.html");

    string line = "";
    bool close_flag = false;
    bool read_flag = false;
    string date_string = "";
    while (getline(inputFile, line)) {

        if (line == "<p><b>Last Update:</b> ") 
        {
            read_flag = true;
            continue;
        }
        if (read_flag) {
            date_string = line;
            break;
        }
        
    }

    inputFile.close();

    updateDate = stringToDate(date_string);


    return updateDate;

}


std::string dateToString(Date& date) {
    std::string currentTimeString = std::to_string(date.day) + " " + std::to_string(date.month) + " " + std::to_string(date.year);

    return currentTimeString;
}

cpr::Response downloadFile(const std::string& downloadLink,const std::string& downloadDestination) {

    std::ofstream of(downloadDestination, std::ios::binary);
    cpr::Response r = cpr::Download(of, cpr::Url{ downloadLink });

    of.close();

    return r;
}

void handleSuccesfullDatabaseDownload(cpr::Response& r,ToolConfig& config,bool& forced) {
    
    if (r.status_code == 200) {
        Date latestUpdateRecorded = stringToDate(config["data_LatestUpdate"]);
        Date droptable_raw_date = getNewestUpdateDate();


        config.setPropertyValue("data_LatestUpdate", dateToString(droptable_raw_date));

        if (latestUpdateRecorded < droptable_raw_date) {
            successLog("New update, downloading...");
            fetchRelicTable();
            parseRelicData();
            std::remove("relictable.html");

        }
        else if (forced == true) {
            successLog("New update forced, downloading...");
            fetchRelicTable();
            parseRelicData();
            std::remove("relictable.html");
        }
        else {
            successLog("Relic database is up to date!");
        }
    }
}

void handleUnSuccesfullDatabaseDownload(cpr::Response& r) {

    if(r.status_code!=200){
        if (r.status_code == 0) {
            errorLog(true,"Check your internet connection, could not download raw html droptable.");
        }
        else {
            errorLog(true,"Couldn't download raw html droptable. Status code: ",std::to_string(r.status_code));
        }
       
    }

}


void updateDatabase(ToolConfig& config,bool forced) {

    cpr::Response r = downloadFile("https://warframe-web-assets.nyc3.cdn.digitaloceanspaces.com/uploads/cms/hnfvc0o3jnfvc873njb03enrf56.html", "droptable-raw.html");

    handleSuccesfullDatabaseDownload(r,config,forced);
    
    handleUnSuccesfullDatabaseDownload(r);

    
    rewriteConfigFile(config);
    std::remove("droptable-raw.html");


}

void updateCurrentDate(ToolConfig& config) {


    Date currentDate = getCurrentDate();

    config.setPropertyValue("data_LastTimeLaunched", dateToString(currentDate));
    rewriteConfigFile(config);
}


void loadRelicDatabase(ToolConfig& config,pair<bool,bool>& updateOrders) {

    if (updateOrders.first) {
        updateDatabase(config,updateOrders.second);
    }

    updateCurrentDate(config);
}


constexpr auto RELICTYPE_Lith = 1;
constexpr auto RELICTYPE_Meso = 2;
constexpr auto RELICTYPE_Neo = 3;
constexpr auto RELICTYPE_Axi = 4;



int determineRelicType(const string& relicname) {

    if (relicname.find("Lith") != string::npos) {
        return RELICTYPE_Lith;

    }
    if (relicname.find("Meso") != string::npos) {
        return RELICTYPE_Meso;
    }
    if (relicname.find("Neo") != string::npos) {
        return RELICTYPE_Neo;
    }
    if (relicname.find("Axi") != string::npos) {
        return RELICTYPE_Axi;
    }
    return -1;
}


void writeToRelicFile(std::ofstream& outputFile,const string& relicname,const string& line,int& cursor) {
    outputFile << relicname << "\n";
    pair<string, string> item = parseItemFromHtmlLine(line, cursor);
    outputFile << "\t" << item.first << "---" << item.second << "\n";

    for (int i = 0; i < 5; i++) {

        item = parseItemFromHtmlLine(line, cursor);
        outputFile << "\t" << item.first << "---" << item.second << "\n";
    }
}


vector<string> loadAllAvalibleItemsToVector() {

    vector<string> allItems = {};

    ifstream inputFile("allItemsFile.txt");
    
    string line = "";

    while (getline(inputFile, line)) {

        trim(line);
        allItems.push_back(line);
    }


    return allItems;

}



void processAllItemsFromTypeFile(std::ifstream& typeFile,std::vector<std::string>& allItems,std::ofstream& outputFile) {

    std::string line = "";

    while (getline(typeFile, line)) {

        if (line.starts_with("\t")) {

            std::vector<std::string> words = explode(line, '---' );

            trim(words[0]);

            std::string newstring = replaceChar(words[0], ' ', "_");
            //replaceAnds(newstring);
            for (auto& x : newstring) {
                x = tolower(x);
            }


            if (std::find(allItems.begin(), allItems.end(), newstring) == allItems.end()) {
                

                allItems.push_back(newstring);
                outputFile << newstring << "\n";
            }
            else continue;
              


        }

    }



}


void parseAllItemsToFile(std::unordered_map<string,std::ifstream>& inputFiles) {
    
    vector<string> alreadyReadItems;
    ofstream allItemsFile("allItemsFile.txt");

    for (auto& [name, inputFile] : inputFiles) {
        processAllItemsFromTypeFile(inputFile, alreadyReadItems, allItemsFile);
    }

    allItemsFile.close();
}




namespace {
    //returns non-zero return value if cant open any of those files.
    bool checkOutputFiles(const std::unordered_map<string,std::ofstream>& fileNames) {
        for (const auto& [name,outputFile] : fileNames) {

            if (!outputFile.is_open()) {
                errorLog(false,"Error opening output file: " + name);
                return 1;
            }
            
        }
        return 0;

    }
    //same thing as checkOutputFiles but with input files.
    bool checkInputFiles(const std::unordered_map<string, std::ifstream>& fileNames) {
        for (const auto& [name, inputFile] : fileNames) {

            if (!inputFile.is_open()) {
                errorLog(false,"Error opening input file: " + name);
                return 1;
            }

        }
        return 0;

    }

    string getRelicName(const string& line,int& cursor) {

        int start = line.find("<th colspan=");
        cursor = start + 16;
        int relicnamepos = line.find("<", cursor);

        string relicname = line.substr(cursor, relicnamepos - cursor);
        cursor += relicname.size();

        return relicname;

    }

    void handleRelic(const string& line,int& cursor,std::unordered_map<string,std::ofstream>& outputFiles ) {

        const string relicname = getRelicName(line, cursor);
        int relictype = determineRelicType(relicname);

        switch (relictype) {
        case RELICTYPE_Lith: writeToRelicFile(outputFiles["lith"], relicname, line, cursor); break;
        case RELICTYPE_Meso: writeToRelicFile(outputFiles["meso"], relicname, line, cursor); break;
        case RELICTYPE_Neo: writeToRelicFile(outputFiles["neo"], relicname, line, cursor); break;
        case RELICTYPE_Axi: writeToRelicFile(outputFiles["axi"], relicname, line, cursor); break;
        case -1: writeToRelicFile(outputFiles["others"], relicname, line, cursor); break;

        }

    }

    void allItemsToOneFile() {

        std::unordered_map<string, ifstream> inputFiles;
        inputFiles.emplace("lith", ifstream("relictable_lith.txt"));
        inputFiles.emplace("meso", ifstream("relictable_meso.txt"));
        inputFiles.emplace("neo", ifstream("relictable_neo.txt"));
        inputFiles.emplace("axi", ifstream("relictable_axi.txt"));

        checkInputFiles(inputFiles);

        parseAllItemsToFile(inputFiles);

    }


}

int parseRelicData() {
    // Input HTML-like file
    ifstream inputFile("relictable.html");
    if (!inputFile.is_open()) {
    errorLog(false,"Error opening input file!");
        return 1;
    }
    std::unordered_map<string, ofstream> outputFiles;
    outputFiles.emplace("others", ofstream("relictable_others.txt"));
    outputFiles.emplace("lith", ofstream("relictable_lith.txt"));
    outputFiles.emplace("meso", ofstream("relictable_meso.txt"));
    outputFiles.emplace("neo", ofstream("relictable_neo.txt"));
    outputFiles.emplace("axi", ofstream("relictable_axi.txt"));
    

    if (checkOutputFiles(outputFiles) != 0) return 1;

        
    string line = "";
    while (getline(inputFile,line)) {

        replaceAmp(line);
        int cursor = 1;
        if (line.starts_with("    <tr class=")) continue;
        if (line.find("<th colspan=")!=string::npos) {
            handleRelic(line, cursor, outputFiles);
        }
        
    }


    allItemsToOneFile();

    return 0;
}


std::array<std::string,6> getRelicRawItems(std::string relic) {

    std::array<std::string, 6> arr;

    std::ifstream inputFile;


    int relictype = determineRelicType(relic);
    

    switch (relictype) {
    case RELICTYPE_Lith:inputFile.open("relictable_lith.txt"); break;
    case RELICTYPE_Meso:inputFile.open("relictable_meso.txt"); break;
    case RELICTYPE_Neo:inputFile.open("relictable_neo.txt"); break;
    case RELICTYPE_Axi:inputFile.open("relictable_axi.txt"); break;
    case -1:inputFile.open("relictable_others.txt"); break;
    }

    if (!inputFile.is_open()) {
        errorLog(false,"Error opening input file!");
        exit(1);
    }

    std::string line = "";
    int i = 0;
    int counter = 1;
    bool save = false;
    int line_counter = 1;
    Timer timer = Timer();
    timer.start();
    while (std::getline(inputFile, line)) {

        if (line == relic) {
            save = true;
            continue;
        }
        //if (save == false && line_counter % 7 != 1) continue;


        if (save == true&&counter!=7) {
            counter++;
            trim(line);

            arr[i] = line;
            i++;
        }
        if (counter == 7)break;
           
        
        line_counter++;
    }
    timer.stop();
    timer.print("The loop");

    return arr;

}


std::array<std::pair<std::string, std::string>, 6> getRelicItemDetails(std::string relic) {

    std::array<std::string, 6> items = getRelicRawItems(relic);

    std::array<std::pair<std::string, std::string>, 6> itemsSeparated;

    int i = 0;

    for (auto& item : items) {
        if (item.find("---") != -1) {

            std::vector<std::string> separation = explode(item, '---');

            std::pair<std::string, std::string> pair;
            pair.first = separation[0];
            pair.second = separation[1];

            itemsSeparated[i] = pair;


        }
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

bool compareWithRoundingErrors(float number,float compareTo) {

    constexpr float EPSILON = 0.0001;

    return std::abs(number - compareTo) < EPSILON;
    


}

void compareRaritiesToPercentages(std::tuple<string,float,ItemDetails>& item,const vector<float>& percantages) {


    if (percantages.size() != 3) errorLog(true,"wrong amount of percantages in compareRaritiesToPercantages");


    if (compareWithRoundingErrors(std::get<1>(item), percantages[0])) {
        std::get<2>(item).rarity = Rarity::Common;
    }
    if (compareWithRoundingErrors(std::get<1>(item), percantages[1])) {
        std::get<2>(item).rarity = Rarity::Uncommon;
    }
    if (compareWithRoundingErrors(std::get<1>(item), percantages[2])) {
        std::get<2>(item).rarity = Rarity::Rare;
    }




}



void fixRarity(RelicInfo& relic) {

    int grade = 0;

    if (relic.name.find("Intact") != std::string::npos) grade = 1;
    else if (relic.name.find("Exceptional") != std::string::npos) grade = 2;
    else if (relic.name.find("Flawless") != std::string::npos) grade = 3;
    else if (relic.name.find("Radiant") != std::string::npos) grade = 4;


    

    switch (grade) {

    case 1: { 
        
        for (auto& item : relic.items) {

            compareRaritiesToPercentages(item, { 25.33f,11.0f,2.0f });

        }

        break; };
    case 2: { 
        
        for (auto& item : relic.items) {

            compareRaritiesToPercentages(item, { 25.33f,13.0f,4.0f });

        }

        break; };
    case 3: { 
        
        for (auto& item : relic.items) {

            compareRaritiesToPercentages(item, { 20.0f,17.0f,6.0f });

        }
    
        break; };
    case 4: { 
        
        for (auto& item : relic.items) {
         
            compareRaritiesToPercentages(item, { 16.67f,20.0f,10.0f });

        }
        
        break; };

    case 0: { 
    break; }


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


bool isRelicTypeString(const std::string& s) {

    if (s == "Lith"
        || s == "Meso"
        || s == "Neo"
        || s == "Axi"
        || s == "Requiem"   //probably never used
        ) {
        return true;
    }
    else return false;


}

//returns -1 if the first letter is not a digit, or returns the digit if it is
int firstLetterIsDigit(const std::string& s) {

    if (std::isdigit(s[0])) {
        return s[0] - '0';
    }
    
    return -1;



}



std::string relicMenuTitleStringToRelicString(std::string& s) {

    std::stringstream ss(s);
    std::string word;
    if (s == "") {
        return "nullRelic";
    }
    
    //split read string into a vector of words
    std::vector<std::string> words;

    while (std::getline(ss, word, ' ')) {


        //filter out useless junk
        if (word != ""
            && word != " "
            && word != "\n"
            && word != "Rewards"
            && word != "Possible"
            )
        {
            words.push_back(word);
        }

    }

    //fix incorrect reading of I or O in the second word of a relic
    if (words.size()>2)
    {
        if (words[2] == "Relic" && isRelicTypeString(words[0])) {
            int firstLetterNumber = 0;
            if ((firstLetterNumber = firstLetterIsDigit(words[1])) != -1) {
                switch (firstLetterNumber) {
                case 0: words[1][0] = 'O'; break;
                case 1: words[1][0] = 'I'; break;
                }

            }

        }
    }
    else {
        return "weird relic things";
    }




    //make a vector of words into a whole word
    std::string clearedString;

    for (std::string& word_in_words : words) {
            clearedString.append(word_in_words);
            clearedString.append(" ");
    }

    int pos = 0;
    std::string resultString = "";

    //add braces around tier level, since thats how our file database reads it from warframe pc drops
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

    if (relic == "nullRelic") {
        RelicInfo relic;
        relic.name = "couldnt find relic";
        relic.relicPrice = 0;


        return relic;
    }

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

    Timer timer = Timer();
    timer.start();
    for (auto& item : preparedItems) {
        if (item == "") continue;
        if (item == "forma_blueprint") continue;
        if (item == "2x_forma_blueprint") continue;


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
    timer.stop();
    timer.print("fetching items from warframe market");

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

    //sort relic items based on their rarity
    //TODO: later add a possibility to sort it in the opposite way, through a setting in appstate
    std::sort(relic1.items.begin(), relic1.items.end(), [](auto& a,auto& b) {
        return std::get<2>(a).rarity > std::get<2>(b).rarity;
        });

    return relic1;



}

void printRelic(RelicInfo& relic) {

    
    if (relic.name != "couldnt find relic") {



        std::cout << "Relic name: " << relic.name;
        for (auto& price : relic.items) {

            std::cout << "Name: " << std::get<0>(price);
            std::cout << " percentages: " << std::get<1>(price);
            std::cout << " prices: " << getFormatedAveragePrices(std::get<2>(price).lowestPrices);
            std::cout << "rarity: " << rarityToString(std::get<2>(price).rarity) << "\n";
        }
        std::cout << "Average relic price: " << relic.relicPrice;


    }
}