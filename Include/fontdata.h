// fontdata.h  Structure for font storage memory
#ifndef FONTDATA_H
#define FONTDATA_H

#include <array>
#include <bitset>

struct FontData {
    static const int NUM_CHARACTERS = 256;
    static const int PATTERN_SIZE_BYTES = 8; // 8 bytes for 8x8 bits

    std::array<std::bitset<8>, NUM_CHARACTERS> characterPatterns;

    FontData(); // Constructor to initialize the array
};

#endif