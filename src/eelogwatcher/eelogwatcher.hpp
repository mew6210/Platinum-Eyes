#pragma once
#include "../core/core.h"

#include <filesystem>
#include <chrono>
#include <thread>
#include <fstream>
#include "efsw/efsw.hpp"


const std::filesystem::path WATCHEDFILEPATH = "C:\\Users\\costam\\AppData\\Local\\Warframe";
const std::string FILENAME = "EE.log";

std::vector<std::string> loadFileContents(const std::string& path);

class UpdateListener : public efsw::FileWatchListener {
public:
    std::vector<std::string> currentFileState;
    AppState& state;

    UpdateListener(AppState& c_state)
        : currentFileState(loadFileContents((WATCHEDFILEPATH / FILENAME).string())),
        state(c_state) {
    }

    void handleFileAction(efsw::WatchID watchid,
        const std::string& dir,
        const std::string& filename,
        efsw::Action action,
        std::string oldFilename) override {
        if (action == efsw::Actions::Modified) {

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            std::vector<std::string> newFileState = loadFileContents((WATCHEDFILEPATH / FILENAME).string());

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
                        state.eeLogTakeScreenshot.store(true, std::memory_order_relaxed);
                    }
                }
            }
            currentFileState = std::move(newFileState);
        }
    }
};

class EELogWatcher {
public:
    UpdateListener listener;
    efsw::FileWatcher fileWatcher{ true };
    EELogWatcher(AppState& state) :listener(state) {}
};

EELogWatcher listenToEELog(AppState& state);