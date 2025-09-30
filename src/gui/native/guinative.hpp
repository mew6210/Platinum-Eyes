#include "../../core/core.h"



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	void nativeWindowCustomization(sf::WindowHandle wHandle);

#endif

#if __linux__
	void nativeWindowCustomization(sf::WindowHandle wHandle);
#endif