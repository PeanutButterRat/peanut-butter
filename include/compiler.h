#ifndef CPBPL_COMPILER_H
#define CPBPL_COMPILER_H

#include <vector>
#include <cstdint>
#include "scanner.h"

typedef std::vector<uint8_t> Bytecode;

enum Opcode {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    CONSTANT
};

class Compiler {
    Bytecode bytes;
    std::vector<Token> tokens;
    size_t ti = 0;
    size_t bi = 0;

    void emit(uint8_t byte);
    void match(const Token& token);
    Token next();

    void expression();

public:
    std::vector<uint8_t> parse();
};

class CompilerException : public std::exception {
    std::string reason;
    size_t line;

public:
    CompilerException(std::string reason, size_t line) {
        this->reason = std::move(reason);
        this->line = line;
    }

    [[nodiscard]] const char* what() const noexcept override { return (reason + " on line " + std::to_string(line) + ".").c_str(); }
};

#endif // CPBPL_COMPILER_H
