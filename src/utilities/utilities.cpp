#include "utilities.h"
#include <lzma.h>
#include <stdexcept>

using std::vector, std::string;

std::vector<int> stringToFormattedLowestPrices(const std::string& s) {
    std::vector<int> prices;

    if (s.size() < 2 || s.front() != '(' || s.back() != ')') {
        return prices; // invalid format, return empty
    }

    std::string content = s.substr(1, s.size() - 2); // remove '(' and ')'
    std::stringstream ss(content);
    std::string token;

    while (std::getline(ss, token, ',')) {
        if (!token.empty()) {
            prices.push_back(std::stoi(token));
        }
    }

    return prices;
}


std::string getFormatedLowestPrices(const std::vector<int>& lowestPrices) {

    std::string s;
    s.append("(");
    for (int i = 0; i < lowestPrices.size(); i++) {
        if (i != lowestPrices.size() - 1) {
            s.append(std::to_string(lowestPrices[i]) + ",");
        }
        else {
            s.append(std::to_string(lowestPrices[i]));
        }


    }
    s.append(")");

    return s;
}


Rarity::level rarityFromString(const std::string& s) {
    if (s == "Common")   return Rarity::level::Common;
    if (s == "Uncommon") return Rarity::level::Uncommon;
    if (s == "Rare")     return Rarity::level::Rare;
    return Rarity::level::Undefined;
}

std::string rarityToString(Rarity::level r) {
    switch (r) {
    case Rarity::level::Common:return "Common";
    case Rarity::level::Uncommon:return "Uncommon";
    case Rarity::level::Rare:return "Rare";
    case Rarity::level::Undefined:return "Undefined";
    default: return "Should never happen";
    }
}

void replaceAnds(string& s) {

    size_t pos = s.find("&");

    if (pos != string::npos)
        s.replace(pos, 1, "and");
}

string replaceChar(string s, char a, string b) {

    string newstring;


    for (char c : s) {

        if (c == a) {
            newstring.append(b);
        }
        else {
            newstring.append(1, c);
        }


    }

    return newstring;

}

string itemToSnakeCase(const string& s) {
    string snakedItem = replaceChar(s, ' ', "_");
    replaceAnds(snakedItem);
    for (auto& x : snakedItem) {
        x = tolower(x);
    }
    return snakedItem;
}

vector<string> itemsToSnakeCase(const vector<string>& list) {

    vector<string> snakedItems;
    for (const string& item : list) {
        string snakedItem = itemToSnakeCase(item);
        snakedItems.push_back(snakedItem);
    }
    return snakedItems;
}

string snakeToItem(const string& s) {
    string result = s;

    for (auto& ch : result) {
        if (ch == '_') ch = ' ';
    }

    size_t pos = result.find(" and ");
    while (pos != string::npos) {
        result.replace(pos, 5, " & ");
        pos = result.find(" and ", pos + 3);
    }

    bool capitalizeNext = true;
    for (auto& ch : result) {
        if (capitalizeNext && isalpha(ch)) {
            ch = toupper(ch);
            capitalizeNext = false;
        }
        else {
            ch = tolower(ch);
        }
        if (ch == ' ') capitalizeNext = true;
    }

    return result;
}

std::pair<int,int> stringToIntPair(std::string s) {

    size_t middlePos = s.find(",");

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