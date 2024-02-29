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
    std::string source = "5 plus 10 minus 3 times 12 mod 6";
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
                OP_MODULO
            },
            {5, 10, 3, 12, 6}
    };
    auto actual = compile(source);

    ASSERT_EQ(expected, actual);
}
