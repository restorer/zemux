// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
 * MIT License (http://www.opensource.org/licenses/mit-license.php)
 *
 * Copyright (c) 2020, Viachaslau Tratsiak (aka restorer)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <iomanip>
#include <fstream>
#include <cstring>
#include <chrono>
#include <boost/test/unit_test.hpp>
#include "z80_speed_test.h"

static const char* ZEXALL_PATH = "../test-extras/zexall.com";
static constexpr int MAX_BDOS_STRING_LEN = 128;

static uint8_t memory[0x10000];

static int64_t steadyClockNowMillis() {
    using namespace std::chrono;
    return time_point_cast<milliseconds>(steady_clock::now()).time_since_epoch().count();
}

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

Z80SpeedTest::Z80SpeedTest() : testCpu { this, zemux::TypeNmos } {
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

void Z80SpeedTest::measure(const char* path) {
    prepare(path);
    BOOST_TEST_MESSAGE("Measuring ZemuX Z80 (test)...");
    int64_t startMillis = steadyClockNowMillis();
    executeTest();
    int64_t testTime = steadyClockNowMillis() - startMillis;
    BOOST_TEST_MESSAGE("ZemuX Z80 (test) passed \"" << path << "\" in " << testTime << " ms");

    prepare(path);
    BOOST_TEST_MESSAGE("Measuring Zame Z80 (ethalon)...");
    startMillis = steadyClockNowMillis();
    executeEthalon();
    int64_t ethalonTime = steadyClockNowMillis() - startMillis;
    BOOST_TEST_MESSAGE("Zame Z80 (ethalon) passed \"" << path << "\" in " << ethalonTime << " ms");

    if (testTime < ethalonTime) {
        double ratio = static_cast<double>(ethalonTime) / static_cast<double>(testTime);

        BOOST_TEST_MESSAGE("ZemuX Z80 (test) is "
                << std::setprecision(3)
                << ratio
                << "x faster then Zame Z80 (ethalon)");
    } else if (testTime == ethalonTime) {
        BOOST_TEST_MESSAGE(
                "ZemuX Z80 (test) has the same speed as Zame Z80 (ethalon), but probably this is bug in testing environment. Please retest.");
    } else {
        double ratio = static_cast<double>(testTime) / static_cast<double>(ethalonTime);

        BOOST_TEST_MESSAGE("ZemuX Z80 (test) is "
                << std::setprecision(3)
                << ratio
                << "x SLOWER then Zame Z80 (ethalon)");
    }
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

void Z80SpeedTest::prepare(const char* path) {
    BOOST_TEST_MESSAGE("Loading \"" << path << "\"...");

    std::ifstream ifs;
    ifs.open(path, std::ifstream::in | std::ifstream::binary);
    BOOST_REQUIRE_MESSAGE(!ifs.fail(), "Failed to open \"" << path << "\"");

    memset(memory, 0, 0x10000);
    memory[7] = 0xF0; // Zexdoc and Zexall set SP to value stored at address 6, so SP will be 0xF000
    ifs.read(reinterpret_cast<char*>(&memory[0x0100]), 0x10000 - 0x0100);
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

    bdosFlush();
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

    bdosFlush();
}

uint16_t Z80SpeedTest::bdos(uint16_t bc, uint16_t de, uint16_t sp) {
    switch (static_cast<uint8_t>(bc)) {
        case 2: {
            bdosChar(static_cast<uint8_t>(de));
            break;
        }

        case 9: {
            for (int addr = de, maxAddr = std::min(0x10000, de + MAX_BDOS_STRING_LEN); addr < maxAddr; ++addr) {
                char ch = static_cast<char>(memory[addr]);

                if (ch == '$') {
                    break;
                }

                bdosChar(ch);
            }
        }
    }

    return memory[sp] | (static_cast<uint16_t>(memory[static_cast<uint16_t>(sp + 1)]) << 8);
}

void Z80SpeedTest::bdosChar(char ch) {
    if (ch == '\r' || ch == '\n') {
        bdosFlush();
    } else {
        bdosBuffer += ch;
    }
}

void Z80SpeedTest::bdosFlush() {
    if (!bdosBuffer.empty()) {
        BOOST_TEST_MESSAGE(bdosBuffer);
        bdosBuffer.clear();
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

BOOST_AUTO_TEST_CASE(Z80Speed) {
    Z80SpeedTest test;
    test.measure(ZEXALL_PATH);
}

#pragma clang diagnostic pop