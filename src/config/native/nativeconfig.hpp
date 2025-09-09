#include <filesystem>
#include "wood.h"
#include "../../core/native/nativeheaders.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
std::filesystem::path getNativeEELogPath();

#endif




#if __linux__
std::filesystem::path getNativeEELogPath();

#endif
