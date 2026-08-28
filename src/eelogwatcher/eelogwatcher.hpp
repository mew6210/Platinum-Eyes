#pragma once
#include "../core/core.h"
#include "updatelistener/updatelistener.hpp"

#include "efsw/efsw.hpp"

class EELogWatcher {
    UpdateListener listener;
    efsw::FileWatcher fileWatcher{ true };
    EELogWatcher(AppState& state) :listener(state) {}

    friend EELogWatcher listenToEELog(AppState& state);
};

EELogWatcher listenToEELog(AppState& state);