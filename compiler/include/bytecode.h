#ifndef CPBPL_BYTECODE_H
#define CPBPL_BYTECODE_H

#include <cstdint>
#include <vector>

typedef uint8_t byte;
typedef uint32_t number;

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
    std::vector<number> constants;

public:
    std::vector<byte> bytes;

    size_t add_constant(number constant);
    byte get_constant(size_t index);
    void add(byte byte);
    void disassemble() const;

    Bytecode() = default;
    Bytecode(std::vector<byte> bytes, std::vector<number> constants);
    friend bool operator== (const Bytecode& a, const Bytecode& b);
    friend bool operator!= (const Bytecode& a, const Bytecode& b);
};

#endif // CPBPL_BYTECODE_H
