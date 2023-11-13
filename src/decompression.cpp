#include "decompression.h"
#include "binary_reader.h"
#include "symbols_constants.h"
#include "trie.h"
#include <vector>
#include "canonical_code.h"
#include <fstream>
#include <memory>

bool DecompressFile(BinaryReader& binary_reader) {
    uint16_t symbols_count = binary_reader.ReadBits9();
    std::vector<std::pair<uint16_t, uint16_t>> lens_symbols(symbols_count);
    for (uint16_t i = 0; i < symbols_count; ++i) {
        lens_symbols[i].second = binary_reader.ReadBits9();
    }
    uint16_t current_symbols_count = 0;
    for (uint16_t len = 1; current_symbols_count < symbols_count; ++len) {
        uint16_t len_symbols_count = binary_reader.ReadBits9();
        for (uint16_t i = current_symbols_count; i < current_symbols_count + len_symbols_count; ++i) {
            lens_symbols[i].first = len;
        }
        current_symbols_count += len_symbols_count;
    }

    Trie trie;

    CanonicalCode canonical_code(lens_symbols.front().first);

    for (size_t i = 0; i < lens_symbols.size(); ++i) {
        auto& [len, symbol] = lens_symbols[i];

        trie.Insert(canonical_code, symbol);

        if (i + 1 < lens_symbols.size()) {
            canonical_code.Increase();
            uint16_t next_len = lens_symbols[i + 1].first;
            canonical_code.Shift(next_len - len);
        }
    }

    std::string file_name;
    for (uint16_t symbol = trie.DecodeSymbol(binary_reader); symbol != FILENAME_END;
         symbol = trie.DecodeSymbol(binary_reader)) {
        file_name.push_back(static_cast<char>(symbol));
    }

    std::ofstream output_file(file_name, std::ios::binary);

    uint16_t symbol = trie.DecodeSymbol(binary_reader);
    while (symbol != ARCHIVE_END && symbol != ONE_MORE_FILE) {
        char symbol_to_write = static_cast<char>(symbol);
        output_file.write(&symbol_to_write, 1);
        symbol = trie.DecodeSymbol(binary_reader);
    }

    return symbol == ONE_MORE_FILE;
}

void DecompressFiles(const char* archive_name) {
    BinaryReader binary_reader(archive_name);

    while (DecompressFile(binary_reader)) {
    }
}