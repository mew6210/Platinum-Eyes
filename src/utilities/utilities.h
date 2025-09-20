#pragma once
#include "../core/core.h"
#include <vector>
#include <string>
#include <utility>

std::vector<int> stringToFormattedLowestPrices(const std::string& s);
std::string getFormatedLowestPrices(const std::vector<int>& lowestPrices);
std::pair<int, int>stringToIntPair(std::string s);
Rarity::level rarityFromString(const std::string& s);

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