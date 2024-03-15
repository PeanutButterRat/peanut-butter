#include <map>

#include "../include/compiler.h"

std::map<TokenType, Opcode> binary_operations {
        { ADDITION, OP_ADD },
        { SUBTRACTION, OP_SUBTRACT },
        { MULTIPLICATION, OP_MULTIPLY },
        { DIVISION, OP_DIVIDE },
        { MODULO, OP_MODULO }
};

size_t Compiler::emit(Byte byte) {
    auto spot = code.size();
    code.add(byte);
    return spot;
}

Token Compiler::next() {
    return index < tokens.size() - 1 ? tokens[index++] : tokens[index];
}

Bytecode Compiler::parse() {
    declarations();
    consume(END_OF_STREAM);
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
            identifier();
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
    Integer number = stoi(consume(NUMBER).lexeme);
    emit(OP_CONSTANT);
    emit(code.add_constant(number));
}

void Compiler::text() {
    String text = consume(TEXT).lexeme;
    emit(OP_CONSTANT);
    emit(code.add_constant(text));
}

void Compiler::identifier() {
    emit(OP_IDENTIFIER);
    emit(code.add_constant(consume(IDENTIFIER).lexeme));
}

void Compiler::assigment() {
    consume(LET);
    auto name = consume(IDENTIFIER).lexeme;
    consume(BE);
    expression();
    consume(PERIOD);

    emit(OP_ASSIGMENT);
    emit(code.add_constant(name));
}

void Compiler::boolean() {
    Boolean boolean = next().lexeme != "false";
    emit(OP_CONSTANT);
    emit(code.add_constant(Value(boolean)));
}

Token Compiler::consume(TokenType type) {
    auto token = next();
    if (token.type != type) {
        throw UnexpectedTokenException(type, token);
    }
    return token;
}

bool Compiler::is_binary_operator(const Token &token) {
    return binary_operations.find(token.type) != binary_operations.end();
}

void Compiler::block() {
    consume(BLOCK_START);
    emit(OP_ENSCOPE);
    declarations();
    emit(OP_DESCOPE);
    consume(BLOCK_END);

}

bool Compiler::check(TokenType next) {
    return peek().type == next;
}

void Compiler::declarations() {
    while (!check(END_OF_STREAM)) {
        switch (peek().type) {
            case LET:
                assigment();
                break;
            case BLOCK_START:
                block();
                break;
            case IF:
                conditional();
                break;
            default:
                return;
        }
    }
}

void Compiler::conditional() {
    consume(IF);
    expression();
    consume(COLON);
    auto location = jump(OP_JUMP_IF_FALSE);
    block();
    patch(location);

    if (check(OTHERWISE)) {
        otherwise();
    }
}

void Compiler::otherwise() {
    consume(OTHERWISE);
    consume(COLON);
    auto location = jump(OP_JUMP);
    block();
    patch(location);
}

size_t Compiler::jump(Byte instruction) {
    emit(instruction);
    Byte placeholder = 0xff;
    return emit(placeholder);
}

void Compiler::patch(size_t location) {
    size_t offset = code.size() - location;
    code.set(location, offset);
}
