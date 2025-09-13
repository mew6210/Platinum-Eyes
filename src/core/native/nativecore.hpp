#pragma once
#include "../core.h"
#include "../../keybindings/keybind/keybind.hpp"

const std::vector<Item> exampleItems = {
	Item("Mesa Prime Neuroptics","Mesa Prime Neuroptics",ItemDetails(5,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("Blah blah blah mesa prime neuroptics","Blah blah blah mesa prime neuroptics",ItemDetails(6,std::vector<int>({1,1,1,1,1}),Rarity::level::Uncommon)),
	Item("someItemName3","someItemName3",ItemDetails(7,std::vector<int>({1,1,1,1,1}),Rarity::level::Rare)),
	Item("someItemName4","someItemName4",ItemDetails(8,std::vector<int>({1,1,1,1,1}),Rarity::level::Undefined)),
	Item("someItemName5","someItemName5",ItemDetails(9,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName6","someItemName6",ItemDetails(10,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName7","someItemName7",ItemDetails(11,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName8","someItemName8",ItemDetails(12,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName9","someItemName9",ItemDetails(13,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName10","someItemName10",ItemDetails(14,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName11","someItemName11",ItemDetails(15,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName12","someItemName12",ItemDetails(16,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName13","someItemName13",ItemDetails(17,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName14","someItemName14",ItemDetails(18,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName15","someItemName15",ItemDetails(19,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
	Item("someItemName16","someItemName16",ItemDetails(20,std::vector<int>({1,1,1,1,1}),Rarity::level::Common)),
};




#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

void listenAndHandleEvents(AppState& state);
void handleNativeEvents(AppState& state, std::map<int, KeyBind> keybindings);

#endif




#if __linux__


void listenAndHandleEvents(AppState& state);
void handleNativeEvents(AppState& state, std::map<int, KeyBind> keybindings);


#endif
