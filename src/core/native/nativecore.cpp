#include "nativecore.hpp"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)


void listenAndHandleEvents(AppState& state) {


    if (PeekMessage(&state.msg, NULL, 0, 0, PM_REMOVE)) {

        handleEvents(state);
        TranslateMessage(&state.msg);
        DispatchMessage(&state.msg);
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
    if (state.msg.message == WM_HOTKEY) {

        try {
            KeyBind keybind = keyBindings.at(state.msg.wParam);
            std::cout << "Alt + " << VirtualKeyCodeToString(keybind.getKey()) << "\n";
        }
        catch (std::out_of_range e) {
            std::cout << "didnt find this hotkey :3 \n";    //should never happen btw
        }

        //keybind logic
        switch (state.msg.wParam) {

        case KB_ReadItemsFromScreen:
        {
            state.items = readFissureRewardsScreen(state);
            state.itemDisplayFlag = ITEMTYPE_fissureItems;
        }
        break;
        case KB_EscapeProgram: state.running = false;
            break;
        case KB_ReadPreviousItems: {
            state.items = readPreviousFissureRewardsScreen(state);
            state.itemDisplayFlag = ITEMTYPE_fissureItems;

        }
                                 break;
        case KB_WindowVisibility:
        {
            state.isVisible = !state.isVisible;
            state.window->setVisible(state.isVisible);

            if (state.isVisible) {
                state.window->setFramerateLimit(state.fpsVisible);
            }
            else {
                state.window->setFramerateLimit(state.fpsHidden);
            }

        }
        break;
        case KB_BackupConfig: copyConfigToOldFile(); break;
        case KB_ExampleItems: {
            if (state.itemDisplayFlag == ITEMTYPE_fissureItems) {
                state.items = exampleItems;
            }
            else if (state.itemDisplayFlag == ITEMTYPE_relicItems) {
                state.currentRelic = fetchRelicItemPrices("Lith A1 Relic (Intact)");
            }



        }
                            break;
        case KB_ReadRelicTitle: {

            state.currentRelic = readItemsFromRelicTitleTesseract(*state.tesseractApi);



            if (state.currentRelic.relicPrice == 0.0) {
                warningLog("No relic name found in its designated area, shifting to right.(Maybe user is in a mission)");
                state.currentRelic = readItemsFromRelicTitleTesseractShifted(*state.tesseractApi);

            }
            printRelic(state.currentRelic);


            state.itemDisplayFlag = ITEMTYPE_relicItems;


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
