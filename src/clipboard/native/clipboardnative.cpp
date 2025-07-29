#include "clipboardnative.hpp"



std::string getClipboardItemText(AppState& state, std::vector<Item>& items) {

    std::string returnString = "";

    for (auto& item : items) {
        returnString += "[";
        std::string name = item.preparedName;

        if (!name.ends_with("Blueprint")) returnString += name;
        else returnString += name.substr(0, name.length() - 10);


        returnString += "] - ";

        returnString += getFormatedAveragePrices(item.itemDetails.lowestPrices);

        returnString += " ";

    }

    if (state.config["clipboardWatermark"] == "yes") returnString += " Lowest prices from warframe market provided by Platinum-Eyes on github";

    return returnString;
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

void saveScreenshotToClipboard(HBITMAP bitmap) {
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, bitmap);
    CloseClipboard();
}
HGLOBAL loadStringToMemory(std::string& text) {

    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hGlobal) return NULL;

    // Lock the memory and copy the text to it
    char* pGlobal = static_cast<char*>(GlobalLock(hGlobal));
    if (!pGlobal) {
        GlobalFree(hGlobal);
        return NULL;
    }
    memcpy(pGlobal, text.c_str(), text.size() + 1);
    GlobalUnlock(hGlobal);

    return hGlobal;

}
void saveItemsToClipboard(AppState& state, std::vector<Item>& items) {
    OpenClipboard(NULL);
    EmptyClipboard();

    std::string text = "";

    text = getClipboardItemText(state, items);

    HGLOBAL textInMemory = loadStringToMemory(text);


    SetClipboardData(CF_TEXT, textInMemory);
    CloseClipboard();

    successLog("Successfully copied prices to clipboard, avalible to paste them in the chat with ctrl + v");

}


#endif


#if __linux__


void saveScreenshotToClipboard() {
    errorLog(true, "currently not implemented");

}

void saveItemsToClipboard(AppState& state, std::vector<Item>& items) {
    errorLog(true, "currently not implemented");

}

#endif