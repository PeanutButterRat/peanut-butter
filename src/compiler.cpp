#include "../include/compiler.h"

void Compiler::emit(uint8_t byte) {

}

void Compiler::match(const Token& token) {

}

Token Compiler::next() {
    return tokens[ti++];
}

std::vector<uint8_t> Compiler::parse() {
    return bytes;
}

void Compiler::expression() {

}
