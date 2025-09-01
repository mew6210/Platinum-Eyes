#define BUFFER_SIZE 1024

#include "eelogwatcher.hpp"
#include "wood.h"


std::vector<std::string> loadFileContents(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

void testEELogPath(const std::filesystem::path& eeLogFilePath) {
    std::ifstream testingFile(eeLogFilePath);
    if (!testingFile.is_open()) {
        warningLog("could not open ee.log at stated path");
    }
    else {
        successLog("EE.log found successfully");
    }
    testingFile.close();
}

UpdateListener::UpdateListener(AppState& c_state) : state(c_state) {
    eeLogPath = state.config["eeLogPath"];
    eeLogFilePath = eeLogPath / "EE.log";

    testEELogPath(eeLogFilePath);

    if (state.config["eeLogShouldTakeScreenshot"] == "yes") {
        shouldTakeScreenshot = true;
    }
    else {
        shouldTakeScreenshot = false;
    }
    currentFileState = loadFileContents((eeLogFilePath).string());
}


EELogWatcher listenToEELog(AppState& state) {

    EELogWatcher eelw(state);
    eelw.fileWatcher.addWatch(eelw.listener.eeLogPath.string(), &eelw.listener, false, {});
    eelw.fileWatcher.watch();

    return eelw;
}