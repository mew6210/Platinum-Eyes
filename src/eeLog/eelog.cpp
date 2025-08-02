#include "eelog.hpp"


const std::filesystem::path WATCHEDFILEPATH = "C:\\Users\\costam\\AppData\\Local\\Warframe";
const std::string FILENAME = "EE.log";

std::vector<std::string> loadFileContents(const std::string& path) {

    std::vector<std::string> lines = {};

    std::ifstream file(path);
    std::string line = "";
    while (getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

void DoOtherWork() {
    while (true) {
        std::cout << "Doing other work..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}


class UpdateListener : public efsw::FileWatchListener {

public:


    std::vector<std::string> currentFileState = loadFileContents((WATCHEDFILEPATH / FILENAME).string());
    int changes = 0;


    void handleFileAction(efsw::WatchID watchid,
        const std::string& dir,
        const std::string& filename,
        efsw::Action action,
        std::string oldFilename) override {

        std::vector<std::string> newFileState;
        switch (action) {
        case efsw::Actions::Modified:
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            newFileState = loadFileContents((WATCHEDFILEPATH / FILENAME).string());

            int linesAdded = newFileState.size() - currentFileState.size();


            for (int i = 0; i < linesAdded; i++) {

                std::string line = newFileState[(newFileState.size() - 1) - i];
                if (line.find("GetVoidProjectionRewards") != std::string::npos) {
                    successLog("TAKE SCREENSHOT NOW");
                }

            }
            currentFileState = newFileState;
            
            changes++;




            break;


        }
    }

    
};

bool areChangesAvalible(UpdateListener* listener) {

    if (listener->changes != 0) {
        listener->changes = 0;
        return true;
    }
    else return false;

}


void listenToEELog(AppState& state) {

    efsw::FileWatcher* fileWatcher = new efsw::FileWatcher(true);
    UpdateListener* listener = new UpdateListener();

    efsw::WatchID watchID3 = fileWatcher->addWatch(WATCHEDFILEPATH.string(), listener, false, {});
    fileWatcher->watch();

    while (true) {
        areChangesAvalible(listener);
    }


}