#include "utilities.h"
#include <lzma.h>
#include <termcolor/termcolor.hpp> //coloring ui single header library. https://github.com/ikalnytskyi/termcolor



void errorLog(const std::string& s) {
    std::cout << "[" << termcolor::red << "-" << termcolor::reset << "] " << s << std::endl;

}

void warningLog(const std::string& s) {
    std::cout << "[" << termcolor::yellow << "?" << termcolor::reset << "] " << s << std::endl;

}

void successLog(const std::string& s) {

    std::cout << "[" << termcolor::green << "+" << termcolor::reset << "] " << s << std::endl;

}


void myAssert(bool stmt,std::string failDescription) {

    if (!stmt) {
        std::cout << "Assertion error: ";
        std::cout << failDescription << std::endl;
        throw ASSERTION_ERROR;
    }
    else {
        return;
    }


}

std::pair<int,int> stringToIntPair(std::string s) {

    int middlePos = s.find(",");

    std::string x = s.substr(0, middlePos);
    std::string y = s.substr(middlePos + 1, s.length() - x.length());




    return std::pair(std::stoi(x), std::stoi(y));

}




bool decompress_lzma(const char* in_filename, const char* out_filename) {
    FILE* in = fopen(in_filename, "rb");
    FILE* out = fopen(out_filename, "wb");
    if (!in || !out) {
        perror("File open error");
        return false;
    }

    lzma_stream strm = LZMA_STREAM_INIT;
    lzma_ret ret = lzma_alone_decoder(&strm, UINT64_MAX);
    if (ret != LZMA_OK) {
        fprintf(stderr, "lzma_stream_decoder init failed\n");
        return false;
    }

    const size_t BUF_SIZE = 8192;
    uint8_t in_buf[BUF_SIZE];
    uint8_t out_buf[BUF_SIZE];

    strm.avail_in = 0;
    strm.next_in = NULL;

    bool success = true;

    while (true) {
        if (strm.avail_in == 0) {
            strm.next_in = in_buf;
            strm.avail_in = fread(in_buf, 1, BUF_SIZE, in);
            if (ferror(in)) {
                perror("fread failed");
                success = false;
                break;
            }
        }

        strm.next_out = out_buf;
        strm.avail_out = BUF_SIZE;

        ret = lzma_code(&strm, LZMA_RUN);

        if (ret != LZMA_OK && ret != LZMA_STREAM_END) {
            fprintf(stderr, "Decompression error: %d\n", ret);
            success = false;
            break;
        }

        size_t write_size = BUF_SIZE - strm.avail_out;
        if (fwrite(out_buf, 1, write_size, out) != write_size) {
            perror("fwrite failed");
            success = false;
            break;
        }

        if (ret == LZMA_STREAM_END) break;
    }

    lzma_end(&strm);
    fclose(in);
    fclose(out);

    return success;
}