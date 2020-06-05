#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <chrono>
#include "z80_speed_test.h"

static const char* ZEXALL_PATH = "extras/zexall.com";
static constexpr int MAX_BDOS_STRING_LEN = 128;
static uint8_t memory[0x10000];

static uint8_t onEthalonRead(uint16_t addr, bool /* m1 */, void* /* data */) {
    return memory[addr];
}

static void onEthalonWrite(uint16_t addr, uint8_t val, void* /* data */) {
    memory[addr] = val;
}

static uint8_t onEthalonIn(uint16_t /* port */, void* /* data */) {
    return 0x00;
}

static void onEthalonOut(uint16_t /* port */, uint8_t /* val */, void* /* data */) {
}

static uint8_t onEthalonReadInt(void* /* data */) {
    return 0xFF;
}

static int64_t steadyClockNowMillis() {
    using namespace std::chrono;
    return time_point_cast<milliseconds>(steady_clock::now()).time_since_epoch().count();
}

Z80SpeedTest::Z80SpeedTest() : testCpu { this } {
    ethalonCpu = __ns_Cpu__new(
        onEthalonRead, nullptr,
        onEthalonWrite, nullptr,
        onEthalonIn, nullptr,
        onEthalonOut, nullptr,
        onEthalonReadInt, nullptr
    );
}

Z80SpeedTest::~Z80SpeedTest() {
    __ns_Cpu__free(ethalonCpu);
}

const char* Z80SpeedTest::name() {
    return "Z80 Speed";
}

bool Z80SpeedTest::run() {
    if (!prepare(ZEXALL_PATH)) {
        return false;
    }

    std::cout << "Measuring ZemuX Z80 (test)...\n";
    int64_t startMillis = steadyClockNowMillis();
    executeTest();
    int64_t testTime = steadyClockNowMillis() - startMillis;
    std::cout << "ZemuX Z80 (test) passed \"" << ZEXALL_PATH << "\" in " << testTime << " ms\n";

    if (!prepare("extras/zexall.com")) {
        return false;
    }

    std::cout << "Measuring Zame Z80 (ethalon)...\n";
    startMillis = steadyClockNowMillis();
    executeEthalon();
    int64_t ethalonTime = steadyClockNowMillis() - startMillis;
    std::cout << "Zame Z80 (ethalon) passed \"" << ZEXALL_PATH << "\" in " << ethalonTime << " ms\n";

    std::cout << "\nZemuX Z80 (test) - " << testTime << " ms\n"
        << "Zame Z80 (ethalon) - " << ethalonTime << " ms\n";

    return true;
}

uint8_t Z80SpeedTest::onZ80MreqRd(uint16_t address, bool /* isM1 */) {
    return memory[address];
}

void Z80SpeedTest::onZ80MreqWr(uint16_t address, uint8_t value) {
    memory[address] = value;
}

uint8_t Z80SpeedTest::onZ80IorqRd(uint16_t /* port */) {
    return 0x00;
}

void Z80SpeedTest::onZ80IorqWr(uint16_t /* port */, uint8_t /* value */) {
}

bool Z80SpeedTest::prepare(const char* path) {
    std::cout << "Loading \"" << path << "\"...\n";

    std::ifstream ifs;
    ifs.open(path, std::ifstream::in | std::ifstream::binary);

    if (ifs.fail()) {
        std::cout << "Failed to open file\n";
        return false;
    }

    memset(memory, 0, 0x10000);
    memory[7] = 0xF0; // Zexdoc and Zexall set SP to value stored at address 6, so SP will be 0xF000
    ifs.read(reinterpret_cast<char*>(&memory[0x0100]), 0x10000 - 0x0100);

    return true;
}

void Z80SpeedTest::executeTest() {
    testCpu.reset();
    testCpu.regs.BC = 0xFFFF;
    testCpu.regs.DE = 0xFFFF;
    testCpu.regs.HL = 0xFFFF;
    testCpu.regs.AF = 0xFFFF;
    testCpu.regs.IX = 0xFFFF;
    testCpu.regs.IY = 0xFFFF;
    testCpu.regs.SP = 0xFFFF;
    testCpu.regs.PC = 0x0100;
    testCpu.regs.MP = 0x0100;
    testCpu.regs.BC_ = 0xFFFF;
    testCpu.regs.DE_ = 0xFFFF;
    testCpu.regs.HL_ = 0xFFFF;
    testCpu.regs.AF_ = 0xFFFF;

    for (;;) {
        uint16_t pc = testCpu.regs.PC;
        uint8_t prefix = testCpu.getOpcodePrefix();

        if (prefix == 0x00 && pc == 0x0000) {
            break;
        }

        if (prefix == 0x00 && pc == 0x0005) {
            uint16_t sp = testCpu.regs.SP;
            uint16_t addr = bdos(testCpu.regs.BC, testCpu.regs.DE, sp);

            testCpu.regs.PC = addr;
            testCpu.regs.MP = addr;
            testCpu.regs.SP = sp + 2;
        }

        testCpu.step();
    }

    std::cout << "\n";
}

void Z80SpeedTest::executeEthalon() {
    __ns_Cpu__reset(ethalonCpu);
    __ns_Cpu__set_reg(ethalonCpu, CPU_BC, 0xFFFF);
    __ns_Cpu__set_reg(ethalonCpu, CPU_DE, 0xFFFF);
    __ns_Cpu__set_reg(ethalonCpu, CPU_HL, 0xFFFF);
    __ns_Cpu__set_reg(ethalonCpu, CPU_AF, 0xFFFF);
    __ns_Cpu__set_reg(ethalonCpu, CPU_IX, 0xFFFF);
    __ns_Cpu__set_reg(ethalonCpu, CPU_IY, 0xFFFF);
    __ns_Cpu__set_reg(ethalonCpu, CPU_SP, 0xFFFF);
    __ns_Cpu__set_reg(ethalonCpu, CPU_PC, 0x0100);
    __ns_Cpu__set_reg(ethalonCpu, CPU_MP, 0x0100);
    __ns_Cpu__set_reg(ethalonCpu, CPU_BC_, 0xFFFF);
    __ns_Cpu__set_reg(ethalonCpu, CPU_DE_, 0xFFFF);
    __ns_Cpu__set_reg(ethalonCpu, CPU_HL_, 0xFFFF);
    __ns_Cpu__set_reg(ethalonCpu, CPU_AF_, 0xFFFF);

    for (;;) {
        uint16_t pc = __ns_Cpu__get_reg(ethalonCpu, CPU_PC);
        uint8_t prefix = ethalonCpu->prefix;

        if (prefix == 0x00 && pc == 0x0000) {
            break;
        }

        if (prefix == 0x00 && pc == 0x0005) {
            uint16_t sp = __ns_Cpu__get_reg(ethalonCpu, CPU_SP);
            uint16_t addr = bdos(__ns_Cpu__get_reg(ethalonCpu, CPU_BC), __ns_Cpu__get_reg(ethalonCpu, CPU_DE), sp);

            __ns_Cpu__set_reg(ethalonCpu, CPU_PC, addr);
            __ns_Cpu__set_reg(ethalonCpu, CPU_MP, addr);
            __ns_Cpu__set_reg(ethalonCpu, CPU_SP, sp + 2);
        }

        __ns_Cpu__tick(ethalonCpu);
    }

    std::cout << "\n";
}

uint16_t Z80SpeedTest::bdos(uint16_t bc, uint16_t de, uint16_t sp) {
    switch (static_cast<uint8_t>(bc)) {
        case 2: {
            std::cout << static_cast<char>(static_cast<uint8_t>(de));
            break;
        }

        case 9: {
            for (int addr = de, maxAddr = std::min(0x10000, de + MAX_BDOS_STRING_LEN); addr < maxAddr; ++addr) {
                char ch = static_cast<char>(memory[addr]);

                if (ch == '$') {
                    break;
                }

                std::cout << ch;
            }
        }
    }

    return memory[sp] | (static_cast<uint16_t>(memory[static_cast<uint16_t>(sp + 1)]) << 8);
}
