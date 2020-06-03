#include <iostream>
#include <fstream>
#include <cstring>
#include <zemux_z80/z80cpu.h>
#include "z80_test.h"

static uint8_t onOtherZ80ReadByteProxy(Z80EX_CONTEXT_PARAM uint16_t addr, int m1_state, void* userData) {
    return static_cast<Z80Test*>(userData)->onOtherZ80ReadByte(addr, m1_state);
}

static void onOtherZ80WriteByteProxy(Z80EX_CONTEXT_PARAM uint16_t addr, uint8_t value, void* userData) {
    static_cast<Z80Test*>(userData)->onOtherZ80WriteByte(addr, value);
}

static uint8_t onOtherZ80InputByteProxy(Z80EX_CONTEXT_PARAM uint16_t port, void* userData) {
    return static_cast<Z80Test*>(userData)->onOtherZ80InputByte(port);
}

static void onOtherZ80OutputByteProxy(Z80EX_CONTEXT_PARAM uint16_t port, uint8_t value, void* userData) {
    static_cast<Z80Test*>(userData)->onOtherZ80OutputByte(port, value);
}

static uint8_t onOtherZ80ReadIntVecProxy(Z80EX_CONTEXT_PARAM void* userData) {
    return static_cast<Z80Test*>(userData)->onOtherZ80ReadIntVec();
}

Z80Test::Z80Test() : cpu { this } {
    otherCpu = z80ex_create(
        onOtherZ80ReadByteProxy,
        this,
        onOtherZ80WriteByteProxy,
        this,
        onOtherZ80InputByteProxy,
        this,
        onOtherZ80OutputByteProxy,
        this,
        onOtherZ80ReadIntVecProxy,
        this
    );
}

Z80Test::~Z80Test() {
    z80ex_destroy(otherCpu);
}

bool Z80Test::run() {
    return /* execute("extras/zexdoc.com") && */ execute("extras/zexall.com");
}

uint8_t Z80Test::onZ80MreqRd(uint16_t address, bool isM1) {
    if (isM1 && address == 0x0005) {
        address = bdos(memory, cpu.regs.C, cpu.regs.DE, cpu.regs.SP);

        cpu.regs.PC = address;
        cpu.regs.MP = address;
        cpu.regs.SP += 2;
    }

    if (isM1 && address == 0x0000) {
        isFinished = true;
    }

    return memory[address];
}

void Z80Test::onZ80MreqWr(uint16_t address, uint8_t value) {
    memory[address] = value;
}

uint8_t Z80Test::onZ80IorqRd(uint16_t /* port */) {
    return 0x00;
}

void Z80Test::onZ80IorqWr(uint16_t /* port */, uint8_t /* value */) {
}

uint8_t Z80Test::onOtherZ80ReadByte(Z80EX_CONTEXT_PARAM uint16_t addr, int m1_state) {
    if (m1_state && addr == 0x0005) {
        addr = bdos(otherMemory,
            static_cast<uint8_t>(z80ex_get_reg(otherCpu, regBC)),
            z80ex_get_reg(otherCpu, regDE),
            z80ex_get_reg(otherCpu, regSP)
        );

        z80ex_set_reg(otherCpu, regPC, addr);
        z80ex_set_reg(otherCpu, regMP, addr);
        z80ex_set_reg(otherCpu, regSP, z80ex_get_reg(otherCpu, regSP) + 2);
    }

    if (m1_state && addr == 0x0000) {
        isOtherFinished = true;
    }

    return otherMemory[addr];
}

void Z80Test::onOtherZ80WriteByte(Z80EX_CONTEXT_PARAM uint16_t addr, uint8_t value) {
    otherMemory[addr] = value;
}

uint8_t Z80Test::onOtherZ80InputByte(Z80EX_CONTEXT_PARAM uint16_t /* port */) {
    return 0x00;
}

void Z80Test::onOtherZ80OutputByte(Z80EX_CONTEXT_PARAM uint16_t /* port */, uint8_t /* value */) {
}

uint8_t Z80Test::onOtherZ80ReadIntVec(Z80EX_CONTEXT_PARAM) {
    return 0xFF;
}

bool Z80Test::execute(const char* path) {
    std::cout << "Z80: Executing \"" << path << "\"...\n";

    std::ifstream ifs;
    ifs.open(path, std::ifstream::in | std::ifstream::binary);

    if (ifs.fail()) {
        std::cout << "Z80: Failed to open file\n";
        return false;
    }

    memset(memory, 0, 0x10000);
    ifs.read(reinterpret_cast<char*>(&memory[0x0100]), 0x10000 - 0x0100);
    memcpy(otherMemory, memory, 0x10000);

    cpu.reset();
    cpu.regs.PC = 0x0100;
    cpu.regs.SP = 0xF000;

    z80ex_reset(otherCpu);
    z80ex_set_reg(otherCpu, regPC, 0x0100);
    z80ex_set_reg(otherCpu, regSP, 0xF000);

    isFinished = false;
    isOtherFinished = false;

    while (!isFinished && !isOtherFinished) {
        std::cout << "Z80: PC = " << cpu.regs.PC << ", otherPC = " << z80ex_get_reg(otherCpu, regPC) << "\n";

        // cpu.step();
        z80ex_step(otherCpu);
    }

    return true;
}

uint16_t Z80Test::bdos(uint8_t* mem, uint8_t regC, uint16_t regDE, uint16_t regSP) {
    switch (regC) {
        case 2: {
            std::cout << static_cast<char>(static_cast<uint8_t>(regDE));
            break;
        }

        case 9: {
            for (uint16_t address = regDE; address < 0xF000; ++address) {
                char ch = static_cast<char>(mem[address]);

                if (ch == '$') {
                    break;
                }

                std::cout << ch;
            }
        }
    }

    return mem[regSP] | (static_cast<uint16_t>(mem[regSP + 1]) << 8);
}
