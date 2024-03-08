#include <vector>

#include "../include/bytecode.h"

bool operator==(const Bytecode &a, const Bytecode &b) {
    return a.bytes == b.bytes && a.constants == b.constants;
}

bool operator!=(const Bytecode &a, const Bytecode &b) {
    return !(a == b);
}

Bytecode::Bytecode(std::vector<byte> bytes, std::vector<Value> constants) {
    this->bytes = std::move(bytes);
    this->constants = std::move(constants);
}

Value Bytecode::get_constant(size_t index) {
    return constants[index];
}

size_t Bytecode::add_constant(const Value& constant) {
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
                Value constant = constants[index];
                cout << "CONSTANT [index: " << index << ", value: " << constant << ", type: " << constant.get_type_string() << "]" << endl;
                break;
            }
            case OP_MODULO: {
                cout << "MOD" << endl;
                break;
            }
            default:
                cout << "[UNKNOWN OPCODE] (" << (unsigned int) opcode << ")" << endl;
                break;
        }
    }
}

