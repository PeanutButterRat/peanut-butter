#include <iostream>

#include "../include/vm.h"

VM::VM(Bytecode code) {
    this->scope = new Scope;
    this->code = code;
    pc = 0;
}

void VM::run() {
    while (pc < code.bytes.size()) {
        Byte opcode = next();

        switch (opcode) {
            case OP_CONSTANT: {
                auto index = next();
                push(code.get_constant(index));
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
            case OP_EQUALITY: {
                auto right = pop();
                auto left = pop();
                push(Value((bool) (left == right)));
                break;
            }
            case OP_GREATER_THAN: {
                auto right = pop();
                auto left = pop();
                push(Value((bool) (left > right)));
                break;
            }
            case OP_LESS_THAN: {
                auto right = pop();
                auto left = pop();
                push(Value((bool) (left < right)));
                break;
            }
            case OP_PRINT: {
                std::cout << pop() << std::endl;
                break;
            }
            case OP_DECLARATION: {
                auto index = next();
                std::string identifier =  code.get_constant(index).string();
                scope->define(identifier, pop());
                break;
            }
            case OP_ASSIGMENT: {
                auto index = next();
                std::string identifier =  code.get_constant(index).string();
                scope->assign(identifier, pop());
                break;
            }
            case OP_IDENTIFIER: {
                auto index = next();
                std::string identifier =  code.get_constant(index).string();
                push(scope->resolve(identifier));
                break;
            }
            case OP_PUSH_SCOPE: {
                push_scope();
                break;
            }
            case OP_POP_SCOPE: {
                pop_scope();
                break;
            }
            case OP_JUMP_IF_FALSE: {
                auto offset = next_short();
                if (pop().falsey()) {
                    pc += offset;
                }
                break;
            }
            case OP_JUMP: {
                auto offset = next_short();
                pc += offset;
                break;
            }
            case OP_AND: {
                auto right = pop();
                auto left = pop();
                push(Value((bool) (left.truthy() && right.truthy())));
                break;
            }
            case OP_OR: {
                auto right = pop();
                auto left = pop();
                push(Value((bool) (left.truthy() || right.truthy())));
                break;
            }
            default:
                std::cout << "Unknown opcode: " << (int) opcode << std::endl;
        }
    }
}

Byte VM::next() {
    return code.bytes[pc++];
}

Value VM::pop() {
    if (stack.empty()) {
        throw RuntimeException("Attempted to pop from empty stack.");
    }
    auto top = stack.top();
    stack.pop();
    return top;
}

void VM::push(const Value& value) {
    stack.push(value);
}

void VM::push_scope() {
    scope = scope->push();
}

void VM::pop_scope() {
    auto shallower = scope->pop();
    if (!shallower) {
        throw RuntimeException("Attempted to remove global scope.");
    }
    delete scope;
    scope = shallower;
}

Short VM::next_short() {
    Short value = 0;
    value |= next() << 8;
    value |= next();
    return value;
}
