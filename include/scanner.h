#ifndef CPBPL_SCANNER_H
#define CPBPL_SCANNER_H

#include <string>
#include <vector>
#include <map>
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
    STRING,
    PERIOD,
    WITH,
    COMMA,
    RETURN,
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
};

class Scanner {
    std::string source;
    std::vector<Token> tokens;
    size_t index = 0;
    size_t line = 0;

    char next();
    char peek();
    void string(char terminator);
    void word();
    void whitespace();
    void period();
    void comma();
    void newline();
    void colon();
    void comment();
    void number();
    bool has_next();

    static bool is_identifier_char(char c);
    static bool is_line_terminator(char c);

public:
    explicit Scanner(std::string source);
    std::vector<Token> tokenize();
};

class ScannerException : public std::exception {
    std::string reason;
    size_t line;

public:
    ScannerException(std::string reason, size_t line) {
        this->reason = std::move(reason);
        this->line = line;
    }

    [[nodiscard]] const char* what() const noexcept override { return (reason + " on line " + std::to_string(line) + ".").c_str(); }
};

#endif // CPBPL_SCANNER_H