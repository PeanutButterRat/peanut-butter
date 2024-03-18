#include "../include/value.h"

#define SAME_TYPE_GUARD(a, b, op) if (a.type != b.type) throw InvalidOperationException(a, b, op)
#define BAD_TYPE_GUARD(a, bad_type, op) if (a.type == bad_type) throw InvalidOperationException(a, op)
#define BINARY_OPERATION(a, b, op) \
    switch (a.type) { \
        case STRING: \
            return a.string() op b.string(); \
        case BOOLEAN: \
            return a.boolean() op b.boolean(); \
        case INTEGER: \
            return a.integer() op b.integer(); \
        default: \
            throw BadCastException(a, "Any"); \
};

Value::Value(Integer number) {
    type = INTEGER;
    as.integer = number;
}

Value::Value(String string) {
    type = STRING;
    as.string = new String(string);
}

Value::Value(const char *string) {
    type = STRING;
    as.string = new String;
    *as.string = string;
}

Value::Value(Boolean boolean) {
    type = BOOLEAN;
    as.boolean = boolean;
}

Value::Value(const Value &other) {
    switch (other.type) {
        case INTEGER:
            type = INTEGER;
            as.integer = other.as.integer;
            break;
        case BOOLEAN:
            type = BOOLEAN;
            as.boolean = other.as.boolean;
            break;
        case STRING:
            type = STRING;
            as.string = new String(other.string());
            break;
        default:
            throw BadCastException(other, "Any");
    }
}

Integer Value::integer() const {
    if (type != INTEGER) {
        throw BadCastException(*this, "Integer");
    }

    return as.integer;
}

String Value::string() const {
    if (type != STRING) {
        throw BadCastException(*this, "String");
    }

    return *as.string;
}

Boolean Value::boolean() const {
    if (type != BOOLEAN) {
        throw BadCastException(*this, "Boolean");
    }

    return as.boolean;
}

std::ostream &operator<<(std::ostream &os, const Value &value) {
    switch (value.type) {
        case INTEGER:
            return os << value.integer();
        case BOOLEAN: {
            std::string string = value.boolean() ? "true" : "false";
            return os << string;
        }
        case STRING:
            return os << value.string();
        default:
            throw BadCastException(value, "Any");
    };
}

Boolean operator==(const Value &a, const Value &b) {
    if (a.type != b.type) {
        return false;
    }
    BINARY_OPERATION(a, b, ==);
}

Boolean operator!=(const Value &a, const Value &b) {
    if (a.type != b.type) {
        return true;
    }
    BINARY_OPERATION(a, b, !=)
}

Boolean operator>(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "greater than");
    BAD_TYPE_GUARD(a, BOOLEAN, "greater than");
    BINARY_OPERATION(a, b, >);
}

Boolean operator<(const Value &a, const Value &b) {
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
    BAD_TYPE_GUARD(a, STRING, "subtraction");
    BINARY_OPERATION(a, b, -);
}

Value operator*(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "multiplication");
    BAD_TYPE_GUARD(a, BOOLEAN, "multiplication");
    BAD_TYPE_GUARD(a, STRING, "multiplication");
    BINARY_OPERATION(a, b, *);
}

Value operator/(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "division");
    BAD_TYPE_GUARD(a, BOOLEAN, "division");
    BAD_TYPE_GUARD(a, STRING, "division");
    BINARY_OPERATION(a, b, /);
}

Value operator%(const Value &a, const Value &b) {
    SAME_TYPE_GUARD(a, b, "modulo");
    BAD_TYPE_GUARD(a, BOOLEAN, "modulo");
    BAD_TYPE_GUARD(a, STRING, "modulo");
    BINARY_OPERATION(a, b, %);
}

String Value::get_type_string() const {
    switch (type) {
        case INTEGER:
            return "Integer";
        case BOOLEAN:
            return "Boolean";
        case STRING:
            return "String";
        default:
            return "Unknown";
    }
}

Value::~Value() {
    if (type == STRING) {
        delete as.string;
    }
}

bool Value::truthy() const {
    switch (type) {
        case INTEGER:
            return integer() != 0;
        case BOOLEAN:
            return boolean();
        case STRING:
            return !string().empty();
        default:
            throw BadCastException(*this, "Any");
    }
}

bool Value::falsey() const {
    return !truthy();
}
