#ifndef CPBPL_BYTECODE_H
#define CPBPL_BYTECODE_H

#include <cstdint>

typedef uint8_t byte;
typedef uint32_t number;

class Bytecode {
    std::vector<number> constants;

public:
    std::vector<byte> bytes;

    size_t add_constant(number constant);
    void add(byte byte);
    void disassemble() const;

    Bytecode() = default;
    Bytecode(std::vector<byte> bytes, std::vector<number> constants);
    friend bool operator== (const Bytecode& a, const Bytecode& b);
    friend bool operator!= (const Bytecode& a, const Bytecode& b);
};

#endif // CPBPL_BYTECODE_H
