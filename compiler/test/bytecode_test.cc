#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

#include "../include/bytecode.h"
#include "../include/compiler.h"
#include "../include/scanner.h"

TEST(BytecodeTests, Serialization) {
    const std::string filename = "TEST_SERIALIZATION_FILE.nut";
    std::ofstream outfile{filename};
    ASSERT_TRUE(outfile.is_open());

    std::string source = "let a be 3.\n"
                         "let b be 5.\n"
                         "let c be a plus b minus 4.\n"
                         "while b:\n"
                         "\tif b:\n"
                         "\t\tset b to b minus 1.";

    Compiler compiler{Scanner{source}.tokenize()};
    auto before = compiler.parse();
    before.serialize(outfile);
    outfile.close();

    std::ifstream infile{filename};
    ASSERT_TRUE(infile.is_open());

    auto after = Bytecode::deserialize(infile);
    infile.close();

    try {
        std::filesystem::remove(filename);
    } catch (const std::filesystem::filesystem_error& error) {
        std::cout << "Test file deletion error:" << error.what() << std::endl;
    }

    ASSERT_EQ(before, after);
}

TEST(BytecodeTests, LineSplitting) {
    std::string line = "\t\tthis is a   test, \t\"Hello, world!\"";
    std::vector<std::string> expected = { "this", "is", "a", "test,", "\"Hello, world!\"" };
    auto actual = split(line);
    ASSERT_EQ(expected, actual);

    line = "a b c \"this";
    expected = { "a", "b", "c", "\"this"};
    actual = split(line);
    ASSERT_EQ(expected, actual);
}