#include <iostream>
#include <fstream>
#include <filesystem>

#include "../include/vm.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: pbpl <filename>" << std::endl;
        return 1;
    }

    fs::path infilepath = argv[1];
    if (infilepath.extension() != ".nut") {
        std::cout << "Error: filename must have '.nut' file extension." << std::endl;
        return 2;
    } else if (!fs::exists(infilepath)) {
        std::cout << "Error: " << infilepath << " does not exist." << std::endl;
        return 3;
    }

    std::ifstream infile{infilepath};
    if (!infile.is_open()) {
        std::cout << "Error: could not open input file " << infilepath << "." << std::endl;
        return 4;
    }

    try {
        auto code = Bytecode::deserialize(infile);
        infile.close();
        VM machine{code};
        machine.run();
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << std::endl;
        infile.close();
        return -1;
    }

    return 0;
}
