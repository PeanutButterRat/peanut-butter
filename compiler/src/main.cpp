#include "../include/scanner.h"
#include "../include/compiler.h"

int main() {
    std::string source = "let a be 5.\n"
                         "if a:\n"
                         "\tlet b be 3.\n"
                         "otherwise:\n"
                         "\tlet c be 4.";
    Scanner scanner{source};
    std::vector<Token> tokens = scanner.tokenize();

    Compiler compiler{tokens};
    Bytecode bytecode = compiler.parse();
    bytecode.disassemble();

    return 0;
}
