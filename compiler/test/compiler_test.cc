#include <gtest/gtest.h>

#include "../include/scanner.h"
#include "../include/compiler.h"

Bytecode compile(std::string& source) {
    Scanner scanner{source};
    std::vector<Token> tokens = scanner.tokenize();

    Compiler compiler{tokens};
    Bytecode bytecode = compiler.parse();

    return bytecode;
}

TEST(CompilerTests, ChainedBinaryExpression) {
    std::string source = "let y be 5 plus 10 minus 3 times 12 mod 6.";
    Bytecode expected {
        {
                OP_CONSTANT, 0,
                OP_CONSTANT, 1,
                OP_ADD,
                OP_CONSTANT, 2,
                OP_SUBTRACT,
                OP_CONSTANT, 3,
                OP_MULTIPLY,
                OP_CONSTANT, 4,
                OP_MODULO,
                OP_ASSIGMENT, 5
            },
            {5, 10, 3, 12, 6, "y"}
    };
    auto actual = compile(source);

    ASSERT_EQ(expected, actual);
}

TEST(CompilerTests, InvalidExpressionException) {
    std::string source = "let y be 5 plus define";
    EXPECT_THROW(compile(source), UnexpectedTokenException);
}

TEST(CompilerTests, ControlFlowIfOtherwise) {
    std::string source = "let a be 5.\n"
                         "if a:\n"
                            "\tlet b be 3.\n"
                         "otherwise:\n"
                            "\tlet c be 4.";
    Bytecode expected {
        {
                OP_CONSTANT, 0,
                OP_ASSIGMENT, 1,
                OP_IDENTIFIER, 1,
                OP_JUMP_IF_FALSE, 7,
                OP_ENSCOPE,
                OP_CONSTANT, 2,
                OP_ASSIGMENT, 3,
                OP_DESCOPE,
                OP_JUMP, 7,
                OP_ENSCOPE,
                OP_CONSTANT, 4,
                OP_ASSIGMENT, 5,
                OP_DESCOPE
            },
            {5, "a", 3, "b", 4, "c"}
    };
    auto actual = compile(source);

    ASSERT_EQ(expected, actual);
}
