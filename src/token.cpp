#include <map>

#include "../include/token.h"


std::map<TokenType, std::string> type_strings {
        { MULTIPLICATION, "MULTIPLICATION" },
        { DIVISION, "DIVISION" },
        { SUBTRACTION, "SUBTRACTION" },
        { ADDITION, "ADDITION" },
        { SET, "SET" },
        { LET, "LET" },
        { IDENTIFIER, "IDENTIFIER" },
        { NUMBER, "NUMBER" },
        { IF, "IF" },
        { AS, "AS" },
        { OTHERWISE, "OTHERWISE" },
        { COLON, "COLON" },
        { OR, "OR" },
        { AND, "AND" },
        { TRUE, "TRUE" },
        { FALSE, "FALSE" },
        { DEFINE, "DEFINE" },
        { BE, "BE" },
        { TO, "TO" },
        { BLOCK_START, "BLOCK_START" },
        { BLOCK_END, "BLOCK_END" },
        { STRING, "STRING" },
        { PERIOD, "PERIOD" },
        { WITH, "WITH" },
        { COMMA, "COMMA" },
        { RETURN, "RETURN" },
        { MODULO, "MODULO" },
        { END_OF_STREAM, "END_OF_STREAM" },
};

Token::Token(TokenType type, std::string lexeme, size_t line) {
    this->type = type;
    this->lexeme = std::move(lexeme);
    this->line = line;
}

bool operator==(const Token& a, const Token& b) {
    return a.type == b.type && a.lexeme == b.lexeme && a.line == b.line;
}

bool operator!=(const Token& a, const Token& b) {
    return !(a == b);
}

std::string Token::get_type_string() const {
    if (type_strings.find(type) != type_strings.end()) {
        return type_strings[type];
    } else {
        return "UNKNOWN";
    }
}