#include "trie.h"
#include <memory>
#include <queue>

Trie::Node::Node() : symbol(0), frequency(0) {
    children[0] = nullptr;
    children[1] = nullptr;
}

Trie::Node::Node(uint16_t symbol) : symbol(symbol), frequency(0) {
    children[0] = nullptr;
    children[1] = nullptr;
}

Trie::Node::Node(uint16_t min_character, size_t frequency) : symbol(min_character), frequency(frequency) {
    children[0] = nullptr;
    children[1] = nullptr;
}

Trie::Node::Node(std::shared_ptr<Node> first, std::shared_ptr<Node> second)
    : symbol(std::min(first->symbol, second->symbol)), frequency(first->frequency + second->frequency) {
    children[0] = first;
    children[1] = second;
}

bool Trie::Node::IsLeave() {
    return !children[0] && !children[1];
}

bool Trie::CompareNodePointers::operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const {
    if (lhs->frequency != rhs->frequency) {
        return lhs->frequency > rhs->frequency;
    }
    return lhs->symbol > rhs->symbol;
}

Trie::Trie() : root_(std::make_shared<Node>()) {
}
Trie::Trie(const std::vector<size_t>& frequency) {
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodePointers> queue;

    for (size_t symbol = 0; symbol < frequency.size(); ++symbol) {
        if (frequency[symbol] > 0) {
            queue.push(std::make_shared<Node>(symbol, frequency[symbol]));
        }
    }

    while (queue.size() > 1) {
        std::shared_ptr<Node> first = queue.top();
        queue.pop();
        std::shared_ptr<Node> second = queue.top();
        queue.pop();
        queue.push(std::make_shared<Node>(first, second));
    }

    root_ = queue.top();

    queue.pop();
}
std::vector<std::pair<uint16_t, uint16_t>> Trie::FindSymbolsCodesLens() {
    std::vector<std::pair<uint16_t, uint16_t>> lens_symbols;
    FindSymbolsCodesLens(root_, 0, lens_symbols);
    return lens_symbols;
}
void Trie::FindSymbolsCodesLens(std::shared_ptr<Node> node, uint16_t len,
                                std::vector<std::pair<uint16_t, uint16_t>>& lens_symbols) {
    if (node->IsLeave()) {
        lens_symbols.emplace_back(len, node->symbol);
        return;
    }
    for (int i : {0, 1}) {
        if (node->children[i]) {
            FindSymbolsCodesLens(node->children[i], len + 1, lens_symbols);
        }
    }
}

void Trie::Insert(const CanonicalCode& code, uint16_t symbol) {
    std::shared_ptr<Node> node = root_;
    for (auto bit : code) {
        if (!node->children[bit]) {
            node->children[bit] = std::make_shared<Node>();
        }
        node = node->children[bit];
    }
    node->symbol = symbol;
}

uint16_t Trie::DecodeSymbol(BinaryReader& binary_reader) {
    std::shared_ptr<Node> node = root_;
    while (!node->IsLeave()) {
        node = node->children[binary_reader.ReadBit()];
    }
    return node->symbol;
}
