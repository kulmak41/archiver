#pragma once

#include "canonical_code.h"
#include <cstdint>
#include <memory>
#include "binary_reader.h"

class Trie {
public:
    Trie();
    explicit Trie(const std::vector<size_t>& frequency);

    std::vector<std::pair<uint16_t, uint16_t>> FindSymbolsCodesLens();

    void Insert(const CanonicalCode& code, uint16_t symbol);

    uint16_t DecodeSymbol(BinaryReader& binary_reader);

private:
    struct Node {
        uint16_t symbol;
        size_t frequency;
        std::shared_ptr<Node> children[2];

        Node();
        explicit Node(uint16_t symbol);
        Node(uint16_t min_character, size_t frequency);
        Node(std::shared_ptr<Node> first, std::shared_ptr<Node> second);

        bool IsLeave();
    };

    struct CompareNodePointers {
        bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const;
    };

    std::shared_ptr<Node> root_;

    void FindSymbolsCodesLens(std::shared_ptr<Node> node, uint16_t len,
                              std::vector<std::pair<uint16_t, uint16_t>>& lens_symbols);
};