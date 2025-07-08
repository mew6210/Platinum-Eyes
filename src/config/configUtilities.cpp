#include "config.h"
#include <direct.h>   


using std::string, std::vector;


int getScreenResolution(int screenWidth, int screenHeight) {
    int screenResolution = SCR_Size_NotSpecified;

    if (screenWidth == 1920 && screenHeight == 1080) screenResolution = SCR_Size_1920x1080;
    if (screenWidth == 3440 && screenHeight == 1440) screenResolution = SCR_Size_3440x1440;


    return screenResolution;

}



namespace screenShotParams{

    struct screenConfig {
        string screenShotWidth;
        string screenShotHeight;
        string centerCoordinates;
        string sfmlSize;
        string imguiSize;
    };

    screenConfig screen_1920x1080 = { "965","80","960,420","800,250","700,200" };
    screenConfig screen_3440x1440 = {"1290","90","1720,570","1200,300","1100,250"};
    screenConfig screen_NotSpecified= { "---","---","---","---","---" };





    string getCurrentPath() {

        const size_t size = 1024;
        // Allocate a character array to store the directory path
        char buffer[size];

        // Call _getcwd to get the current working directory and store it in buffer
        if (getcwd(buffer, size) != NULL) {
            // print the current working directory
            std::cout << "Current working directory: " << buffer << std::endl;
        }
        else {
            // If _getcwd returns NULL, print an error message
            std::cerr << "Error getting current working directory" << std::endl;
        }

        string returnString(buffer);
        returnString.append("/screenshot.bmp");

        return returnString;


    }

    string getCenterCoordinates(int screenResolution){
        switch (screenResolution) {
        case SCR_Size_1920x1080: return screen_1920x1080.centerCoordinates; break;
        case SCR_Size_3440x1440: return screen_3440x1440.centerCoordinates; break;



        case SCR_Size_NotSpecified:
        default:
            return screen_NotSpecified.centerCoordinates; break;


        }

    }

    string getWidth(int screenResolution){
    
        switch (screenResolution) {
        case SCR_Size_1920x1080: return screen_1920x1080.screenShotWidth; break;
        case SCR_Size_3440x1440: return screen_3440x1440.screenShotWidth; break;



        case SCR_Size_NotSpecified:
        default:
            return screen_NotSpecified.screenShotWidth; break;


        }
    
    
    }

    string getHeight(int screenResolution){
    
        switch (screenResolution) {
        case SCR_Size_1920x1080: return screen_1920x1080.screenShotHeight; break;
        case SCR_Size_3440x1440: return screen_3440x1440.screenShotHeight; break;



        case SCR_Size_NotSpecified:
        default:
            return screen_NotSpecified.screenShotHeight; break;


        }
    }

    string getSfmlSize(int screenResolution){
        switch (screenResolution) {
        case SCR_Size_1920x1080: return screen_1920x1080.sfmlSize; break;
        case SCR_Size_3440x1440: return screen_3440x1440.sfmlSize; break;



        case SCR_Size_NotSpecified:
        default:
            return screen_NotSpecified.sfmlSize; break;


        }
    
    
    
    }

    string getImGuiSize(int screenResolution){
    
        switch (screenResolution) {
        case SCR_Size_1920x1080: return screen_1920x1080.imguiSize; break;
        case SCR_Size_3440x1440: return screen_3440x1440.imguiSize; break;



        case SCR_Size_NotSpecified:
        default:
            return screen_NotSpecified.imguiSize; break;


        }
    
    
    
    }


}




bool checkIfConfigFileExists() {

    std::ifstream file(CONFIG_FILENAME);
    return file.is_open();

}

WindowParameters getWindowSize(string s,ToolConfig& toolConfig) {

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

void fillOutConfigFile(std::ofstream& configFile) {

    int screenResolution = getNativeScreenResolution();

    for (std::string configProperty : CONFIGPROPERTIES) {

        configFile << configProperty << ": ";

        
        if (configProperty == "screenShotFilePath") {
            configFile << screenShotParams::getCurrentPath();
        }
        else if (configProperty == "coordinatesOfScreenShotCenter") {
            configFile << screenShotParams::getCenterCoordinates(screenResolution);
        }
        else if (configProperty == "screenShotWidth") {
            configFile << screenShotParams::getWidth(screenResolution);
        }
        else if (configProperty == "screenShotHeight") {
            configFile << screenShotParams::getHeight(screenResolution);
        }
        else if (configProperty == "sfmlSize") {
            configFile << screenShotParams::getSfmlSize(screenResolution);
        }
        else if (configProperty == "imguiSize") {
            configFile << screenShotParams::getImGuiSize(screenResolution);
        }
        else if(configProperty=="keyBind_ReadItemsFromScreen") {
            configFile << "b";
        }
        else if (configProperty == "keyBind_EscapeProgram") {
            configFile<<"esc";
        }
        else if (configProperty == "keyBind_ReadPreviousItems") {
            configFile<<"x";
        }
        else if (configProperty == "keyBind_WindowVisibility") {
            configFile<<"c";
        }
        else if (configProperty == "keyBind_BackupConfig") {
            configFile<<"a";
        }
        else if (configProperty == "keyBind_ExampleItems"){
            configFile<<"s";
        }
        else if (configProperty == "keyBind_ReadRelicTitle") {
            configFile << "f";
        }
        else if (configProperty == "fontFile") {
            configFile << "Lexend-Regular.ttf";
        }
        else if (configProperty == "fontSize") {
            configFile << "12";
        }
        else if (configProperty == "data_LastTimeLaunched") {
            configFile << "0 0 0";
        }
        else if (configProperty == "data_LatestUpdate") {
            configFile << "0 0 0";
        }
        else if (configProperty == "updatingType") {
            configFile << "Once per day";
        }
        else if (configProperty == "fpsVisible") {
            configFile << "60";
        }
        else if (configProperty == "fpsHidden") {
            configFile << "10";
        }
        else if (configProperty == "clipboardCopy") {
            configFile << "yes";
        }
        else if (configProperty == "clipboardWatermark") {
            configFile<<"yes";
        }
    
    



        configFile << "\n";



    }
    

}


void createConfigFile() {

    std::ofstream configFile(CONFIG_FILENAME);

    fillOutConfigFile(configFile);

    configFile.close();

}


void resolveConfigLine(ToolConfig& toolConfig, const string& line, int it) {

    int startingPoint = 0;


    startingPoint = CONFIGPROPERTIES[it].length() + 1;

    int whereEnds = line.find_last_of("\n");

    string configProperty = line.substr(startingPoint, whereEnds - 1);


    string key = CONFIGPROPERTIES[it];
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
    configFile.close();
    return toolConfig;


}


void copyConfigToOldFile() {

     std::ifstream fileCurrent(CONFIG_FILENAME);

     std::ofstream fileCopy(COPY_FILENAME);

     std::string line;
     while (std::getline(fileCurrent,line)) {

         fileCopy << line << "\n";

     }
     fileCurrent.close();
     fileCopy.close();

}

void rewriteConfigFile(ToolConfig& config) {


    std::ofstream configFile(CONFIG_FILENAME);

    string line;


    for (auto& property : CONFIGPROPERTIES) {

        configFile << property <<": "<< config[property]<<"\n";


    }
    configFile.close();



}

/*
- Reads config, if it exists and creates it if it doesnt.
- Loads database, and registers hotkeys also.
*/
ToolConfig initConfig() {


    if (!checkIfConfigFileExists()) {
        createConfigFile();
        successLog("Config file successfully created. Settings have been automatically generated.");

    }
    ToolConfig toolConfig = readConfigFile();


    return toolConfig;

}