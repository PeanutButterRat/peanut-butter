#include <gtest/gtest.h>

#include "../include/scanner.h"

std::vector<Token> tokenize(std::string& source) {
    Scanner scanner{source};
    return scanner.tokenize();
}

TEST(ScannerTests, Blocks) {
    std::string source = "line 1.\n"
                         "\tline 2.\n"
                         "\t\tline 3.\n"
                         "\tline 4.";

    std::vector<Token> expected{
            {IDENTIFIER,    "line", 1},
            {NUMBER,        "1",    1},
            {PERIOD,        ".",    1},

            {BLOCK_START,   "",     2},
            {IDENTIFIER,    "line", 2},
            {NUMBER,        "2",    2},
            {PERIOD,        ".",    2},

            {BLOCK_START,   "",     3},
            {IDENTIFIER,    "line", 3},
            {NUMBER,        "3",    3},
            {PERIOD,        ".",    3},

            {BLOCK_END,     "",     4},
            {IDENTIFIER,    "line", 4},
            {NUMBER,        "4",    4},
            {PERIOD,        ".",    4},

            {BLOCK_END,     "",     5},
            {END_OF_STREAM, "",     5}
    };

    std::vector<Token> actual = tokenize(source);

    ASSERT_EQ(expected, actual);
}

TEST(ScannerTests, WhileLoop) {
    std::string source = "let x be 3.\n"
                         "while x:\n"
                         "\tlet y be 2.\n";

    std::vector<Token> expected{
            {LET,           "let",   1},
            {IDENTIFIER,    "x",     1},
            {BE,            "be",    1},
            {NUMBER,        "3",     1},
            {PERIOD,        ".",     1},

            {WHILE,         "while", 2},
            {IDENTIFIER,    "x",     2},
            {COLON,         ":",     2},
            {BLOCK_START,   "",      3},

            {LET,           "let",   3},
            {IDENTIFIER,    "y",     3},
            {BE,            "be",    3},
            {NUMBER,        "2",     3},
            {PERIOD,        ".",     3},

            {BLOCK_END,     "",      4},
            {END_OF_STREAM, "",      4}
    };

    std::vector<Token> actual = tokenize(source);

    ASSERT_EQ(expected, actual);
}
