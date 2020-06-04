#include <iostream>
#include <vector>
#include <memory>
#include "test.h"
#include "z80_correctness_test.h"
#include "z80_speed_test.h"

int main() {
    std::vector<std::unique_ptr<Test>> tests;
    tests.push_back(std::move(std::make_unique<Z80CorrectnessTest>()));
    tests.push_back(std::move(std::make_unique<Z80SpeedTest>()));

    for (auto& test : tests) {
        std::cout << "[[ Running \"" << test->name() << "\" ]]\n\n";

        if (!test->run()) {
            return 1;
        }

        std::cout << "\n";
    }

    std::cout << "All tests passed\n";
    return 0;
}
