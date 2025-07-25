#include "native.hpp"


/*
	things that are native:
	-taking a screenshot
	-registering and unregistering keybinds
	-handling keybinds events
	-customizing windows in a specific way
	
*/

namespace {

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

}



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)


std::string VirtualKeyCodeToString(UCHAR virtualKey)
{
	UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

	CHAR szName[128];
	int result = 0;
	switch (virtualKey)
	{
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
	case VK_RCONTROL: case VK_RMENU:
	case VK_LWIN: case VK_RWIN: case VK_APPS:
	case VK_PRIOR: case VK_NEXT:
	case VK_END: case VK_HOME:
	case VK_INSERT: case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		scanCode |= KF_EXTENDED;
	default:
		result = GetKeyNameTextA(scanCode << 16, szName, 128);
	}
	if (result == 0)
		throw std::system_error(std::error_code(GetLastError(), std::system_category()),
			"WinAPI Error occured.");
	return szName;
}


int StringToVirtualKeyCode(std::string s) {

	for (auto& c : s) {
		c = std::tolower(c);
	}

	if (s == "esc") {
		return VK_ESCAPE;
	}

	HKL keyboard = LoadKeyboardLayoutA("00000409", KLF_ACTIVATE);        //primary english US keyboard layout

	return VkKeyScanEx(s[0], keyboard);
}


HRESULT setTransparency(HWND hWnd)
{
	HRESULT hr = S_OK;
	
	DWM_BLURBEHIND bb = { 0 };

	bb.dwFlags = DWM_BB_ENABLE;
	bb.fEnable = true;
	bb.hRgnBlur = NULL;

	hr = DwmEnableBlurBehindWindow(hWnd, &bb);
	return hr;
}



void listenAndHandleEvents(AppState& state) {


	if (PeekMessage(&state.msg, NULL, 0, 0, PM_REMOVE)) {

		handleEvents(state);
		TranslateMessage(&state.msg);
		DispatchMessage(&state.msg);
	}

}

void nativeWindowCustomization(AppState& state,sf::WindowHandle wHandle) {

	HWND hwnd = wHandle;
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	style |= WS_EX_TOOLWINDOW;
	SetWindowLongPtr(hwnd, GWL_EXSTYLE, style);
	setTransparency(hwnd);

}


void registerNativeHotkeys(std::map<int,KeyBind>& keyBindings) {

	for (auto& p : keyBindings) {

		if (RegisterHotKey(
			NULL,
			p.first,
			MOD_ALT | MOD_NOREPEAT,
			p.second.getKey()))
		{
			successLog("Successfully registered hotkey: Alt + ",VirtualKeyCodeToString(p.second.getKey())," for: ",p.second.getDescription());
		}
		else {
			warningLog("Failed to register hotkey: Alt + ",VirtualKeyCodeToString(p.second.getKey()) + " for: ",p.second.getDescription());

		}

	}



}

void unregisterNativeHotkeys(std::map<int, KeyBind>& keyBindings) {


	for (auto& p : keyBindings) {
		if (UnregisterHotKey(NULL, p.first)) {
			successLog("Succesfully unregistered Alt + " ,VirtualKeyCodeToString(p.second.getKey()));
		}
		else {
			errorLog(false,"Failed to unregister Alt + " ,VirtualKeyCodeToString(p.second.getKey()));
		}
	}



}


int getNativeScreenResolution() {


	HDC hScreen = GetDCEx(NULL, NULL, DCX_NORESETATTRS);
	int width = GetDeviceCaps(hScreen, HORZRES);
	int height = GetDeviceCaps(hScreen, VERTRES);
	DeleteDC(hScreen);
	int screenResolution = getScreenResolution(width, height);
	return screenResolution;

}

HBITMAP takeScreenshot(int imageWidth, int imageHeight, int offsetX, int offsetY) {


    HRGN constraints = CreateRectRgn(200, 200, 800, 800);

    const int CONSTRAINTX1 = 200;
    const int CONSTRAINTY1 = 200;
    const int CONSTRAINTX2 = 800;
    const int CONSTRAINTY2 = 800;




    HDC hScreen = GetDCEx(NULL, constraints, DCX_NORESETATTRS);
    HDC hTarget = CreateCompatibleDC(hScreen);

    int width = GetDeviceCaps(hScreen, HORZRES);
    int height = GetDeviceCaps(hScreen, VERTRES);

    //std::cout << "Width: " << width << std::endl;
    //std::cout << "\nHeight: " << height << std::endl;



    //int imageWidth = 1200;
    //int imageHeight = 800;



    //int offsetX = 1000;
    //int offsetY = 200;


    HBITMAP bitmap = CreateCompatibleBitmap(hScreen, imageWidth, imageHeight);
    HGDIOBJ oldbitmap = SelectObject(hTarget, bitmap);

    BitBlt(hTarget, 0, 0, imageWidth, imageHeight, hScreen, offsetX, offsetY, SRCCOPY);



    SelectObject(hScreen, oldbitmap);
    ReleaseDC(NULL, hTarget);
    DeleteDC(hScreen);

    return bitmap;

}

HBITMAP takeScreenshot(int imageWidth, int imageHeight, Point p)
{

    HRGN constraints = CreateRectRgn(200, 200, 800, 800);

    const int CONSTRAINTX1 = 200;
    const int CONSTRAINTY1 = 200;
    const int CONSTRAINTX2 = 800;
    const int CONSTRAINTY2 = 800;


    HDC hScreen = GetDCEx(NULL, constraints, DCX_NORESETATTRS);
    HDC hTarget = CreateCompatibleDC(hScreen);

    int width = GetDeviceCaps(hScreen, HORZRES);
    int height = GetDeviceCaps(hScreen, VERTRES);



    //int imageWidth = 1200;
    //int imageHeight = 800;



    //int offsetX = 1000;
    //int offsetY = 200;

    int leftCornerX = p.x - imageWidth / 2;
    int leftCornerY = p.y - imageHeight / 2;


    HBITMAP bitmap = CreateCompatibleBitmap(hScreen, imageWidth, imageHeight);
    HGDIOBJ oldbitmap = SelectObject(hTarget, bitmap);

    BitBlt(hTarget, 0, 0, imageWidth, imageHeight, hScreen, leftCornerX, leftCornerY, SRCCOPY);



    SelectObject(hScreen, oldbitmap);
    ReleaseDC(NULL, hTarget);
    DeleteDC(hScreen);
    return bitmap;

}


BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
    HDC hDC;
    int iBits;
    WORD wBitCount;
    DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
    BITMAP Bitmap0;
    BITMAPFILEHEADER bmfHdr;
    BITMAPINFOHEADER bi;
    LPBITMAPINFOHEADER lpbi;
    HANDLE fh, hDib, hPal, hOldPal2 = NULL;
    hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);
    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else
        wBitCount = 24;
    GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap0.bmWidth;
    bi.biHeight = -Bitmap0.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 256;
    dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8
        * Bitmap0.bmHeight;
    hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = GetDC(NULL);
        hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }


    GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
        + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

    if (hOldPal2)
    {
        SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
    }

    fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    bmfHdr.bfType = 0x4D42; // "BM"
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);
    return TRUE;
}

void saveScreenshotToClipboard(HBITMAP bitmap) {
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, bitmap);
    CloseClipboard();
}

//chatgpt cooked here
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

void handleNativeEvents(AppState& state){
if (state.msg.message == WM_HOTKEY) {
        
        try {
            KeyBind keybind = keyBindings.at(state.msg.wParam);
            std::cout << "Alt + " << VirtualKeyCodeToString(keybind.getKey())<<"\n";
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
            state.window.setVisible(state.isVisible);

            if (state.isVisible) {
                state.window.setFramerateLimit(state.fpsVisible);
            }
            else {
                state.window.setFramerateLimit(state.fpsHidden);
            }

        }
            break;
        case KB_BackupConfig: copyConfigToOldFile(); break;
        case KB_ExampleItems: {
            if (state.itemDisplayFlag == ITEMTYPE_fissureItems) {
                state.items = exampleItems;
            }
            else if (state.itemDisplayFlag == ITEMTYPE_relicItems) {
                state.currentRelic= FetchRelicItemPrices("Lith A1 Relic (Intact)");
            }
           


        }
            break;
        case KB_ReadRelicTitle: {

            state.currentRelic=readItemsFromRelicTitleTesseract(state.tesseractApi);



            if (state.currentRelic.relicPrice == 0.0) {
                warningLog("No relic name found in its designated area, shifting to right.(Maybe user is in a mission)");
                state.currentRelic = readItemsFromRelicTitleTesseractShifted(state.tesseractApi);

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



void listenAndHandleEvents(){
	errorLog(true,"currently not implemented");

}
void nativeWindowCustomization(AppState& state,sf::WindowHandle wHandle) {
	errorLog(true,"currently not implemented");

}
void registerNativeHotkeys() {
	errorLog(true,"currently not implemented");

}
void unregisterNativeHotkeys() {
	errorLog(true,"currently not implemented");

}
void takeScreenshot(){
    errorLog(true,"currently not implemented");

}
void takeScreenshot(){
    errorLog(true,"currently not implemented");

}
void SaveHBITMAPToFile(){
    errorLog(true,"currently not implemented");

}
void saveScreenshotToClipboard(){
    errorLog(true,"currently not implemented");

}
void loadStringToMemory(){
    errorLog(true,"currently not implemented");

}
void saveItemsToClipboard(){
    errorLog(true, "currently not implemented");

}
void handleNativeEvents(AppState& state){
    errorLog(true, "currently not implemented");
}
int getNativeScreenResolution(){

    return 1000;
    errorLog(false,"currently not implemented");
}










#endif