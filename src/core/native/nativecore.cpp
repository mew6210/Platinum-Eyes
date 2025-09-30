#include "nativecore.hpp"
#include "../../items/itemcache/itemcache.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)


void listenAndHandleEvents(AppState& state) {

    if (state.system->eeLogTakeScreenshot.load(std::memory_order_relaxed)) {
        state.data.items = readFissureRewardsScreen(state);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        state.data.itemDisplayMode = ItemDisplayMode::FissureDisplay;
        state.system->eeLogTakeScreenshot.store(false, std::memory_order_relaxed);
    }
    if (PeekMessage(&state.system->msg, NULL, 0, 0, PM_REMOVE)) {

        handleEvents(state);
        TranslateMessage(&state.system->msg);
        DispatchMessage(&state.system->msg);
    }
}



#define KB_ReadItemsFromScreen 1
#define KB_EscapeProgram 2
#define KB_ReadPreviousItems 3
#define KB_WindowVisibility 4
#define KB_BackupConfig 5
#define KB_ExampleItems 6
#define KB_ReadRelicTitle 7

void handleNativeEvents(AppState& state, std::map<int, KeyBind> keyBindings) {
    if (state.system->msg.message == WM_HOTKEY) {

        try {
            KeyBind keybind = keyBindings.at(state.system->msg.wParam);
            std::cout << "Alt + " << VirtualKeyCodeToString(keybind.getKey()) << "\n";
        }
        catch (std::out_of_range e) {
            std::cout << "didnt find this hotkey :3 \n";    //should never happen btw
        }

        //keybind logic
        switch (state.system->msg.wParam) {

        case KB_ReadItemsFromScreen:
        {
            state.data.items = readFissureRewardsScreen(state);
            state.data.itemDisplayMode = ItemDisplayMode::FissureDisplay;
        }
        break;
        case KB_EscapeProgram: state.system->running = false;
            break;
        case KB_ReadPreviousItems: {
            state.data.items = readPreviousFissureRewardsScreen(state);
            state.data.itemDisplayMode = ItemDisplayMode::FissureDisplay;

        }
                                 break;
        case KB_WindowVisibility:
        {
            state.gui.isVisible = !state.gui.isVisible;
            state.gui.window->setVisible(state.gui.isVisible);

            if (state.gui.isVisible) {
                state.gui.window->setFramerateLimit(state.gui.fpsVisible);
            }
            else {
                state.gui.window->setFramerateLimit(state.gui.fpsHidden);
            }

        }
        break;
        case KB_BackupConfig: copyConfigToOldFile(); break;
        case KB_ExampleItems: {
            switch (state.data.itemDisplayMode) {
            case ItemDisplayMode::StartingScreenDisplay:
                state.data.itemDisplayMode = ItemDisplayMode::FissureDisplay;
                state.data.items = exampleItems; break;
            case ItemDisplayMode::FissureDisplay: 
                state.data.items = exampleItems; break;
            case ItemDisplayMode::RelicDisplay: 
                state.data.currentRelic = fetchRelicItemPrices("Lith A1 Relic (Intact)",CacheOptions(false,"")); break;
            }
        }
                            break;
        case KB_ReadRelicTitle: {

            CacheOptions cacheOpt = CacheOptions(configStringToBool(state.config["shouldCache"]), state.config["cacheDuration"]);
            state.data.currentRelic = readItemsFromRelicTitleTesseract(*state.ocr.tesseractApi,cacheOpt);

            if (state.data.currentRelic.relicPrice == 0.0) {
                warningLog("No relic name found in its designated area, shifting to right.(Maybe user is in a mission)");
                state.data.currentRelic = readItemsFromRelicTitleTesseractShifted(*state.ocr.tesseractApi,cacheOpt);

            }
            printRelic(state.data.currentRelic);

            state.data.itemDisplayMode = ItemDisplayMode::RelicDisplay;

            break;
        }
        }



    }

}



#endif




#if __linux__


void listenAndHandleEvents(AppState& state) {
    errorLog(true, "currently not implemented");

}


void handleNativeEvents(AppState& state, std::map<int, KeyBind> keyBindings) {
    errorLog(true, "currently not implemented");
}


#endif
