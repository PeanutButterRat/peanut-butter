#include "../include/scanner.h"

#include <bits/stdc++.h>

const std::map<std::string, TokenType> keywords {
        { "define", DEFINE },
        { "as", AS },
        { "set", SET },
        { "let", LET },
        { "be", BE },
        { "to", TO },
        { "or", OR },
        { "if", IF },
        { "and", AND },
        { "true", TRUE },
        { "false", FALSE },
        { "otherwise", OTHERWISE },
        { "times", MULTIPLICATION},
        { "minus", SUBTRACTION },
        { "plus", ADDITION },
        { "over", DIVISION },
        { "with", WITH },
        { "return", RETURN },
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

        if (!stack.empty() && tabs > stack.top() || stack.empty() && tabs > 0) {
            stack.push(tabs);
            tokens.emplace_back(BLOCK_START, "", line);
        }
        while (!stack.empty() && tabs < stack.top()) {
            stack.pop();
            tokens.emplace_back(BLOCK_END, "", line);
        }

        auto current = peek();

        while (!is_line_terminator(current)) {
            if (isspace(current)) {
                whitespace();
            } else if (current == '(') {
                comment();
            } else if (current == ':') {
                colon();
            } else if (current == '.') {
                period();
            } else if (current == ',') {
                comma();
            } else if (current == '"' || current == '\'') {
                string(current);
            } else if (isdigit(current)) {
                number();
            } else if (is_identifier_char(current)) {  // Identifier or keyword.
                word();
            } else {
                throw ScannerException(std::string("Unexpected character: '") + current + "'", line);
            }

            current = peek();
        }

        newline();  // Discard the newline character.
    }

    while (!stack.empty()) {
        stack.pop();
        tokens.emplace_back(BLOCK_END, "", line);
    }

    tokens.emplace_back(END_OF_STREAM, "", line);

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

    if (peek() == '0') {  // Numbers that start with '0' are treated as 0 (no leading 0's allowed).
        tokens.emplace_back(NUMBER, "0", line);
        next();
    } else {
        next();

        while (isdigit(peek())) { next(); }
        auto number = source.substr(start, index - start);

        tokens.emplace_back(NUMBER, number, line);
    }
}

void Scanner::comma() {
    tokens.emplace_back(COMMA, ",", line);
    next();
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    return os << "Token: { Type: " << token.get_type_string() << ", Lexeme: '" << token.lexeme << "', Line: " << token.line << " }";
}

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
    std::string type_string = "UNKNOWN";
    switch (type) {
        case COLON:
            type_string = "COLON";
            break;
        case PERIOD:
            type_string = "PERIOD";
            break;
        case STRING:
            type_string = "STRING";
            break;
        case BLOCK_START:
            type_string = "BLOCK_START";
            break;
        case BLOCK_END:
            type_string = "BLOCK_END";
            break;
        case IDENTIFIER:
            type_string = "IDENTIFIER";
            break;
        case COMMA:
            type_string = "COMMA";
            break;
        case NUMBER:
            type_string = "NUMBER";
            break;
        default:  // Search for the keyword literal in the map.
            for (auto [keyword, value] : keywords) {
                if (value == type) {
                    type_string = keyword;
                    std::transform(type_string.begin(), type_string.end(), type_string.begin(), toupper);
                    break;
                }
            }
            break;
    }
    return type_string;
}
