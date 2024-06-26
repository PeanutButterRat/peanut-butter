#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <unordered_map>

#include "../include/bytecode.h"

std::unordered_map<std::string, Opcode> opcodes{
        {"ADD",           OP_ADD},
        {"SUBTRACT",      OP_SUBTRACT},
        {"MULTIPLY",      OP_MULTIPLY},
        {"DIVIDE",        OP_DIVIDE},
        {"CONSTANT",      OP_CONSTANT},
        {"MODULO",        OP_MODULO},
        {"PRINT",         OP_PRINT},
        {"ASSIGMENT",     OP_ASSIGMENT},
        {"DECLARATION",   OP_DECLARATION},
        {"IDENTIFIER",    OP_IDENTIFIER},
        {"PUSH_SCOPE",    OP_PUSH_SCOPE},
        {"POP_SCOPE",     OP_POP_SCOPE},
        {"JUMP_IF_FALSE", OP_JUMP_IF_FALSE},
        {"JUMP",          OP_JUMP},
        {"GREATER_THAN",  OP_GREATER_THAN},
        {"LESS_THAN",     OP_LESS_THAN},
        {"EQUALITY",      OP_EQUALITY},
        {"AND",           OP_AND},
        {"OR",            OP_OR},
};

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

size_t Bytecode::add_constant(const Value &constant) {
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

void Bytecode::serialize(std::ofstream &outfile) {
    using std::endl, std::setw, std::left, std::right;
    const int iwidth = 15;
    const int argwidth = 5;

    outfile << "MAIN:" << endl;
    for (auto it = bytes.begin(); it != bytes.end(); it++) {
        auto opcode = *it;
        outfile << left << '\t';

        std::string key;
        for (auto &i: opcodes) {
            if (i.second == opcode) {
                key = i.first;
                break;
            }
        }
        if (key.empty()) {
            throw SerializationException(opcode);
        }

        switch (opcode) {
            case OP_ADD:
            case OP_SUBTRACT:
            case OP_MULTIPLY:
            case OP_DIVIDE:
            case OP_MODULO:
            case OP_OR:
            case OP_AND:
            case OP_EQUALITY:
            case OP_GREATER_THAN:
            case OP_LESS_THAN:
            case OP_PRINT:
            case OP_PUSH_SCOPE:
            case OP_POP_SCOPE: {
                outfile << setw(iwidth) << key << endl;
                break;
            }
            case OP_CONSTANT:
            case OP_ASSIGMENT:
            case OP_DECLARATION:
            case OP_IDENTIFIER: {
                size_t index = *(++it);
                Value constant = constants[index];
                outfile << setw(iwidth) << key << setw(argwidth) << index << " -> " << constants[index] << endl;
                break;
            }
            case OP_JUMP_IF_FALSE:
            case OP_JUMP: {
                Short offset = 0;
                offset |= *(++it) << 8;
                offset |= *(++it);
                outfile << setw(iwidth) << key << setw(argwidth) << (int) offset << endl;
                break;
            }
            default:
                throw SerializationException(opcode);
        }
    }

    outfile << "\nCONSTANTS:" << endl;
    for (const auto &constant: constants) {
        if (constant.type == STRING) {
            outfile << '\t' << '"' << constant << '"' << endl;
        } else {
            outfile << '\t' << constant << endl;
        }
    }
}

std::vector<std::string> split(const std::string &line) {
    std::vector<std::string> words{};

    for (size_t index = 0; index < line.size(); index++) {
        if (line[index] == '"') {
            auto start = index++;
            while (index < line.size() && line[index++] != '"');
            words.emplace_back(line.substr(start, index - start));
        } else if (!isspace(line[index])) {
            auto start = index++;
            while (index < line.size() && !isspace(line[index])) index++;
            words.emplace_back(line.substr(start, index - start));
        }
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

        if (opcodes.find(symbols[0]) != opcodes.end()) {
            auto opcode = opcodes[symbols[0]];
            code.add(opcode);

            switch (opcode) {
                case OP_CONSTANT:
                case OP_ASSIGMENT:
                case OP_DECLARATION:
                case OP_IDENTIFIER:
                    code.add(std::stoi(symbols[1]));
                    break;
                case OP_JUMP_IF_FALSE:
                case OP_JUMP: {
                    auto offset = (Short) std::stoi(symbols[1]);
                    Byte left = (offset >> 8) & 0xff;
                    Byte right = offset & 0xff;
                    code.add(left);
                    code.add(right);
                    break;
                }
            }
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
            code.add_constant(Value(true));
        } else if (string == "false") {
            code.add_constant(Value(false));
        } else {
            code.add_constant(std::stoi(string));
        }
    }

    return code;
}
