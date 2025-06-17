#include "ocr.h"



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



std::string getClipboardItemText(std::vector<Item>& items) {

    std::string returnString = "";
    
    for (auto& item : items) {
        returnString += "[";
        std::string name = item.preparedName;

        if (!name.ends_with("Blueprint")) returnString += name;
        else returnString += name.substr(0, name.length() - 10);


        returnString += "] - ";

        returnString +=getFormatedAveragePrices(item.itemDetails.lowestPrices);

        returnString += " ";

    }

    returnString += " Lowest prices from warframe market provided by Platinum-Eyes on github";

    return returnString;
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



void saveItemsToClipboard(std::vector<Item>& items) {
    OpenClipboard(NULL);
    EmptyClipboard();

    std::string text = "";
    
    text = getClipboardItemText(items);

    HGLOBAL textInMemory = loadStringToMemory(text);


    SetClipboardData(CF_TEXT,textInMemory);
    CloseClipboard();

    successLog("Successfully copied prices to clipboard, avalible to paste them in the chat with ctrl + v");

}