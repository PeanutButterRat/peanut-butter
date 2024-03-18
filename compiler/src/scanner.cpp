#include <unordered_map>
#include <stack>
#include <utility>
#include <algorithm>

#include "../include/scanner.h"

const std::unordered_map<std::string, TokenType> keywords{
        {"define",    DEFINE},
        {"as",        AS},
        {"set",       SET},
        {"let",       LET},
        {"be",        BE},
        {"to",        TO},
        {"or",        OR},
        {"if",        IF},
        {"and",       AND},
        {"true",      TRUE},
        {"false",     FALSE},
        {"otherwise", OTHERWISE},
        {"times",     MULTIPLICATION},
        {"minus",     SUBTRACTION},
        {"plus",      ADDITION},
        {"over",      DIVISION},
        {"with",      WITH},
        {"return",    RETURN},
        {"mod",       MODULO},
        {"while",     WHILE},
        {"of",        OF},
        {"print",     PRINT},
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
    auto type = keywords.find(word) != keywords.end() ? keywords.at(word) : IDENTIFIER;

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

    tokens.emplace_back(TEXT, word, line);
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
    return os << "Token: { Type: " << token.get_type_string() << ", Lexeme: '" << token.lexeme << "', Line: "
              << token.line << " }";
}
