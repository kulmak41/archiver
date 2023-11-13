#include "binary_reader.h"
#include "file_existence_exception.h"

BinaryReader::BinaryReader(const char *file_name)
    : input_file_(file_name, std::ios::binary), current_byte_(0), current_byte_pos_(8) {
    if (!input_file_.good()) {
        throw FileExistenceException(file_name);
    }
}

uint8_t BinaryReader::ReadBit() {
    if (current_byte_pos_ == 8) {
        input_file_.read(&current_byte_, 1);
        current_byte_pos_ = 0;
    }
    uint8_t bit = (current_byte_ >> (7 - current_byte_pos_)) & 1;
    ++current_byte_pos_;
    return bit;
}
uint16_t BinaryReader::ReadBits9() {
    uint16_t val = 0;
    for (int i = 8; i >= 0; --i) {
        val |= (ReadBit() << i);
    }
    return val;
}
