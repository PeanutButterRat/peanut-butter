#include "../include/scope.h"


Value Scope::resolve(const std::string &identifier) {
    auto it = variables.find(identifier);

    if (it != variables.end()) {
        return it->second;
    } else if (shallower) {
        return shallower->resolve(identifier);
    } else {
        throw ScopeException(identifier + " is undefined.");
    }
}

void Scope::define(const std::string &identifier, const Value &value) {
    if (variables.find(identifier) == variables.end()) {
        variables.emplace(identifier, value);
    } else {
        throw ScopeException(identifier + " is already defined.");
    }
}

void Scope::assign(const std::string &identifier, const Value &value) {
    auto it = variables.find(identifier);

    if (it != variables.end()) {
        it->second = value;
    } else if (shallower) {
        shallower->resolve(identifier);
    } else {
        throw ScopeException(identifier + " is undefined.");
    }
}


Scope *Scope::push() {
    auto scope = new Scope;
    deeper = scope;
    scope->shallower = this;
    return scope;
}

Scope *Scope::pop() {
    return shallower;
}
