#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>

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

size_t Bytecode::size() const {
    return bytes.size();
}

void Bytecode::set(size_t index, Byte byte) {
    bytes[index] = byte;
}

void Bytecode::serialize(std::ofstream& outfile) {
    using std::endl, std::setw, std::left, std::right;

    outfile << "MAIN:" << endl;
    for (auto it = bytes.begin(); it != bytes.end(); it++) {
        auto opcode = *it;
        const int iwidth = 15;
        const int argwidth = 5;
        outfile << left << '\t';

        switch (opcode) {
            case OP_ADD: {
                outfile << setw(iwidth) << "ADD" << endl;
                break;
            }
            case OP_SUBTRACT: {
                outfile << setw(iwidth) << "SUBTRACT" << endl;
                break;
            }
            case OP_MULTIPLY: {
                outfile << setw(iwidth) << "MULTIPLY" << endl;
                break;
            }
            case OP_DIVIDE: {
                outfile << setw(iwidth) << "DIVIDE" << endl;
                break;
            }
            case OP_CONSTANT: {
                size_t index = *(++it);
                Value constant = constants[index];
                outfile << setw(iwidth) << "CONSTANT" << setw(argwidth) << index << " -> " << constants[index] << endl;
                break;
            }
            case OP_MODULO: {
                outfile << setw(iwidth) << "MOD" << endl;
                break;
            }
            case OP_PRINT: {
                outfile << setw(iwidth) << "PRINT" << endl;
                break;
            }
            case OP_ASSIGMENT: {
                size_t index = *(++it);
                Value constant = constants[index];
                outfile << setw(iwidth) << "ASSIGMENT" << setw(argwidth) << index << " -> " << constants[index] << endl;
                break;
            }
            case OP_DECLARATION: {
                size_t index = *(++it);
                Value constant = constants[index];
                outfile << setw(iwidth) << "DECLARATION" << setw(argwidth) << index << " -> " << constants[index].string() << endl;
                break;
            }
            case OP_IDENTIFIER: {
                size_t index = *(++it);
                Value constant = constants[index];
                outfile << setw(iwidth) << "IDENTIFIER" << setw(argwidth) << index << " -> " << constants[index].string() << endl;
                break;
            }
            case OP_ENSCOPE: {
                outfile << setw(iwidth) << "ENSCOPE" << endl;
                break;
            }
            case OP_DESCOPE: {
                outfile << setw(iwidth) << "DESCOPE" << endl;
                break;
            }
            case OP_JUMP_IF_FALSE: {
                Short offset = 0;
                offset |= *(++it) << 8;
                offset |= *(++it);
                outfile << setw(iwidth) << "JUMP_IF_FALSE" << setw(argwidth) << ' ' << " -> " << (int) offset << endl;
                break;
            }
            case OP_JUMP: {
                Short offset = 0;
                offset |= *(++it) << 8;
                offset |= *(++it);
                outfile << setw(iwidth) << "JUMP" << setw(argwidth) << ' ' << " -> " << (int) offset << endl;
                break;
            }
            default:
                throw SerializationException(opcode);
        }
    }

    outfile << "\nCONSTANTS:" << endl;
    for (const auto& constant : constants) {
        if (constant.type == STRING) {
            outfile << '\t' << '"' << constant << '"' << endl;
        } else {
            outfile << '\t' << constant << endl;
        }
    }
}

