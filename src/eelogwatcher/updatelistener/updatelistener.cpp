#include "updatelistener.hpp"
#include <chrono>
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

void UpdateListener::handleFileAction(efsw::WatchID watchid,
    const std::string& dir,
    const std::string& filename,
    efsw::Action action,
    std::string oldFilename) {
    if (action == efsw::Actions::Modified) {

        if (!shouldTakeScreenshot) {
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::vector<std::string> newFileState = loadFileContents((eeLogFilePath).string());

        if (newFileState.size() < currentFileState.size()) {
            infoLog("File truncated or rotated, resetting state");
            currentFileState.clear();
        }

        int linesAdded = static_cast<int>(newFileState.size() - currentFileState.size());

        if (linesAdded > 0) {
            for (size_t i = currentFileState.size(); i < newFileState.size(); i++) {
                const std::string& line = newFileState[i];
                if (line.find("GetVoidProjectionRewards") != std::string::npos) {
                    successLog("Fissure rewards automatically detected, taking a screenshot");
                    state.system->eeLogTakeScreenshot.store(true, std::memory_order_relaxed);
                }
            }
        }
        currentFileState = std::move(newFileState);
    }
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