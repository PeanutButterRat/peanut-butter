#ifndef CPBPL_SCOPE_H
#define CPBPL_SCOPE_H

#include <unordered_map>
#include <string>

#include "../../compiler/include/value.h"

class Scope {
    std::unordered_map<std::string, Value> variables;
    Scope* deeper = nullptr;
    Scope* shallower = nullptr;

public:
    Value resolve(const std::string& identifier);
    void define(const std::string& identifier, const Value& value);
    void assign(const std::string& identifier, const Value& value);

    Scope* push();
    Scope* pop();
};

class ScopeException : public std::exception {
    std::string reason;

public:
    explicit ScopeException(const std::string& reason) {
        this->reason = reason;
    }

    [[nodiscard]] const char* what() const noexcept override { return reason.c_str(); }
};

#endif // CPBPL_SCOPE_H
