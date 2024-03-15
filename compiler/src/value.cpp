#include <utility>

#include "../include/value.h"

#define SAME_TYPE_GUARD(a, b, op) if (a.type != b.type) throw InvalidOperationException(a, b, op)
#define BAD_TYPE_GUARD(a, bad_type, op) if (a.type == bad_type) throw InvalidOperationException(a, op)
#define BINARY_OPERATION(a, b, op) \
    switch (a.type) { \
        case STRING: \
            return a.string() op b.string(); \
        case BOOLEAN: \
            return a.boolean() op b.boolean(); \
        default: \
            return a.integer() op b.integer(); \
};

Value::Value(Integer number) {
    type = INTEGER;
    ref = new Integer;
    *static_cast<Integer*>(ref) = number;
}

Value::Value(String string) {
    type = STRING;
    ref = new String;
    *static_cast<String*>(ref) = std::move(string);
}


Value::Value(const char *string) {
    type = STRING;
    ref = new String;
    *static_cast<String*>(ref) = std::move(String(string));
}


Value::Value(Boolean boolean) {
    type = BOOLEAN;
    ref = new Boolean;
    *static_cast<Boolean*>(ref) = boolean;
}

Value::Value(const Value &other) {
    switch (other.type) {
        case INTEGER:
            type = INTEGER;
            ref = new Integer;
            *static_cast<Integer*>(ref) = other.integer();
            break;
        case BOOLEAN:
            type = BOOLEAN;
            ref = new Boolean;
            *static_cast<Boolean*>(ref) = other.boolean();
            break;
        default:
            type = STRING;
            ref = new String;
            *static_cast<String*>(ref) = other.string();
            break;
    }
}

Integer Value::integer() const {
    if (type != INTEGER) {
        throw BadCastException(*this, "Integer");
    }

    return *static_cast<Integer*>(ref);
}

String Value::string() const {
    if (type != STRING) {
        throw BadCastException(*this, "Integer");
    }

    return *static_cast<String*>(ref);
}

Boolean Value::boolean() const {
    if (type != BOOLEAN) {
        throw BadCastException(*this, "Integer");
    }

    return *static_cast<Boolean*>(ref);
}



std::ostream &operator<<(std::ostream &os, const Value &value) {
    switch (value.type) {
        case INTEGER:
            return os << value.integer();
        case BOOLEAN: {
            std::string string = value.boolean() ? "true" : "false";
            return os << string;
        }
        default:
            return os << value.string();
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

String Value::get_type_string() const {
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
                delete static_cast<Boolean*>(ref);
                break;
            default:
                delete static_cast<String*>(ref);
                break;
        }
    }
}

bool Value::truthy() const {
    if (!ref) {
        return false;
    }
    switch (type) {
        case INTEGER:
            return integer() != 0;
        case BOOLEAN:
            return boolean();
        default:
            return !string().empty();
    }
}

bool Value::falsey() const {
    return !truthy();
}
