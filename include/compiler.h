#ifndef CPBPL_COMPILER_H
#define CPBPL_COMPILER_H

#include <utility>
#include <vector>
#include <cstdint>
#include "scanner.h"

typedef uint8_t byte;
typedef uint32_t number;

enum Opcode {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_CONSTANT,
    OP_MODULO,
};

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

class Compiler {
    Bytecode code;
    std::vector<Token> tokens;
    size_t index = 0;

    void emit(byte byte);
    Token next();
    Token peek();
    void expression();
    void constant();

    static bool is_binary_operator(const Token& token);

public:
    Bytecode parse();
    explicit Compiler(std::vector<Token> tokens) { this->tokens = std::move(tokens); }
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
