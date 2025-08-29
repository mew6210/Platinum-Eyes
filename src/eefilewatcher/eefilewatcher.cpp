#define BUFFER_SIZE 1024
#include "efsw/efsw.hpp"
#include "eefilewatcher.hpp"
#include "wood.h"


const std::filesystem::path WATCHEDFILEPATH = "C:\\Users\\costam\\AppData\\Local\\Warframe";
const std::string FILENAME = "EE.log";

std::vector<std::string> loadFileContents(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}


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
            std::cout << "Modified " << filename << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            std::vector<std::string> newFileState = loadFileContents((WATCHEDFILEPATH / FILENAME).string());

            if (newFileState.size() < currentFileState.size()) {
                infoLog("File truncated or rotated, resetting state");
                currentFileState.clear();
            }

            int linesAdded = static_cast<int>(newFileState.size() - currentFileState.size());

            infoLog("----------------");
            infoLog("lines added: ", linesAdded);

            if (linesAdded > 0) {
                // iterate forward to preserve line order
                for (size_t i = currentFileState.size(); i < newFileState.size(); i++) {
                    const std::string& line = newFileState[i];
                    if (line.find("GetVoidProjectionRewards") != std::string::npos) {
                        successLog("Fissure rewards automatically detected");
                        state.eeLogTakeScreenshot.store(true, std::memory_order_relaxed);
                    }
                }
            }

            currentFileState = std::move(newFileState);
            infoLog("----------------");
        }
    }
};


int listenToEELog(AppState& state) {
    efsw::FileWatcher* fileWatcher = new efsw::FileWatcher(true);
    UpdateListener* listener = new UpdateListener(state);
    efsw::WatchID watchID3 = fileWatcher->addWatch(WATCHEDFILEPATH.string(), listener, false, {});
    fileWatcher->watch();

    return 0;
}