#include "relics.h"
#include <regex>
#include <ctime>

using std::vector, std::string, std::pair,std::ofstream,std::ifstream;

vector<string> explode(const string& s, const string& delimiter){
    vector<string> result;
    size_t start = 0;
    size_t end;

    while ((end = s.find(delimiter, start)) != string::npos) {
        if (end != start) // avoid empty tokens from consecutive delimiters
            result.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }

    if (start < s.length())
        result.push_back(s.substr(start));

    return result;
}


namespace {
    string getHtmlLineItemName(const string& line, size_t& cursor) {

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
    string getHtmlLineItemPercantage(const string& line, size_t& cursor) {


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
pair<string,string> parseItemFromHtmlLine(const string& line,size_t& cursor) {

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

    if (s == "") {
        errorLog(false, "empty string date");
        return Date{ 0,0,0 };
    }

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

    static const string amp = "&amp;";
    size_t pos = s.find(amp);
    if (pos != string::npos)
        s.replace(pos, amp.length(), "&");
}

bool doesDatabaseExist() {
    ifstream txtDatabase;
    txtDatabase.open("data/relictable_lith.txt");
    return txtDatabase.is_open();
}

bool is_first_launch(ToolConfig& config) {

    if (config["data_LastTimeLaunched"] == "0 0 0" && config["data_LatestUpdate"] == "0 0 0") {
        successLog("First launch, hello!");
        return true;
    } 
    else return false;
}

bool shouldUpdate(ToolConfig& config) {

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
    if (shouldUpdate(config)) return pair<bool,bool>(true,false);

    return pair<bool,bool>(false,false);
}

void fetchRelicTable() {

    std::ifstream inputFile("data/droptable-raw.html");
    if (!inputFile.is_open()) {
        errorLog(false,"Error opening input file!");
        return;
    }
    std::ofstream outputFile("data/relictable.html");
    if (!outputFile.is_open()) {
        errorLog(false,"Error opening input file!");
        return;
    }

    string line = "";
    bool read_flag = false;
    bool close_flag = false;
    while (std::getline(inputFile, line)) {

        if (line.find("<h3 id=\"relicRewards\">Relics:</h3>")!=std::string::npos) {
            read_flag = true;
            continue;
        
        } 
        if (line.find("<h3 id=\"keyRewards\">Keys:</h3>")!=std::string::npos) {
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
    string line = "";
    bool read_flag = false;
    string date_string = "";

    ifstream inputFile("data/droptable-raw.html");
    if (!inputFile.is_open()) {
        errorLog(false, "failed to open droptable-raw.html");
        return Date{ 0,0,0 };
    }
    while (getline(inputFile, line)) {

        if (line.find("<p><b>Last Update:</b>") != std::string::npos){
            read_flag = true;
            continue;
        }
        if (read_flag) {
            date_string = line;
            break;
        }
        
    }
    inputFile.close();

    if (date_string == "") {
        errorLog(false, "could not find date in droptable-raw");
        return Date{ 0,0,0 };
    }

    updateDate = stringToDate(date_string);
    return updateDate;
}


std::string dateToString(Date& date) {
    return std::to_string(date.day) + " " + std::to_string(date.month) + " " + std::to_string(date.year);;
}

cpr::Response downloadFile(const std::string& downloadLink,const std::string& downloadDestination) {

    std::ofstream of(downloadDestination, std::ios::binary);
    cpr::Response r = cpr::Download(of, cpr::Url{ downloadLink });
    of.close();

    return r;
}

void handleSuccesfullDatabaseDownload(cpr::Response& r,ToolConfig& config,bool& forced) {
    
    
        Date latestUpdateRecorded = stringToDate(config["data_LatestUpdate"]);
        Date droptable_raw_date = getNewestUpdateDate();

        config.setPropertyValue("data_LatestUpdate", dateToString(droptable_raw_date));

        if (latestUpdateRecorded < droptable_raw_date) {
            successLog("New update, downloading...");
            fetchRelicTable();
            parseRelicData();
            std::remove("data/relictable.html");

        }
        else if (forced == true) {
            successLog("New update forced, downloading...");
            fetchRelicTable();
            parseRelicData();
            std::remove("data/relictable.html");
        }
        else {
            successLog("Relic database is up to date!");
        }
}

void handleUnSuccesfullDatabaseDownload(cpr::Response& r) {

    if (r.status_code == 0) {
        errorLog(true,"Check your internet connection, could not download raw html droptable.");
    }
    else {
        errorLog(true,"Couldn't download raw html droptable. Status code: ",std::to_string(r.status_code));
    }
}


void updateDatabase(ToolConfig& config,bool forced) {

    cpr::Response r = downloadFile("https://warframe-web-assets.nyc3.cdn.digitaloceanspaces.com/uploads/cms/hnfvc0o3jnfvc873njb03enrf56.html", "data/droptable-raw.html");

    if (r.status_code == 200) {
        handleSuccesfullDatabaseDownload(r, config, forced);
    }
    else {
        handleUnSuccesfullDatabaseDownload(r);
    }
    
    rewriteConfigFile(config);
    std::remove("data/droptable-raw.html");
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


void writeRelicLinesToFile(std::ofstream& outputFile,const string& relicname,const string& line, size_t& cursor) {
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
    string line = "";
    
    ifstream inputFile("data/allItemsFile.txt");
    if (!inputFile.is_open()) {
        errorLog(false, "failed to open all items file");
        return {};
    }
    while (getline(inputFile, line)) {

        trim(line);
        allItems.push_back(line);
    }

    return allItems;
}


/*
@brief Writes all items from inputFile to outputFile and allItems vector in a specific format.

For example, this line:
    
    Saryn Prime Neuroptics Blueprint---Uncommon (11.00%)

    with "\t" at the start, writes 'saryn_prime_neuroptics_blueprint' to outputFile and adds to allItems vector.
*/
void appendAllItemsFromFile(std::ifstream& inputFile,vector<string>& allItems,std::ofstream& outputFile) {

    string line = "";
    while (getline(inputFile, line)) {

        if (line.starts_with("\t")) {
            
            vector<string> words = explode(line, "---");

            trim(words[0]);

            string itemName= replaceChar(words[0], ' ', "_");
            //replaceAnds(newstring);
            for (auto& x : itemName) {
                x = tolower(x);
            }
            if (std::find(allItems.begin(), allItems.end(), itemName) == allItems.end()) {
                
                allItems.push_back(itemName);
                outputFile << itemName<< "\n";
            }
        }

    }
}


void parseAllItemsToFile(std::unordered_map<string,std::ifstream>& inputFiles) {
    
    vector<string> alreadyReadItems;
    ofstream allItemsFile("data/allItemsFile.txt");

    for (auto& [name, inputFile] : inputFiles) {
        appendAllItemsFromFile(inputFile, alreadyReadItems, allItemsFile);
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

    string getRelicName(const string& line, size_t& cursor) {

        size_t start = line.find("<th colspan=");
        cursor = start + 16;
        size_t relicnamepos = line.find("<", cursor);

        string relicname = line.substr(cursor, relicnamepos - cursor);
        cursor += relicname.size();

        return relicname;
    }

    void handleRelic(const string& line, size_t& cursor,std::unordered_map<string,std::ofstream>& outputFiles ) {

        const string relicname = getRelicName(line, cursor);
        int relictype = determineRelicType(relicname);

        switch (relictype) {
        case RELICTYPE_Lith: writeRelicLinesToFile(outputFiles["lith"], relicname, line, cursor); break;
        case RELICTYPE_Meso: writeRelicLinesToFile(outputFiles["meso"], relicname, line, cursor); break;
        case RELICTYPE_Neo: writeRelicLinesToFile(outputFiles["neo"], relicname, line, cursor); break;
        case RELICTYPE_Axi: writeRelicLinesToFile(outputFiles["axi"], relicname, line, cursor); break;
        case -1: writeRelicLinesToFile(outputFiles["others"], relicname, line, cursor); break;
        }
    }

    void allItemsToOneFile() {

        std::unordered_map<string, ifstream> inputFiles;
        inputFiles.emplace("lith", ifstream("data/relictable_lith.txt"));
        inputFiles.emplace("meso", ifstream("data/relictable_meso.txt"));
        inputFiles.emplace("neo", ifstream("data/relictable_neo.txt"));
        inputFiles.emplace("axi", ifstream("data/relictable_axi.txt"));

        checkInputFiles(inputFiles);

        parseAllItemsToFile(inputFiles);
    }

    void flushofStreams(std::unordered_map<string,ofstream>& outputFiles) {
        for (auto& file : outputFiles) {
            file.second.flush();
        }
    }

    void processRelicData(ifstream& inputFile, std::unordered_map<string, ofstream>& outputFiles) {
        string line = "";
        while (getline(inputFile, line)) {
            replaceAmp(line);
            size_t cursor = 1;
            if (line.starts_with("    <tr class=")) continue;
            if (line.find("<th colspan=") != string::npos) {
                handleRelic(line, cursor, outputFiles);
            }
        }
        flushofStreams(outputFiles);
    }
}

int parseRelicData() {
    ifstream inputFile("data/relictable.html");
    if (!inputFile.is_open()) {
    errorLog(false,"Error opening input file!");
        return 1;
    }
    std::unordered_map<string, ofstream> outputFiles;
    outputFiles.emplace("others", ofstream("data/relictable_others.txt"));
    outputFiles.emplace("lith", ofstream("data/relictable_lith.txt"));
    outputFiles.emplace("meso", ofstream("data/relictable_meso.txt"));
    outputFiles.emplace("neo", ofstream("data/relictable_neo.txt"));
    outputFiles.emplace("axi", ofstream("data/relictable_axi.txt"));
    
    if (checkOutputFiles(outputFiles) != 0) return 1;

    processRelicData(inputFile,outputFiles);
    allItemsToOneFile();
    return 0;
}


std::array<string,6> getRelicRawItems(string relic) {

    std::array<string, 6> arr;

    std::ifstream inputFile;

    int relictype = determineRelicType(relic);
    
    switch (relictype) {
    case RELICTYPE_Lith:inputFile.open("data/relictable_lith.txt"); break;
    case RELICTYPE_Meso:inputFile.open("data/relictable_meso.txt"); break;
    case RELICTYPE_Neo:inputFile.open("data/relictable_neo.txt"); break;
    case RELICTYPE_Axi:inputFile.open("data/relictable_axi.txt"); break;
    case -1:inputFile.open("data/relictable_others.txt"); break;
    }

    if (!inputFile.is_open()) {
        errorLog(false,"Error opening input file!");
        exit(1);
    }

    string line = "";
    int i = 0;
    int counter = 1;
    bool save = false;
    int line_counter = 1;
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


    return arr;

}


std::array<pair<string, string>, 6> getRelicItemDetails(std::string relic) {

    std::array<string, 6> items = getRelicRawItems(relic);
    std::array<pair<string, string>, 6> itemsSeparated;

    int i = 0;

    for (auto& item : items) {
        if (item.find("---") != -1) {

            std::vector<std::string> separation = explode(item, "---");

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

    size_t start = s.find("(");
    size_t end = s.find(")");

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
    default: return "Should never happen";
    }
}

bool compareWithRoundingErrors(float number,float compareTo) {
    constexpr float EPSILON = 0.0001f;
    return std::abs(number - compareTo) < EPSILON;
}

void compareRaritiesToPercentages(RelicItem& item,const vector<float>& percantages) {


    if (percantages.size() != 3) errorLog(true,"wrong amount of percantages in compareRaritiesToPercantages");


    if (compareWithRoundingErrors(item.percentage, percantages[0])) {
        item.itemDetails.rarity = Rarity::Common;
    }
    if (compareWithRoundingErrors(item.percentage, percantages[1])) {
        item.itemDetails.rarity = Rarity::Uncommon;
    }
    if (compareWithRoundingErrors(item.percentage, percantages[2])) {
        item.itemDetails.rarity = Rarity::Rare;
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
    case 0: { break; }
    }
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
    if (words.size()>2){
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

    size_t pos = 0;
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

std::map<string,ItemDetails> fetchRelicVectorPrices(vector<string> names) {

    vector<std::future<pair<string, ItemDetails>>> futures;

    Timer timer = Timer();
    timer.start();
    for (auto& item : names) {
        if (item == "") continue;
        if (item == "forma_blueprint") continue;
        if (item == "2x_forma_blueprint") continue;


        futures.push_back(std::async(std::launch::async, [item]() -> pair<string, ItemDetails> {
            return { item, fetchItemPrice(item) };
            }
        )
        );
    }

    std::map<string, ItemDetails> results;

    for (auto& future : futures) {
        auto result = future.get();
        results.insert(result);
    }
    timer.stop();
    timer.print("Fetching relic item's prices from warframe market");
    return results;
}


RelicInfo fetchRelicItemPrices(std::string relicName) {         //TODO: THIS HAS TO BE REFACTORED LATER LIKE OMFG WHAT IS HAPPENING HERE

    if (relicName == "nullRelic")  return RelicInfo("nullRelic",{},0);
    
    std::array<pair<string, string>, 6> items = getRelicItemDetails(relicName);
    vector<string> itemNames;
    
    for (auto& item : items) {
        string snakedName = itemToSnakeCase(item.first);
        itemNames.push_back(snakedName);
    }

    vector<pair<string, string>> itemsWithDetails;
    int i = 0;
    for (auto& item : itemNames) {
        itemsWithDetails.push_back({item,items[i].second});
        i++;
    }

    vector<std::future<pair<string, ItemDetails>>> futures;


    auto results = fetchRelicVectorPrices(itemNames);
    
    vector<RelicItem> relicItems;

    for (auto& item : results) {    

        std::string percentageString="";
        for (auto& item2 : itemsWithDetails) {
            if (item.first == item2.first) {            
                percentageString = item2.second;
            }
        }
        relicItems.push_back(RelicItem(snakeToItem(item.first), item.first, item.second, getPercantageFromString(percentageString)));    //TODO: rawname the same as preparedname, it shouldn't be like that but it is never used so ig its aight
    }

    RelicInfo relic=RelicInfo(relicName,relicItems,0);
    relic.calculateRelicPrice();
    fixRarity(relic);

    //sort relic items based on their rarity
    //TODO: later add a possibility to sort it in the opposite way, through a setting in appstate
    std::sort(relic.items.begin(), relic.items.end(), [](auto& a,auto& b) {
        return a.itemDetails.rarity > b.itemDetails.rarity;
        });

    return relic;
}

void printRelic(RelicInfo& relic) {

    if (relic.name != "nullRelic") {

        std::cout << "Relic name: " << relic.name;
        for (auto& item : relic.items) {

            std::cout << "Name: " << item.rawName;
            std::cout << " Chance for drop: " << item.percentage;
            std::cout << " prices: " << getFormatedAveragePrices(item.itemDetails.lowestPrices);
            std::cout << "rarity: " << rarityToString(item.itemDetails.rarity) << "\n";
        }
        std::cout << "Average relic price: " << relic.relicPrice;
    }
}