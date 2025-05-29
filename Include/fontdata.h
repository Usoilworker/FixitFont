// fontdata.h  Structure for font storage memory
#ifndef FONTDATA_H
#define FONTDATA_H

#include <array>
#include <bitset>

struct FontData {
    static const int NUM_CHARACTERS = 256;
    static const int PATTERN_SIZE_BYTES = 8; // 8 bytes for 8x8 bits
    static const int PATTERN_ROWS = 8;             // <--- ADD THIS LINE
    static const int PATTERN_BITS_PER_ROW = 8;

   // std::array<std::bitset<8>, NUM_CHARACTERS> characterPatterns;
    std::array<std::array<std::bitset<PATTERN_BITS_PER_ROW>, PATTERN_ROWS>, NUM_CHARACTERS> characterPatterns;
    FontData(); // Constructor to initialize the array
};

#endif