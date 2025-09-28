#pragma once
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include "../items/itemdetails/itemdetails.hpp"

std::vector<int> stringToFormattedLowestPrices(const std::string& s);
std::string getFormatedLowestPrices(const std::vector<int>& lowestPrices);
std::pair<int, int>stringToIntPair(std::string s);
Rarity::level rarityFromString(const std::string& s);


std::string rarityToString(Rarity::level r);


struct WindowParameters {
    int width = 0;
    int height = 0;

    WindowParameters(int width, int height) :width(width), height(height) {};
    WindowParameters() {};
};


enum class ItemDisplayMode {
    FissureDisplay = 1,
    RelicDisplay = 2,
    StartingScreenDisplay = 3
};


void replaceAnds(std::string& s);
std::string replaceChar(std::string s, char a, std::string b);
std::string itemToSnakeCase(const std::string& s);
std::vector<std::string> itemsToSnakeCase(const std::vector<std::string>& list);
std::string snakeToItem(const std::string& s);






/*
    example usage:
    if (decompress_lzma("index_en.lzma", "data.txt")) {
        printf("Decompression successful.\n");
    }
    else {
        fprintf(stderr, "Decompression failed.\n");
    }

*/
bool decompress_lzma(const char* in_filename, const char* out_filename);