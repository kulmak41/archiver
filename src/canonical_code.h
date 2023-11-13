#pragma once

#include <vector>
#include <cstdint>

class CanonicalCode : public std::vector<uint8_t> {
public:
    CanonicalCode() = default;
    explicit CanonicalCode(size_t len);

    void Increase();

    void Shift(uint16_t shift);
};