#include "../include/vm.h"

int main() {
    // let y be 5 plus 10 minus 3 times 12 mod 6.
    Bytecode code {
            {
                OP_CONSTANT, 0,
                OP_CONSTANT, 1,
                OP_ADD,
                OP_CONSTANT, 2,
                OP_SUBTRACT,
                OP_CONSTANT, 3,
                OP_MULTIPLY,
                OP_CONSTANT, 4,
                OP_MODULO,
                OP_ASSIGMENT, 5,
            },
            {5, 10, 3, 12, 6, "y"}
    };

    VM test{&code};
    test.run();

    return 0;
}
