#include <iostream>

#include "../include/vm.h"

VM::VM(Bytecode* code) {
    this->code = code;
    pc = 0;
}

void VM::run() {
    while (pc < code->bytes.size()) {
        Byte opcode = next();

        switch (opcode) {
            case OP_CONSTANT: {
                auto index = next();
                push(code->get_constant(index));
                break;
            }
            case OP_MODULO: {
                auto right = pop();
                auto left = pop();
                push(left % right);
                break;
            }
            case OP_MULTIPLY: {
                auto right = pop();
                auto left = pop();
                push(left * right);
                break;
            }
            case OP_DIVIDE: {
                auto right = pop();
                auto left = pop();
                push(left / right);
                break;
            }
            case OP_ADD: {
                auto right = pop();
                auto left = pop();
                push(left + right);
                break;
            }
            case OP_SUBTRACT: {
                auto right = pop();
                auto left = pop();
                push(left - right);
                break;
            }
            case OP_PRINT: {
                std::cout << pop() << std::endl;
                break;
            }
            case OP_ASSIGMENT: {
                auto index = next();
                std::string identifier =  code->get_constant(index).string();

                if (globals.find(identifier) != globals.end()) {
                    throw RuntimeException("Redefinition of " + identifier  + "' is not allowed.");
                }

                globals.emplace(identifier, pop());
            }
            break;
            case OP_IDENTIFIER: {
                auto index = next();
                std::string identifier =  code->get_constant(index).string();

                auto it = globals.find(identifier);
                if (it == globals.end()) {
                    throw RuntimeException("'" + identifier  + "' is undefined.");
                }

                push(it->second);
            }
            default:
                std::cout << "Unknown opcode: " << opcode << std::endl;
        }
    }
}

Byte VM::next() {
    return code->bytes[pc++];
}

Value VM::pop() {
    auto top = stack.top();
    stack.pop();
    return top;
}

void VM::push(const Value& value) {
    stack.push(value);
}
