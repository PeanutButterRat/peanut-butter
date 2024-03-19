#ifndef CPBPL_BYTECODE_H
#define CPBPL_BYTECODE_H

#include <cstdint>
#include <vector>
#include <string>

#include "value.h"

typedef uint8_t Byte;
typedef int16_t Short;

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
    OP_PUSH_SCOPE,
    OP_POP_SCOPE,
    OP_JUMP_IF_FALSE,
    OP_JUMP,
    OP_EQUALITY,
    OP_GREATER_THAN,
    OP_LESS_THAN,
};

std::vector<std::string> split(const std::string &line);

class Bytecode {
    std::vector<Value> constants;

public:
    std::vector<Byte> bytes;

    size_t add_constant(const Value& constant);
    [[nodiscard]] size_t size() const;
    Value get_constant(size_t index);
    void add(Byte byte);
    void set(size_t index, Byte byte);
    void serialize(std::ofstream& outfile);
    static Bytecode deserialize(std::ifstream& infile);

    Bytecode() = default;
    Bytecode(std::vector<Byte> bytes, std::vector<Value> constants);
    friend bool operator== (const Bytecode& a, const Bytecode& b);
    friend bool operator!= (const Bytecode& a, const Bytecode& b);
};

class SerializationException : std::exception {
    std::string reason;

public:
    SerializationException(int opcode) {
        reason = "Unknown opcode encountered during serialization (" + std::to_string(opcode) + ")";
    }

    const char * what() const noexcept override { return reason.c_str(); }
};

#endif // CPBPL_BYTECODE_H
