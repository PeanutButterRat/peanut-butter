#include <utility>

#include "../include/value.h"

#define AS_INTEGER(value) *static_cast<Integer*>(value.ref)
#define AS_STRING(value) *static_cast<std::string*>(value.ref)
#define AS_BOOLEAN(value) *static_cast<bool*>(value.ref)

#define SAME_TYPE_GUARD(a, b, op) if (a.type != b.type) throw InvalidOperationException(a, b, op)
#define BAD_TYPE_GUARD(a, bad_type, op) if (a.type == bad_type) throw InvalidOperationException(a, op)
#define BINARY_OPERATION(a, b, op) \
    switch (a.type) { \
        case STRING: \
            return AS_STRING(a) op AS_STRING(b); \
        case BOOLEAN: \
            return AS_BOOLEAN(a) op AS_BOOLEAN(b); \
        default: \
            return AS_INTEGER(a) op AS_INTEGER(b); \
};

Value::Value(Integer number) {
    type = INTEGER;
    ref = new Integer;
    *static_cast<Integer*>(ref) = number;
}

Value::Value(std::string string) {
    type = STRING;
    ref = new std::string;
    *static_cast<std::string*>(ref) = std::move(string);
}


Value::Value(const char *string) {
    type = STRING;
    ref = new std::string;
    *static_cast<std::string*>(ref) = std::move(std::string(string));
}


Value::Value(bool boolean) {
    type = BOOLEAN;
    ref = new bool;
    *static_cast<bool*>(ref) = boolean;
}

Value::Value(const Value &other) {
    switch (other.type) {
        case INTEGER:
            type = INTEGER;
            ref = new Integer;
            *static_cast<Integer*>(ref) = AS_INTEGER(other);
            break;
        case BOOLEAN:
            type = BOOLEAN;
            ref = new bool;
            *static_cast<bool*>(ref) = AS_BOOLEAN(other);
            break;
        default:
            type = STRING;
            ref = new std::string;
            *static_cast<std::string*>(ref) = AS_STRING(other);
            break;
    }
}


std::ostream &operator<<(std::ostream &os, const Value &value) {
    switch (value.type) {
        case INTEGER:
            return os << AS_INTEGER(value);
        case BOOLEAN:
            return os << AS_BOOLEAN(value);
        default:
            return os << AS_STRING(value);
    };
}

bool operator==(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "equality");
    BINARY_OPERATION(a, b, ==);
}

bool operator!=(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "inequality");
    BINARY_OPERATION(a, b, !=)
}

bool operator>(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "greater than");
    BAD_TYPE_GUARD(a, BOOLEAN, "greater than");
    BINARY_OPERATION(a, b, >);
}

bool operator<(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "less than");
    BAD_TYPE_GUARD(a, BOOLEAN, "less than");
    BINARY_OPERATION(a, b, <);
}

Value operator+(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "addition");
    BAD_TYPE_GUARD(a, BOOLEAN, "addition");
    BINARY_OPERATION(a, b, +);
}

Value operator-(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "subtraction");
    BAD_TYPE_GUARD(a, BOOLEAN, "subtraction");
    BINARY_OPERATION(a, b, -);
}

Value operator*(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "multiplication");
    BAD_TYPE_GUARD(a, BOOLEAN, "multiplication");
    BINARY_OPERATION(a, b, *);
}

Value operator/(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "division");
    BAD_TYPE_GUARD(a, BOOLEAN, "division");
    BINARY_OPERATION(a, b, /);
}

Value operator%(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "modulo");
    BAD_TYPE_GUARD(a, BOOLEAN, "modulo");
    BINARY_OPERATION(a, b, %);
}

std::string Value::get_type_string() const {
    switch (type) {
        case INTEGER:
            return "Integer";
        case BOOLEAN:
            return "Boolean";
        default:
            return "String";
    }
}

Value::~Value() {
    if (ref) {
        switch (type) {
            case INTEGER:
                 delete static_cast<Integer*>(ref);
                 break;
            case BOOLEAN:
                delete static_cast<bool*>(ref);
                break;
            default:
                delete static_cast<std::string*>(ref);
                break;
        }
    }
}
