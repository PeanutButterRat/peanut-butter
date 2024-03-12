#ifndef CPBPL_VM_H
#define CPBPL_VM_H

#include <stack>
#include <map>

#include "../../compiler/include/bytecode.h"

class VM {
    std::stack<Value> stack;
    std::map<std::string, Value> globals;
    Bytecode* code;
    size_t pc;

    Byte next();
    void push(const Value& value);
    Value pop();

public:
    explicit VM(Bytecode* code);
    void run();
};

class RuntimeException : public std::exception {
    std::string reason;

public:
    explicit RuntimeException(const std::string& reason) { this->reason = reason; }
    [[nodiscard]] const char* what() const noexcept override { return reason.c_str(); }
};

#endif //CPBPL_VM_H
