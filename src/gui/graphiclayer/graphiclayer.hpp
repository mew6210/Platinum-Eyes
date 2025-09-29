#include <SFML/Graphics/RenderWindow.hpp>
#include "../../utilities/utilities.h"


struct GraphicLayer {
	std::unique_ptr<sf::RenderWindow> window;
	bool isVisible;
	WindowParameters sfmlSize;	
	WindowParameters imguiSize;	
	bool settingsVisible;	
	bool shouldResizeImGui;
	bool shouldUpdateFonts;
	int fpsVisible;
	int fpsHidden;
};