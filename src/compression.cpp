#include "compression.h"
#include "binary_reader.h"
#include "binary_writer.h"
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include "symbols_constants.h"
#include "trie.h"
#include "canonical_code.h"
#include <memory>
#include "file_existence_exception.h"

std::vector<size_t> FindFrequencies(char* file_name) {
    std::vector<size_t> frequency(MAX_SYMBOL);
    frequency[FILENAME_END] = frequency[ONE_MORE_FILE] = frequency[ARCHIVE_END] = 1;
    for (size_t i = 0; file_name[i] != '\0'; ++i) {
        ++frequency[file_name[i]];
    }

    std::ifstream file(file_name);
    if (!file.good()) {
        throw FileExistenceException(file_name);
    }
    char symbol = 0;
    while (file.read(&symbol, 1)) {
        ++frequency[static_cast<unsigned char>(symbol)];
    }
    return frequency;
}

void FindCanonicalCodes(std::vector<std::pair<uint16_t, uint16_t>>& lens_symbols,
                        std::vector<CanonicalCode>& symbol_code, std::vector<uint16_t>& len_codes_count) {
    CanonicalCode canonical_code(lens_symbols.front().first);
    for (size_t i = 0; i < lens_symbols.size(); ++i) {
        auto& [len, symbol] = lens_symbols[i];
        symbol_code[symbol] = canonical_code;
        ++len_codes_count[len];
        if (i + 1 < lens_symbols.size()) {
            canonical_code.Increase();
            uint16_t next_len = lens_symbols[i + 1].first;
            canonical_code.Shift(next_len - len);
        }
    }
}

void EncodeSymbolsInformation(uint16_t symbols_count, const std::vector<std::pair<uint16_t, uint16_t>>& lens_symbols,
                              const std::vector<uint16_t>& len_codes_count, BinaryWriter& binary_writer) {
    binary_writer.WriteBits9(symbols_count);

    for (auto& [len, symbol] : lens_symbols) {
        binary_writer.WriteBits9(symbol);
    }

    for (size_t len = 1; len < len_codes_count.size(); ++len) {
        binary_writer.WriteBits9(len_codes_count[len]);
    }
}

void EncodeFile(const char* file_name, std::vector<CanonicalCode>& symbol_code, BinaryWriter& binary_writer) {
    for (size_t i = 0; file_name[i] != '\0'; ++i) {
        binary_writer.WriteBitsSequence(symbol_code[file_name[i]]);
    }

    binary_writer.WriteBitsSequence(symbol_code[FILENAME_END]);

    std::ifstream file(file_name);
    char symbol = 0;
    while (file.read(&symbol, 1)) {
        binary_writer.WriteBitsSequence(symbol_code[static_cast<unsigned char>(symbol)]);
    }
}

void CompressFile(char* file_name, BinaryWriter& binary_writer, bool file_is_last) {
    std::vector<size_t> frequency = FindFrequencies(file_name);
    uint16_t symbols_count = frequency.size() - std::count(frequency.begin(), frequency.end(), 0);

    Trie trie(frequency);

    std::vector<std::pair<uint16_t, uint16_t>> lens_symbols = trie.FindSymbolsCodesLens();
    std::sort(lens_symbols.begin(), lens_symbols.end());
    std::vector<CanonicalCode> symbol_code(MAX_SYMBOL);
    std::vector<uint16_t> len_codes_count(lens_symbols.back().first + 1);
    FindCanonicalCodes(lens_symbols, symbol_code, len_codes_count);

    EncodeSymbolsInformation(symbols_count, lens_symbols, len_codes_count, binary_writer);
    EncodeFile(file_name, symbol_code, binary_writer);
    if (file_is_last) {
        binary_writer.WriteBitsSequence(symbol_code[ARCHIVE_END]);
    } else {
        binary_writer.WriteBitsSequence(symbol_code[ONE_MORE_FILE]);
    }
}

void CompressFiles(char* archive_name, size_t files_count, char** files_names) {
    BinaryWriter binary_writer(archive_name);

    for (size_t i = 0; i < files_count; ++i) {
        CompressFile(files_names[i], binary_writer, i == files_count - 1);
    }
}