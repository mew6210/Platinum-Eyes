#include "eelogwatcher.hpp"

EELogWatcher listenToEELog(AppState& state) {

    EELogWatcher eelw(state);
    eelw.fileWatcher.addWatch(eelw.listener.eeLogPath.string(), &eelw.listener, false, {});
    eelw.fileWatcher.watch();

    return eelw;
}