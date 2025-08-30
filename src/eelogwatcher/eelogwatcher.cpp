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


EELogWatcher listenToEELog(AppState& state) {

    EELogWatcher eelw(state);
    eelw.fileWatcher.addWatch(WATCHEDFILEPATH.string(), &eelw.listener, false, {});

    eelw.fileWatcher.watch();

    return eelw;
}