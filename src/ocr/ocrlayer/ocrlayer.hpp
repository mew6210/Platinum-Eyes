#include <tesseract/baseapi.h>

struct OcrLayer {
	std::unique_ptr<tesseract::TessBaseAPI> tesseractApi;
};