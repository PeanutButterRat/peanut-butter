#ifndef CPBPL_TOKEN_H
#define CPBPL_TOKEN_H

#include <iostream>

enum TokenType {
    MULTIPLICATION,
    DIVISION,
    SUBTRACTION,
    ADDITION,
    SET,
    LET,
    IDENTIFIER,
    NUMBER,
    IF,
    AS,
    OTHERWISE,
    COLON,
    OR,
    AND,
    TRUE,
    FALSE,
    DEFINE,
    BE,
    TO,
    BLOCK_START,
    BLOCK_END,
    TEXT,
    PERIOD,
    WITH,
    COMMA,
    RETURN,
    MODULO,
    WHILE,
    OF,
    END_OF_STREAM
};

struct Token {
    TokenType type;
    std::string lexeme;
    size_t line;

    std::string get_type_string() const;

    Token(TokenType type, std::string lexeme, size_t line);
    friend std::ostream& operator<<(std::ostream& os, const Token& token);
    friend bool operator== (const Token& a, const Token& b);
    friend bool operator!= (const Token& a, const Token& b);

    static std::string get_type_string(TokenType type);
};

#endif //CPBPL_TOKEN_H
