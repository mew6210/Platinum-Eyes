#include "nativeconfig.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
std::filesystem::path getNativeEELogPath(){
	PWSTR pathTmp = nullptr;
	HRESULT result =  SHGetKnownFolderPath(FOLDERID_LocalAppData,0,NULL,&pathTmp);
	if (FAILED(result) || pathTmp == nullptr) {
		errorLog(false,"failed to get LocalAppData");
		return "";
	}
	std::filesystem::path resultPath(pathTmp);
	CoTaskMemFree(pathTmp);
	return resultPath / "Warframe";
}

#endif




#if __linux__
std::filesystem::path getNativeEELogPath() {
	return "";
}

#endif
