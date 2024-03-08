#include <gtest/gtest.h>

#include "../include/vm.h"

TEST(VirtualMachineTests, IntegerAddStringException) {
    Bytecode code {
            {
                    OP_CONSTANT, 0,
                    OP_CONSTANT, 1,
                    OP_ADD,
            },
            {5, "Hello"}
    };
    VM vm{&code};

    EXPECT_THROW(vm.run(), InvalidOperationException);
}

TEST(VirtualMachineTests, IntegerAddBooleanException) {
    Bytecode code {
            {
                    OP_CONSTANT, 0,
                    OP_CONSTANT, 1,
                    OP_ADD,
            },
            {5, Value(false)}
    };
    VM vm{&code};

    EXPECT_THROW(vm.run(), InvalidOperationException);
}

TEST(VirtualMachineTests, BooleanAddStringException) {
    Bytecode code {
            {
                    OP_CONSTANT, 0,
                    OP_CONSTANT, 1,
                    OP_ADD,
            },
            {Value(false), "Hello"}
    };
    VM vm{&code};

    EXPECT_THROW(vm.run(), InvalidOperationException);
}

TEST(VirtualMachineTests, BooleansCannotAdd) {
    Bytecode code {
            {
                    OP_CONSTANT, 0,
                    OP_CONSTANT, 1,
                    OP_ADD,
            },
            {Value(false), Value(true)}
    };
    VM vm{&code};

    EXPECT_THROW(vm.run(), InvalidOperationException);
}