#include <iostream>

#include "../include/vm.h"

VM::VM(Bytecode* code) {
    this->code = code;
    pc = 0;
}

void VM::run() {
    while (pc < code->bytes.size()) {
        byte opcode = next();

        switch (opcode) {
            case OP_CONSTANT: {
                auto index = next();
                stack.push(code->get_constant(index));
                break;
            }
            case OP_MODULO: {
                auto right = pop();
                auto left = pop();
                stack.push(left % right);
                break;
            }
            case OP_MULTIPLY: {
                auto right = pop();
                auto left = pop();
                stack.push(left * right);
                break;
            }
            case OP_DIVIDE: {
                auto right = pop();
                auto left = pop();
                stack.push(left / right);
                break;
            }
            case OP_ADD: {
                auto right = pop();
                auto left = pop();
                stack.push(left + right);
                break;
            }
            case OP_SUBTRACT: {
                auto right = pop();
                auto left = pop();
                stack.push(left - right);
                break;
            }
            case OP_PRINT: {
                std::cout << pop() << std::endl;
                break;
            }
            default:
                std::cout << "Unknown opcode: " << opcode << std::endl;
        }
    }
}

byte VM::next() {
    return code->bytes[pc++];
}

Value VM::pop() {
    auto top = stack.top();
    stack.pop();
    return top;
}
