#include <map>

#include "../include/compiler.h"

const Byte placeholder = 0xff;

std::map<TokenType, Opcode> binary_operations{
        {ADDITION,       OP_ADD},
        {SUBTRACTION,    OP_SUBTRACT},
        {MULTIPLICATION, OP_MULTIPLY},
        {DIVISION,       OP_DIVIDE},
        {MODULO,         OP_MODULO},
        {IS,             OP_EQUALITY},
        {GREATER,        OP_GREATER_THAN},
        {LESS,           OP_LESS_THAN},
        {AND,            OP_AND},
        {OR,             OP_OR},
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
    statements();
    consume(END_OF_STREAM);
    return code;
}

void Compiler::expression() {
    value();

    while (is_binary_operator(peek())) {
        Opcode operation = binary_operations[next().type];
        if (operation == OP_GREATER_THAN || operation == OP_LESS_THAN) {
            consume(THAN);
        }
        value();
        emit(operation);
    }
}

TokenType Compiler::peek() const {
    return tokens[index].type;
}

void Compiler::value() {
    switch (peek()) {
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

void Compiler::declaration() {
    consume(LET);
    auto name = consume(IDENTIFIER).lexeme;
    consume(BE);
    expression();
    consume(PERIOD);

    emit(OP_DECLARATION);
    emit(code.add_constant(name));
}

void Compiler::assigment() {
    consume(SET);
    auto name = consume(IDENTIFIER).lexeme;
    consume(TO);
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

bool Compiler::is_binary_operator(TokenType type) {
    return binary_operations.find(type) != binary_operations.end();
}

void Compiler::block() {
    consume(BLOCK_START);
    emit(OP_PUSH_SCOPE);
    statements();
    emit(OP_POP_SCOPE);
    consume(BLOCK_END);
}

bool Compiler::check(TokenType next) {
    return peek() == next;
}

void Compiler::statements() {
    while (!check(END_OF_STREAM)) {
        switch (peek()) {
            case LET:
                declaration();
                break;
            case SET:
                assigment();
                break;
            case BLOCK_START:
                block();
                break;
            case IF:
                conditional();
                break;
            case WHILE:
                loop();
                break;
            case PRINT:
                print();
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
    auto falsey = jump(OP_JUMP_IF_FALSE);
    block();

    size_t truthy;
    if (check(OTHERWISE)) {
        truthy = jump(OP_JUMP);
    }

    patch(falsey);

    if (check(OTHERWISE)) {
        otherwise();
        patch(truthy);
    }
}

void Compiler::otherwise() {
    consume(OTHERWISE);
    consume(COLON);
    block();
}

size_t Compiler::jump(Byte instruction) {
    emit(instruction);
    size_t location = emit(placeholder);
    emit(placeholder);
    return location;
}

void Compiler::patch(size_t location) {
    Short jump = here() - location - 2;
    Byte left = jump >> 8 & 0xff;
    Byte right = jump & 0xff;
    code.set(location, left);
    code.set(location + 1, right);
}

void Compiler::loop() {
    consume(WHILE);
    auto start = here();
    expression();
    consume(COLON);
    auto skip = jump(OP_JUMP_IF_FALSE);
    block();
    jump(OP_JUMP, start);
    patch(skip);
}

void Compiler::jump(Byte instruction, Byte location) {
    emit(instruction);
    Short jump = location - here() - 2;
    Byte left = (jump >> 8) & 0xff;
    Byte right = jump & 0xff;
    emit(left);
    emit(right);
}

size_t Compiler::here() {
    return code.size();
}

void Compiler::function() {

}

void Compiler::print() {
    consume(PRINT);
    expression();
    emit(OP_PRINT);
    consume(PERIOD);
}
