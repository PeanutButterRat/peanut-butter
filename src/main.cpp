#include <iostream>

#include "../include/scanner.h"

int main() {
    std::string text = "THIS IS A TEST STRING define as plus over \" string \" (comment) times";
    Scanner test{text};

    try {
        for (const auto& token : test.tokenize()) {
            std::cout << token << std::endl;
        }

    } catch (ScannerException& e){
        std::cout << e.what();
    }

    return 0;
}
