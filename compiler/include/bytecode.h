#ifndef CPBPL_BYTECODE_H
#define CPBPL_BYTECODE_H

#include <cstdint>
#include <vector>

#include "value.h"

typedef uint8_t byte;

enum Opcode {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_CONSTANT,
    OP_MODULO,
    OP_PRINT,
};

class Bytecode {
    std::vector<Value> constants;

public:
    std::vector<byte> bytes;

    size_t add_constant(const Value& constant);
    Value get_constant(size_t index);
    void add(byte byte);
    void disassemble() const;

    Bytecode() = default;
    Bytecode(std::vector<byte> bytes, std::vector<Value> constants);
    friend bool operator== (const Bytecode& a, const Bytecode& b);
    friend bool operator!= (const Bytecode& a, const Bytecode& b);
};

#endif // CPBPL_BYTECODE_H
