#include "../platinumEyes.h"




std::map<std::string, ItemDetails> readItemsFromScreen(AppState state) {

	myAssert(state.ocrType == OCR_tesseract || state.ocrType == OCR_easyocr, "Invalid ocr type, it should be either 'tesseract' or 'easyocr'");
	std::map<std::string, ItemDetails> items;


	if (state.ocrType == OCR_tesseract) {
		items = readItemsFromScreenTesseract(state.config, state.tesseractApi);
	}
	
	else {
		items = readItemsFromScreenEasyocr(state.config);
	}
	return items;
	
}

std::map<std::string, ItemDetails> readItemsFromScreenWithoutScreenShot(AppState state) {

	myAssert(state.ocrType == OCR_tesseract || state.ocrType == OCR_easyocr, "Invalid ocr type, it should be either 'tesseract' or 'easyocr'");
	std::map<std::string, ItemDetails> items;


	if (state.ocrType == OCR_tesseract) {
		items = readItemsFromScreenWithoutScreenShotTesseract(state.config, state.tesseractApi);
	}

	else {
		items = readItemsFromScreenWithoutScreenshotEasyocr(state.config);
	}
	return items;

}



