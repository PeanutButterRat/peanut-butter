#include "scanner.h"

#include <bits/stdc++.h>


const std::map<std::string, TokenType> keywords {
        { std::string("class"), TokenType::CLASS },
        { std::string("define"), TokenType::DEFINE },
        { std::string("as"), TokenType::AS },
        { std::string("set"), TokenType::SET },
        { std::string("let"), TokenType::LET },
        { std::string("be"), TokenType::BE },
        { std::string("to"), TokenType::TO },
        { std::string("parameters"), TokenType::PARAMETERS },
        { std::string("or"), TokenType::OR },
        { std::string("if"), TokenType::IF },
        { std::string("and"), TokenType::AND },
        { std::string("true"), TokenType::TRUE },
        { std::string("false"), TokenType::FALSE },
        { std::string("otherwise"), TokenType::OTHERWISE },
        { std::string("times"), TokenType::MULTIPLICATION},
        { std::string("minus"), TokenType::SUBTRACTION },
        { std::string("plus"), TokenType::ADDITION },
        { std::string("over"), TokenType::DIVISION },
};

std::vector<Token> Scanner::tokenize() {
    std::transform(source.begin(), source.end(), source.begin(), tolower);
    std::stack<size_t> stack{};

    while (has_next()) {
        size_t tabs = 0;

        while (peek() == '\t') {
            tabs++;
            next();
        }

        if (!stack.empty() && tabs > stack.top()) {
            stack.push(tabs);
            tokens.emplace_back(BLOCK_START, "", line);
        }

        auto current = peek();

        while (!is_line_terminator(current)) {
            if (current == '\n') {
                newline();
            } else if (isspace(current)) {
                whitespace();
            } else if (current == '(') {
                comment();
            } else if (current == ':') {
                colon();
            } else if (current == '.') {
                period();
            } else if (current == '"' || current == '\'') {
                string(current);
            } else if (is_number_start_char(current)) {
                number();
            } else if (is_identifier_char(current)) {  // Identifier or keyword.
                word();
            } else {
                throw ScannerException(std::string("Unexpected character: '") + current + "'", line);
            }

            current = peek();
        }

        next();  // Discard the newline.

        while (!stack.empty() && tabs > stack.top()) {
            stack.pop();
            tokens.emplace_back(BLOCK_END, "", line);
        }
    }

    return tokens;
}

void Scanner::word() {
    auto start = index;

    while (is_identifier_char(peek())) { next(); }
    auto word = source.substr(start, index - start);
    auto type = keywords.count(word) > 0 ? keywords.at(word) : IDENTIFIER;

    tokens.emplace_back(type, word, line);
}

void Scanner::comment() {
    while (!is_line_terminator(peek()) && peek() != ')') { next(); }
    if (next() != ')') { throw ScannerException("Unterminated comment", line); }
}

void Scanner::string(char terminator) {
    auto start = index;
    next();  // Move past the opening quotation.

    while (!is_line_terminator(peek()) && peek() != '"') { next(); }
    if (next() != terminator) { throw ScannerException("Unterminated string literal", line); }
    auto word = source.substr(start + 1, index - start - 2);  // Don't include the quotes.

    tokens.emplace_back(STRING, word, line);
}

bool Scanner::is_identifier_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

char Scanner::next() {
    return has_next() ? source[index++] : '\0';
}

char Scanner::peek() {
    return has_next() ? source[index] : '\0';
}

bool Scanner::is_line_terminator(char c) {
    return c == '\n' || c == '\0';
}

bool Scanner::has_next() {
    return index < source.length();
}

void Scanner::whitespace() {
    next();
}

void Scanner::period() {
    tokens.emplace_back(PERIOD, ".", line);
    next();
}

void Scanner::newline() {
    line++;
    next();
}

void Scanner::colon() {
    tokens.emplace_back(COLON, ":", line);
    next();
}

Scanner::Scanner(std::string source) {
    this->source = std::move(source);
    line = 1;
}

void Scanner::number() {
    auto start = index;
    next();  // Move past the opening quotation.

    while (isdigit(peek()) && peek() != '"') { next(); }
    auto number = source.substr(start + 1, index - start - 2);  // Don't include the quotes.

    tokens.emplace_back(NUMBER, number, line);
}

bool Scanner::is_number_start_char(char c) {
    return isdigit(c) && c != '0';
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    std::string type = "UNKNOWN";

    switch (token.type) {
        case COLON:
            type = "COLON";
            break;
        case PERIOD:
            type = "PERIOD";
            break;
        case STRING:
            type = "STRING";
            break;
        case BLOCK_START:
            type = "BLOCK_START";
            break;
        case BLOCK_END:
            type = "BLOCK_END";
            break;
        case IDENTIFIER:
            type = "IDENTIFIER";
            break;
        default:  // Search for the keyword literal in the map.
            for (auto [keyword, value] : keywords) {
                if (value == token.type) {
                    type = keyword;
                    std::transform(type.begin(), type.end(), type.begin(), toupper);
                    break;
                }
            }
            break;
    }

    os << "Token: { Type: " << type << ", Lexeme: " << token.lexeme << ", Line: " << token.line << " }";
    return os;
}

Token::Token(TokenType type, std::string lexeme, size_t line) {
    this->type = type;
    this->lexeme = std::move(lexeme);
    this->line = line;
}
