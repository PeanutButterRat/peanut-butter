#include <iostream>

#include "../include/scanner.h"
#include "../include/compiler.h"

int main() {
    std::string source = "5 plus 10 minus 3 times 12";
    Scanner scanner{source};
    std::vector<Token> tokens = scanner.tokenize();

    Compiler compiler{tokens};
    Bytecode bytecode = compiler.parse();
    bytecode.disassemble();

    return 0;
}
