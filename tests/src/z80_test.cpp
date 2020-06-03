#include <iostream>
#include <fstream>
#include <cstring>
#include <zemux_z80/z80cpu.h>
#include "z80_test.h"

Z80Test::Z80Test() : cpu { this } {
}

bool Z80Test::run() {
    return /* execute("extras/zexdoc.com") && */ execute("extras/zexall.com");
}

uint8_t Z80Test::onZ80MreqRd(uint16_t address, bool isM1) {
    if (address != 0x0005 || !isM1) {
        if (address == 0x0000 && isM1) {
            isFinished = true;
        }

        return memory[address];
    }

    switch (cpu.regs.C) {
        case 2: {
            std::cout << static_cast<char>(cpu.regs.C);
            break;
        }

        case 9: {
            for (uint16_t address = cpu.regs.DE; address < 0xF000; ++address) {
                char ch = static_cast<char>(memory[address]);

                if (ch == '$') {
                    break;
                }

                std::cout << ch;
            }
        }
    }

    cpu.regs.PC = memory[cpu.regs.SP] | (static_cast<uint16_t>(memory[cpu.regs.SP + 1]) << 8);
    cpu.regs.MP = cpu.regs.PC;
    cpu.regs.SP += 2;

    return memory[cpu.regs.PC];
}

void Z80Test::onZ80MreqWr(uint16_t address, uint8_t value) {
    memory[address] = value;
}

uint8_t Z80Test::onZ80IorqRd(uint16_t /* port */) {
    return 0;
}

void Z80Test::onZ80IorqWr(uint16_t /* port */, uint8_t /* value */) {
}

bool Z80Test::execute(const char* path) {
    std::cout << "Z80: Executing \"" << path << "\"...\n";
    memset(memory, 0, 0x10000);

    std::ifstream ifs;
    ifs.open(path, std::ifstream::in | std::ifstream::binary);

    if (ifs.fail()) {
        std::cout << "Z80: Failed to open file\n";
        return false;
    }

    ifs.read(reinterpret_cast<char*>(&memory[0x0100]), 0x10000 - 0x0100);

    cpu.reset();
    cpu.regs.PC = 0x0100;
    cpu.regs.SP = 0xF000;

    isFinished = false;

    while (!isFinished) {
        std::cout << "Z80: PC = " << cpu.regs.PC << "\n";
        cpu.step();
    }

    return true;
}
