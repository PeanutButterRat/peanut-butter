#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "../include/scanner.h"
#include "../include/compiler.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: cpbpl <filename>" << std::endl;
        return 1;
    }

    fs::path infilepath = argv[1];
    if (infilepath.extension() != ".pb") {
        std::cout << "Error: filename must have '.pb' file extension." << std::endl;
        return 2;
    } else if (!fs::exists(infilepath)) {
        std::cout << "Error: " << infilepath << " does not exist." << std::endl;
        return 3;
    }

    auto outfilepath = infilepath;
    outfilepath.replace_extension(".nut");
    std::ifstream infile{infilepath};
    std::ofstream outfile{outfilepath};

    if (!infile.is_open()) {
        std::cout << "Error: could not open input file " << infilepath << "." << std::endl;
        return 4;
    } else if (!outfile.is_open()) {
        std::cout << "Error: could not open output file " << outfilepath << "." << std::endl;
        return 5;
    }

    std::stringstream buffer;
    buffer << infile.rdbuf();
    auto source = buffer.str();
    infile.close();

    try {
        auto tokens = Scanner{source}.tokenize();
        auto bytecode = Compiler{tokens}.parse();
        bytecode.serialize(outfile);
        outfile.close();
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << std::endl;
        outfile.close();
        return -1;
    }

    return 0;
}
