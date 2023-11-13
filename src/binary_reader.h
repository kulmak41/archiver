#pragma once

#include <fstream>
#include <cstdint>

class BinaryReader {
public:
    explicit BinaryReader(const char *file_name);
    uint8_t ReadBit();
    uint16_t ReadBits9();

private:
    std::ifstream input_file_;
    char current_byte_;
    int current_byte_pos_;
};