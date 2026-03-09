#include "../../core/core.h"

#include "efsw/efsw.hpp"
#include <string>
#include <vector>
#include <filesystem>

class UpdateListener : public efsw::FileWatchListener {     //only system
public:
    std::vector<std::string> currentFileState;
    AppState& state;
    bool shouldTakeScreenshot;
    std::filesystem::path eeLogPath;
    std::filesystem::path eeLogFilePath;

    UpdateListener(AppState& c_state);

    void handleFileAction(efsw::WatchID watchid,
        const std::string& dir,
        const std::string& filename,
        efsw::Action action,
        std::string oldFilename) override;
};