#pragma once
#include "../core/core.h"



std::pair<int, int>stringToIntPair(std::string s);




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