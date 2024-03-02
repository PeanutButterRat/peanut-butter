#include <vector>

#include "../include/bytecode.h"

bool operator==(const Bytecode &a, const Bytecode &b) {
    return a.bytes == b.bytes && a.constants == b.constants;
}

bool operator!=(const Bytecode &a, const Bytecode &b) {
    return !(a == b);
}

Bytecode::Bytecode(std::vector<byte> bytes, std::vector<number> constants) {
    this->bytes = std::move(bytes);
    this->constants = std::move(constants);
}

byte Bytecode::get_constant(size_t index) {
    return constants[index];
}
