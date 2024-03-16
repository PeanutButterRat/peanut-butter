#ifndef CPBPL_VALUE_H
#define CPBPL_VALUE_H

#include <cstdint>
#include <iostream>
#include <utility>

typedef int32_t Integer;
typedef std::string String;
typedef bool Boolean;

enum Type {
    INTEGER,
    BOOLEAN,
    STRING
};

struct Value {
    union {
        String* string;
        Integer integer;
        Boolean boolean;
    } as;

    Type type;

    Value(Integer number);
    Value(String string);
    Value(const char* string);
    explicit Value(Boolean boolean);

    [[nodiscard]] Integer integer() const;
    [[nodiscard]] String string() const;
    [[nodiscard]] Boolean boolean() const;

    Value(const Value& other);
    ~Value();

    [[nodiscard]] std::string get_type_string() const;
    [[nodiscard]] bool truthy() const;
    [[nodiscard]] bool falsey() const;

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

class BadCastException : public std::exception {
    std::string reason;

public:
    BadCastException(const Value& value, const std::string& cast) {
        this->reason = "Could not interpret value of type " + value.get_type_string() + " as type " + cast;
    }

    [[nodiscard]] const char* what() const noexcept override { return reason.c_str(); }
};


#endif // CPBPL_VALUE_H
