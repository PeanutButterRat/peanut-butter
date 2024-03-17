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
                outfile << setw(iwidth) << "JUMP_IF_FALSE" << setw(argwidth) << (int) offset << endl;
                break;
            }
            case OP_JUMP: {
                Short offset = 0;
                offset |= *(++it) << 8;
                offset |= *(++it);
                outfile << setw(iwidth) << "JUMP" << setw(argwidth) << (int) offset << endl;
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

std::vector<std::string> split(const std::string& line) {
    size_t start = line.size();
    std::vector<std::string> words{};

    for (size_t index = 0; index < line.size(); index++) {
        auto character = line[index];

        if (!isspace(character) && start >= line.size()) {
            start = index;
        } else if (isspace(character) && start < line.size()) {
            auto length = index - start;
            words.emplace_back(line.substr(start, length));
            start = line.size();
        }
    }

    if (start < line.size()) {
        words.emplace_back(line.substr(start, line.size()));
    }

    return words;
}

Bytecode Bytecode::deserialize(std::ifstream &infile) {
    Bytecode code{};
    std::string line;

    while (getline(infile, line) && line.find("CONSTANTS:") != 0) {
        auto symbols = split(line);
        if (symbols.empty()) {
            continue;
        }

        auto opcode = symbols[0];

        if (opcode == "ADD") {
            code.add(OP_ADD);
        } else if (opcode == "SUBTRACT") {
            code.add(OP_SUBTRACT);
        } else if (opcode == "MULTIPLY") {
            code.add(OP_MULTIPLY);
        } else if (opcode == "DIVIDE") {
            code.add(OP_DIVIDE);
        } else if (opcode == "CONSTANT") {
            code.add(OP_CONSTANT);
            code.add(std::stoi(symbols[1]));
        } else if (opcode == "MOD") {
            code.add(OP_MODULO);
        } else if (opcode == "PRINT") {
            code.add(OP_PRINT);
        } else if (opcode == "ASSIGMENT") {
            code.add(OP_ASSIGMENT);
            code.add(std::stoi(symbols[1]));
        } else if (opcode == "DECLARATION") {
            code.add(OP_DECLARATION);
            code.add(std::stoi(symbols[1]));
        } else if (opcode == "IDENTIFIER") {
            code.add(OP_IDENTIFIER);
            code.add(std::stoi(symbols[1]));
        } else if (opcode == "ENSCOPE") {
            code.add(OP_ENSCOPE);
        } else if (opcode == "DESCOPE") {
            code.add(OP_DESCOPE);
        } else if (opcode == "JUMP_IF_FALSE") {
            code.add(OP_JUMP_IF_FALSE);
            auto offset = (Short) std::stoi(symbols[1]);
            Byte left = (offset >> 8) & 0xff;
            Byte right = offset & 0xff;
            code.add(left);
            code.add(right);
        } else if (opcode == "JUMP") {
            code.add(OP_JUMP);
            auto offset = (Short) std::stoi(symbols[1]);
            Byte left = (offset >> 8) & 0xff;
            Byte right = offset & 0xff;
            code.add(left);
            code.add(right);
        }
    }

    while (getline(infile, line)) {
        auto symbol = split(line);
        if (symbol.empty()) {
            continue;
        }

        auto string = symbol[0];
        if (string[0] == '"') {
            string = string.substr(1, string.length() - 2);
            code.add_constant(Value(string));
        } else if (string == "true") {
            code.add_constant(true);
        } else if (string == "false") {
            code.add_constant(false);
        } else {
            code.add_constant(std::stoi(string));
        }
    }

    return code;
}


