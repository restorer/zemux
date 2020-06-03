#include <iostream>
#include "z80_test.h"

int main() {
    Z80Test z80test;

    if (!z80test.run()) {
        return 1;
    }

    return 0;
}
