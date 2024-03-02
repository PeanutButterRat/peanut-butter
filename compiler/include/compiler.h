#ifndef CPBPL_COMPILER_H
#define CPBPL_COMPILER_H

#include <vector>
#include <cstdint>

#include "token.h"
#include "bytecode.h"

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
