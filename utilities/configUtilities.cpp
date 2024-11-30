#include "../platinumEyes.h"




bool checkIfConfigFileExists() {

    std::ifstream file;

    file.open(CONFIG_FILENAME);

    if (file) return true;
    else return false;



}

WindowParameters getWindowSize(std::string s,ToolConfig& toolConfig) {

    WindowParameters parameters;

    for (auto& c : s) {
        c = std::tolower(c);
    }
    if (s == "sfml") {

        std::pair<int, int> sfmlCoordinates = stringToIntPair(toolConfig["sfmlSize"]);
        parameters = WindowParameters(sfmlCoordinates.first, sfmlCoordinates.second);
    }
    else if (s == "imgui") {
        std::pair<int, int> imguiCoordinates = stringToIntPair(toolConfig["imguiSize"]);
        parameters = WindowParameters(imguiCoordinates.first, imguiCoordinates.second);

    }
    else {
        return parameters;
    }
    return parameters;


}


void createConfigFile() {

    std::ofstream config(CONFIG_FILENAME);


    for (std::string configProperty : CONFIGPROPERTIES) {
        config << configProperty << ": \n";
    }

    config.close();


}


void resolveConfigLine(ToolConfig& toolConfig, std::string& line, int it) {

    int startingPoint = 0;


    startingPoint = CONFIGPROPERTIES[it].length() + 1;

    int whereEnds = line.find_last_of("\n");

    std::string configProperty = line.substr(startingPoint, whereEnds - 1);





    std::string key = CONFIGPROPERTIES[it];
    toolConfig.setPropertyValue(key, configProperty);




}


ToolConfig readConfigFile() {

    ToolConfig toolConfig = ToolConfig();


    std::ifstream configFile(CONFIG_FILENAME);
    std::string line;
    int it = 0;
    while (getline(configFile, line)) {
        resolveConfigLine(toolConfig, line, it);
        it += 1;
    }

    return toolConfig;


}


void copyConfigToOldFile() {

     std::ifstream fileCurrent(CONFIG_FILENAME);

     std::ofstream fileCopy(COPY_FILENAME);

     std::string line;
     while (std::getline(fileCurrent,line)) {

         fileCopy << line << "\n";

     }

}

void rewriteConfigFile(ToolConfig& config) {


    std::ofstream configFile(CONFIG_FILENAME);

    std::string line;


    for (auto& property : CONFIGPROPERTIES) {

        configFile << property <<": "<< config[property]<<"\n";


    }



}

