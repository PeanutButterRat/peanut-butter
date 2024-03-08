#ifndef CPBPL_VALUE_H
#define CPBPL_VALUE_H

#include <cstdint>
#include <iostream>
#include <utility>

typedef int32_t Integer;

enum Type {
    INTEGER,
    BOOLEAN,
    STRING
};

struct Value {
    void* ref = nullptr;
    Type type;

    Value(Integer number);
    Value(std::string string);
    Value(const char* string);
    explicit Value(bool boolean);

    Value(const Value& other);
    ~Value();

    [[nodiscard]] std::string get_type_string() const;

    friend std::ostream& operator<<(std::ostream& os, const Value& token);
    friend bool operator==(const Value& a, const Value& b);
    friend bool operator!=(const Value& a, const Value& b);
    friend bool operator>(const Value& a, const Value& b);
    friend bool operator<(const Value& a, const Value& b);
    friend Value operator+(const Value& a, const Value& b);
    friend Value operator-(const Value& a, const Value& b);
    friend Value operator*(const Value& a, const Value& b);
    friend Value operator/(const Value& a, const Value& b);
    friend Value operator%(const Value& a, const Value& b);
};

class InvalidOperationException : public std::exception {
    std::string reason;

public:
    InvalidOperationException(const Value& a, const Value& b, const std::string& operation) {
        auto first = a.get_type_string();
        auto second = b.get_type_string();
        this->reason = "Invalid operation (" + operation + ") between types " + first + " and " + second + ".";
    }

    InvalidOperationException(const Value& a, const std::string& operation) {
        this->reason = "Invalid operation (" + operation + ") for type " + a.get_type_string() + ".";
    }

    [[nodiscard]] const char* what() const noexcept override { return reason.c_str(); }
};

#endif // CPBPL_VALUE_H