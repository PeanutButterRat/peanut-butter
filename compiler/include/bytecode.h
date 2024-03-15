#ifndef CPBPL_BYTECODE_H
#define CPBPL_BYTECODE_H

#include <cstdint>
#include <vector>

#include "value.h"

typedef uint8_t Byte;

enum Opcode {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_CONSTANT,
    OP_MODULO,
    OP_PRINT,
    OP_DECLARATION,
    OP_ASSIGMENT,
    OP_IDENTIFIER,
    OP_ENSCOPE,
    OP_DESCOPE,
    OP_JUMP_IF_FALSE,
    OP_JUMP,
};

class Bytecode {
    std::vector<Value> constants;

public:
    std::vector<Byte> bytes;

    size_t add_constant(const Value& constant);
    size_t size() const;
    Value get_constant(size_t index);
    void add(Byte byte);
    void set(size_t index, Byte byte);
    void disassemble() const;

    Bytecode() = default;
    Bytecode(std::vector<Byte> bytes, std::vector<Value> constants);
    friend bool operator== (const Bytecode& a, const Bytecode& b);
    friend bool operator!= (const Bytecode& a, const Bytecode& b);
};

#endif // CPBPL_BYTECODE_H
