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
                OP_DECLARATION, 5
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
                OP_DECLARATION, 1,
                OP_IDENTIFIER, 1,
                OP_JUMP_IF_FALSE, 0, 9,
                OP_PUSH_SCOPE,
                OP_CONSTANT, 2,
                OP_DECLARATION, 3,
                OP_POP_SCOPE,
                OP_JUMP, 0, 6,
                OP_PUSH_SCOPE,
                OP_CONSTANT, 4,
                OP_DECLARATION, 5,
                OP_POP_SCOPE
            },
            {5, "a", 3, "b", 4, "c"}
    };
    auto actual = compile(source);

    ASSERT_EQ(expected, actual);
}

TEST(CompilerTests, WhileLoop) {
    std::string source = "let a be 5.\n"
                         "while a:\n"
                         "\tset a to a minus 1.";
    Bytecode expected {
        {
                OP_CONSTANT, 0,
                OP_DECLARATION, 1,
                OP_IDENTIFIER, 1,
                OP_JUMP_IF_FALSE, 0, 12,
                OP_PUSH_SCOPE,
                OP_IDENTIFIER, 1,
                OP_CONSTANT, 2,
                OP_SUBTRACT,
                OP_ASSIGMENT, 1,
                OP_POP_SCOPE,
                OP_JUMP, 255, 0b11101111
            },
            {5, "a", 1}
    };
    auto actual = compile(source);

    ASSERT_EQ(expected, actual);
}
