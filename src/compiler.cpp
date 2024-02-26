#include "../include/compiler.h"

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
    constant();
    switch (peek().type) {
        case ADDITION:
            addition();
            break;
        case SUBTRACTION:
            subtraction();
            break;
        case MULTIPLICATION:
            multiplication();
            break;
        case DIVISION:
            division();
            break;
    }
}

Token Compiler::peek() {
    return tokens[index];
}

void Compiler::constant() {
    number number = stoi(next().lexeme);
    emit(CONSTANT);
    emit(code.add_constant(number));
}

void Compiler::subtraction() {
    next();  // Consume the '-'.
    expression();
    emit(SUBTRACT);
}

void Compiler::addition() {
    next();  // Consume the '+'.
    expression();
    emit(ADDITION);
}

void Compiler::multiplication() {
    next();  // Consume the '*'.
    expression();
    emit(MULTIPLY);
}

void Compiler::division() {
    next();  // Consume the '/'.
    expression();
    emit(DIVIDE);
}

size_t Bytecode::add_constant(number constant) {
    constants.emplace_back(constant);
    return constants.size() - 1;
}

void Bytecode::add(byte byte) {
    bytes.emplace_back(byte);
}
