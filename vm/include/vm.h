#ifndef CPBPL_VM_H
#define CPBPL_VM_H

#include <stack>

#include "../../compiler/include/bytecode.h"

class VM {
    std::stack<byte> stack;
    Bytecode* code;
    size_t pc;

    byte next();
    byte pop();

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
