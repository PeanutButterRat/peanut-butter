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

    void emit(Byte byte);
    Token next();
    Token peek();
    bool check(TokenType next);
    void expression();
    void value();
    void number();
    void text();
    void identifier();
    void assigment();
    void boolean();
    void block();
    void declarations();

    Token consume(TokenType type);

    static bool is_binary_operator(const Token& token);

public:
    Bytecode parse();
    explicit Compiler(std::vector<Token> tokens) { this->tokens = std::move(tokens); }
};

class UnexpectedTokenException : public std::exception {
    std::string reason;

public:
    explicit UnexpectedTokenException(const std::string& expected, const Token& token) {
        this->reason = "Expected " + expected + " but found token (" + token.get_type_string()
                + ", " + token.lexeme + ") on line " + std::to_string(token.line) + ".";
    }

    explicit UnexpectedTokenException(TokenType type, const Token& token) {
        this->reason = "Expected " + Token::get_type_string(type) + " but found token (" + token.get_type_string()
                       + ", " + token.lexeme + ") on line " + std::to_string(token.line) + ".";
    }

    [[nodiscard]] const char* what() const noexcept override { return reason.c_str(); }
};

#endif // CPBPL_COMPILER_H
