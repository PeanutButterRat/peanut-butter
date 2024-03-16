#include "../include/vm.h"

int main() {
    // let y be 5 plus 10 minus 3 times 12 mod 6.
    Bytecode code {
            {
                OP_CONSTANT, 0,
                OP_DECLARATION, 1,
                OP_IDENTIFIER, 1,
                OP_JUMP_IF_FALSE, 0, 12,
                OP_ENSCOPE,
                OP_IDENTIFIER, 1,
                OP_CONSTANT, 2,
                OP_SUBTRACT,
                OP_ASSIGMENT, 1,
                OP_DESCOPE,
                OP_JUMP, 255, 0b11101111,
                OP_IDENTIFIER, 1,
                OP_PRINT,
            },
            {5, "a", 1}
    };

    VM test{&code};
    test.run();

    return 0;
}
