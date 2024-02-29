#include <utility>

#include "../include/compiler.h"

std::map<TokenType, Opcode> binary_operations {
        { ADDITION, OP_ADD },
        { SUBTRACTION, OP_SUBTRACT},
        { MULTIPLICATION, OP_MULTIPLY},
        { DIVISION, OP_DIVIDE},
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
    constant();

    while (is_binary_operator(peek())) {
        Opcode operation = binary_operations[next().type];
        constant();
        emit(operation);
    }
}

Token Compiler::peek() {
    return tokens[index];
}

void Compiler::constant() {
    number number = stoi(next().lexeme);
    emit(OP_CONSTANT);
    emit(code.add_constant(number));
}

bool Compiler::is_binary_operator(const Token &token) {
    return binary_operations.find(token.type) != binary_operations.end();
}

size_t Bytecode::add_constant(number constant) {
    constants.emplace_back(constant);
    return constants.size() - 1;
}

void Bytecode::add(byte byte) {
    bytes.emplace_back(byte);
}

void Bytecode::disassemble() const {
    using std::cout, std::endl;

    for (auto it = bytes.begin(); it != bytes.end(); it++) {
        auto opcode = *it;

        switch (opcode) {
            case OP_ADD:
                cout << "ADD" << endl;
                break;
            case OP_SUBTRACT:
                cout << "SUBTRACT"  << endl;
                break;
            case OP_MULTIPLY:
                cout << "MULTIPLY"  << endl;
                break;
            case OP_DIVIDE:
                cout << "DIVIDE"  << endl;
                break;
            case OP_CONSTANT: {
                size_t index = *(++it);
                cout << "CONSTANT [index: " << index << ", value: " << constants[index] << "]" << endl;
                break;
            }
            default:
                cout << "[UNKNOWN OPCODE]" << endl;
                break;
        }
    }
}

bool operator==(const Bytecode &a, const Bytecode &b) {
    return a.bytes == b.bytes && a.constants == b.constants;
}

bool operator!=(const Bytecode &a, const Bytecode &b) {
    return !(a == b);
}

Bytecode::Bytecode(std::vector<byte> bytes, std::vector<number> constants) {
    this->bytes = std::move(bytes);
    this->constants = std::move(constants);
}
