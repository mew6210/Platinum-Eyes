#include "../datalayer/datalayer.hpp"
#include "../../config/toolconfig.hpp"
#include "../../gui/graphiclayer/graphiclayer.hpp"
#include "../../system/systemlayer/systemlayer.hpp"
#include "../../ocr/ocrlayer/ocrlayer.hpp"

struct AppState {
	DataLayer data;
	ToolConfig config;
	GraphicLayer gui;
	std::unique_ptr<SystemLayer> system;
	OcrLayer ocr;

	AppState(DataLayer d, ToolConfig c, GraphicLayer g, std::unique_ptr<SystemLayer> s, OcrLayer o)
		: data(std::move(d))
		, config(std::move(c))
		, gui(std::move(g))
		, system(std::move(s))
		, ocr(std::move(o)) {
	}
};