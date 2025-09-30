#include "guinative.hpp"



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)



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



void nativeWindowCustomization(sf::WindowHandle wHandle) {

	HWND hwnd = wHandle;
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	style |= WS_EX_TOOLWINDOW;
	SetWindowLongPtr(hwnd, GWL_EXSTYLE, style);
	setTransparency(hwnd);

}


#endif

#if __linux__

void nativeWindowCustomization(sf::WindowHandle wHandle) {
	errorLog(true, "currently not implemented");

}


#endif
