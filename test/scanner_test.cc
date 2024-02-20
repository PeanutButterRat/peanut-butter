#include <gtest/gtest.h>

#include "../include/scanner.h"


TEST(ScannerTests, AdditionFunction) {
    std::string source = "define foo with parameters a, b, c as:\n"
                         "\tlet x be 5.\n"
                         "\tlet y be 10.\n"
                         "\treturn x plus y.";

    std::vector<Token> expected {
            {DEFINE, "define", 1},
            {IDENTIFIER, "foo", 1},
            {WITH, "with", 1},
            {PARAMETERS, "parameters", 1},
            {IDENTIFIER, "a", 1},
            {COMMA, ",", 1},
            {IDENTIFIER, "b", 1},
            {COMMA, ",", 1},
            {IDENTIFIER, "c", 1},
            {AS, "as", 1},
            {COLON, ":", 1},

            {BLOCK_START, "", 2},
            {LET, "let", 2},
            {IDENTIFIER, "x", 2},
            {BE, "be", 2},
            {NUMBER, "5", 2},
            {PERIOD, ".", 2},

            {LET, "let", 3},
            {IDENTIFIER, "y", 3},
            {BE, "be", 3},
            {NUMBER, "10", 3},
            {PERIOD, ".", 3},

            {RETURN, "return", 4},
            {IDENTIFIER, "x", 4},
            {ADDITION, "plus", 4},
            {IDENTIFIER, "y", 4},
            {PERIOD, ".", 4},

            {BLOCK_END, "", 5}
    };

    std::vector<Token> actual = Scanner{source}.tokenize();

    ASSERT_EQ(expected, actual);
}

TEST(ScannerTests, Blocks) {
    std::string source = "line 1.\n"
                         "\tline 2.\n"
                         "\t\tline 3.\n"
                         "\tline 4.";

    std::vector<Token> expected {
            {IDENTIFIER, "line", 1},
            {NUMBER, "1", 1},
            {PERIOD, ".", 1},

            {BLOCK_START, "", 2},
            {IDENTIFIER, "line", 2},
            {NUMBER, "2", 2},
            {PERIOD, ".", 2},

            {BLOCK_START, "", 3},
            {IDENTIFIER, "line", 3},
            {NUMBER, "3", 3},
            {PERIOD, ".", 3},

            {BLOCK_END, "", 4},
            {IDENTIFIER, "line", 4},
            {NUMBER, "4", 4},
            {PERIOD, ".", 4},

            {BLOCK_END, "", 5},
    };

    std::vector<Token> actual = Scanner{source}.tokenize();

    ASSERT_EQ(expected, actual);
}

TEST(ScannerTests, NoParametersFunction) {
    std::string source = "define function as:\n"
                         "\tlet a be true.\n"
                         "\treturn a.\n";

    std::vector<Token> expected {
            {DEFINE, "define", 1},
            {IDENTIFIER, "function", 1},
            {AS, "as", 1},
            {COLON, ":", 1},

            {BLOCK_START, "", 2},
            {LET, "let", 2},
            {IDENTIFIER, "a", 2},
            {BE, "be", 2},
            {TRUE, "true", 2},
            {PERIOD, ".", 2},

            {RETURN, "return", 3},
            {IDENTIFIER, "a", 3},
            {PERIOD, ".", 3},

            {BLOCK_END, "", 4},
    };

    std::vector<Token> actual = Scanner{source}.tokenize();

    ASSERT_EQ(expected, actual);
}
