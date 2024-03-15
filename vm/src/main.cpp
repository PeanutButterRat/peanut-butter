#include "../include/vm.h"

int main() {
    // let y be 5 plus 10 minus 3 times 12 mod 6.
    Bytecode code {
            {
                OP_CONSTANT, 0,
                OP_CONSTANT, 1,
                OP_ADD,
                OP_ENSCOPE,
                OP_DECLARATION, 2,
                OP_IDENTIFIER, 2,
                OP_CONSTANT, 3,
                OP_ADD,
                OP_PRINT,
                OP_DESCOPE
            },
            {5, 10, "a", 20}
    };

    VM test{&code};
    test.run();

    return 0;
}
