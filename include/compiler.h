#ifndef CPBPL_COMPILER_H
#define CPBPL_COMPILER_H

#include <vector>
#include <cstdint>
#include "scanner.h"

typedef uint8_t byte;
typedef uint32_t number;

enum Opcode {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    CONSTANT
};

class Bytecode {
    std::vector<byte> bytes;
    std::vector<number> constants;

public:
    size_t add_constant(number constant);
    void add(byte byte);
};

class Compiler {
    Bytecode code;
    std::vector<Token> tokens;
    size_t index = 0;

    void emit(byte byte);
    Token next();
    Token peek();
    void expression();
    void constant();
    void addition();
    void subtraction();
    void multiplication();
    void division();

public:
    Bytecode parse();
};

class UnexpectedTokenException : public std::exception {
    std::string reason;
    size_t line;

public:
    explicit UnexpectedTokenException(const Token& token) {
        this->reason = "Unexpected token (" + token.get_type_string() + ", " + token.lexeme + ")";
        this->line = token.line;
    }

    [[nodiscard]] const char* what() const noexcept override { return (reason + " on line " + std::to_string(line) + ".").c_str(); }
};

#endif // CPBPL_COMPILER_H
