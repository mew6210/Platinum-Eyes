

struct SystemLayer {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	MSG msg;
#endif

#if __linux__
	XEvent msg;		
#endif
	bool running;
	std::atomic<bool> eeLogTakeScreenshot = false;


	SystemLayer(decltype(msg) m, bool r)
		: msg(m), running(r), eeLogTakeScreenshot(false) {
	}

};