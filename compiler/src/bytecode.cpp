#include <vector>

#include "../include/bytecode.h"

bool operator==(const Bytecode &a, const Bytecode &b) {
    return a.bytes == b.bytes && a.constants == b.constants;
}

bool operator!=(const Bytecode &a, const Bytecode &b) {
    return !(a == b);
}

Bytecode::Bytecode(std::vector<Byte> bytes, std::vector<Value> constants) {
    this->bytes = std::move(bytes);
    this->constants = std::move(constants);
}

Value Bytecode::get_constant(size_t index) {
    return constants[index];
}

size_t Bytecode::add_constant(const Value& constant) {
    for (size_t i = 0; i < constants.size(); i++) {
        if (constants[i] == constant) {
            return i;
        }
    }
    constants.emplace_back(constant);
    return constants.size() - 1;
}

void Bytecode::add(Byte byte) {
    bytes.emplace_back(byte);
}

void Bytecode::disassemble() const {
    using std::cout, std::endl;

    for (auto it = bytes.begin(); it != bytes.end(); it++) {
        auto opcode = *it;

        switch (opcode) {
            case OP_ADD: {
                cout << "ADD" << endl;
                break;
            }
            case OP_SUBTRACT: {
                cout << "SUBTRACT" << endl;
                break;
            }
            case OP_MULTIPLY: {
                cout << "MULTIPLY" << endl;
                break;
            }
            case OP_DIVIDE: {
                cout << "DIVIDE" << endl;
                break;
            }
            case OP_CONSTANT: {
                size_t index = *(++it);
                Value constant = constants[index];
                cout << "CONSTANT [index: " << index << ", value: " << constant << ", type: " << constant.get_type_string() << "]" << endl;
                break;
            }
            case OP_MODULO: {
                cout << "MOD" << endl;
                break;
            }
            case OP_PRINT: {
                cout << "PRINT" << endl;
                break;
            }
            case OP_ASSIGMENT: {
                size_t index = *(++it);
                Value constant = constants[index];
                cout << "ASSIGMENT [identifier: " << constants[index].string() << "]" << endl;
                break;
            }
            case OP_IDENTIFIER: {
                size_t index = *(++it);
                Value constant = constants[index];
                cout << "IDENTIFIER [index: " << index << ", name: " << constants[index].string() << "]" << endl;
                break;
            }
            case OP_ENSCOPE: {
                cout << "ENSCOPE" << endl;
                break;
            }
            case OP_DESCOPE: {
                cout << "DESCOPE" << endl;
                break;
            }
            case OP_JUMP_IF_FALSE: {
                size_t offset = *(++it);
                cout << "JUMP IF FALSE [offset: " << offset << "]" << endl;
                break;
            }
            case OP_JUMP: {
                size_t offset = *(++it);
                cout << "JUMP [offset: " << offset << "]" << endl;
                break;
            }
            default:
                cout << "[UNKNOWN OPCODE] (" << (unsigned int) opcode << ")" << endl;
                break;
        }
    }
}

size_t Bytecode::size() const {
    return bytes.size();
}

void Bytecode::set(size_t index, Byte byte) {
    bytes[index] = byte;
}

