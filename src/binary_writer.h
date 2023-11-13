#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

class BinaryWriter {
public:
    explicit BinaryWriter(const char* file_name);

    void WriteBit(uint8_t bit);

    void WriteBits9(uint16_t bits);

    void WriteBitsSequence(const std::vector<uint8_t>& seq);

    ~BinaryWriter();

private:
    std::ofstream output_file_;
    char current_byte_;
    uint8_t current_byte_pos_;
};