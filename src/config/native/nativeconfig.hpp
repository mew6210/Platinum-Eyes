#include <filesystem>
#include "wood.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <ShlObj_core.h>
#include <Windows.h>
std::filesystem::path getNativeEELogPath();

#endif




#if __linux__
std::filesystem::path getNativeEELogPath();

#endif
