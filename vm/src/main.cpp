#include "../include/vm.h"

int main() {
    // 5 plus 10 minus 3 times 2
    Bytecode code {
            {
                OP_CONSTANT, 0,
                OP_CONSTANT, 1,
                OP_ADD,
                OP_CONSTANT, 2,
                OP_SUBTRACT,
                OP_CONSTANT, 3,
                OP_MULTIPLY,
                OP_PRINT
            },
            {5, 10, 3, 2}
    };

    VM test{&code};
    test.run();

    return 0;
}
