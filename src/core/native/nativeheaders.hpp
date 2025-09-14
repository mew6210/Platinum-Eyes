#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#include <dwmapi.h>
#include <ShlObj_core.h>
#endif


#if __linux__
#include <X11/Xlib.h>
#ifdef None
#undef None
#endif
#ifdef Status
#undef Status
#endif
#endif
