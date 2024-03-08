#include <map>

#include "../include/compiler.h"

std::map<TokenType, Opcode> binary_operations {
        { ADDITION, OP_ADD },
        { SUBTRACTION, OP_SUBTRACT },
        { MULTIPLICATION, OP_MULTIPLY },
        { DIVISION, OP_DIVIDE },
        { MODULO, OP_MODULO }
};

void Compiler::emit(uint8_t byte) {
    code.add(byte);
}

Token Compiler::next() {
    return index < tokens.size() - 1 ? tokens[index++] : tokens[index];
}

Bytecode Compiler::parse() {
    expression();
    return code;
}

void Compiler::expression() {
    value();

    while (is_binary_operator(peek())) {
        Opcode operation = binary_operations[next().type];
        value();
        emit(operation);
    }
}

Token Compiler::peek() {
    return tokens[index];
}

void Compiler::value() {
    switch (peek().type) {
        case IDENTIFIER:
            break;
        case NUMBER:
            number();
            break;
        case TEXT:
            text();
            break;
        case TRUE:
        case FALSE:
            boolean();
            break;
        default:
            throw UnexpectedTokenException("value", next());
    }
}

void Compiler::number() {
    Integer number = stoi(next().lexeme);
    emit(OP_CONSTANT);
    emit(code.add_constant(number));
}

void Compiler::text() {
    String text = next().lexeme;
    emit(OP_CONSTANT);
    emit(code.add_constant(text));
}

void Compiler::boolean() {
    Boolean boolean = next().lexeme != "false";
    emit(OP_CONSTANT);
    emit(code.add_constant(Value(boolean)));
}

bool Compiler::is_binary_operator(const Token &token) {
    return binary_operations.find(token.type) != binary_operations.end();
}
