#include "binary_writer.h"
#include <cstdint>

BinaryWriter::BinaryWriter(const char* file_name)
    : output_file_(file_name, std::ios::binary), current_byte_(0), current_byte_pos_(0) {
}

BinaryWriter::~BinaryWriter() {
    if (current_byte_pos_ != 0) {
        output_file_.write(&current_byte_, 1);
    }
}

void BinaryWriter::WriteBit(uint8_t bit) {
    if (current_byte_pos_ == 8) {
        output_file_.write(&current_byte_, 1);
        current_byte_ = 0;
        current_byte_pos_ = 0;
    }
    current_byte_ = static_cast<char>(current_byte_ | (bit << (7 - current_byte_pos_)));
    ++current_byte_pos_;
}

void BinaryWriter::WriteBits9(uint16_t bits) {
    for (int i = 8; i >= 0; --i) {
        WriteBit((bits >> i) & 1);
    }
}

void BinaryWriter::WriteBitsSequence(const std::vector<uint8_t>& seq) {
    for (uint8_t bit : seq) {
        WriteBit(bit);
    }
}
