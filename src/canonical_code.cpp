#include "canonical_code.h"

CanonicalCode::CanonicalCode(size_t len) : std::vector<uint8_t>(len, 0) {
}

void CanonicalCode::Increase() {
    for (uint16_t i = size() - 1; i >= 0; --i) {
        if (at(i) == 0) {
            at(i) = 1;
            break;
        }
        at(i) = 0;
    }
}

void CanonicalCode::Shift(uint16_t shift) {
    resize(size() + shift);
}
