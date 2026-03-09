#pragma once
#include "../core/core.h"
#include "updatelistener/updatelistener.hpp"

#include "efsw/efsw.hpp"

class EELogWatcher {
public:
    UpdateListener listener;
    efsw::FileWatcher fileWatcher{ true };
    EELogWatcher(AppState& state) :listener(state) {}
};

EELogWatcher listenToEELog(AppState& state);