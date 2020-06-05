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
#include <boost/test/unit_test.hpp>
#include "z80_correctness_test.h"

// #define DUMP_EXECUTION

static const char* ZEXALL_PATH = "../test-extras/zexall.com";
static const char* ZEXDOC_PATH = "../test-extras/zexdoc.com";
static constexpr int MAX_BDOS_STRING_LEN = 128;
static const char* ERROR_PHRASE = "ERROR";
static constexpr int ERROR_PHRASE_LEN = 5;

static uint8_t testMemory[0x10000];
static uint8_t ethalonMemory[0x10000];

// NB. BOOST_TEST_MESSAGE output uint8_t as char, so cast all the uint8_t to uint16_t in output messages.

static void compareAndOutputByte(const char* name, uint8_t test, uint8_t ethalon) {
    BOOST_TEST_MESSAGE(std::hex << std::setfill('0') << std::uppercase
        << "test " << name << " " << std::setw(2) << static_cast<uint16_t>(test)
        << (test == ethalon ? " == " : " != ")
        << "ethalon " << name << " " << std::setw(2) << static_cast<uint16_t>(ethalon)
    );
}

static void compareAndOutputWord(const char* name, uint16_t test, uint16_t ethalon) {
    BOOST_TEST_MESSAGE(std::hex << std::setfill('0') << std::uppercase
        << "test " << name << " " << std::setw(4) << test
        << (test == ethalon ? " == " : " != ")
        << "ethalon " << name << " " << std::setw(4) << ethalon
    );
}

static void compareAndOutputInt(const char* name, int test, int ethalon) {
    BOOST_TEST_MESSAGE("test " << name << " " << test
        << (test == ethalon ? " == " : " != ")
        << "ethalon " << name << " " << ethalon
    );
}

static uint8_t onEthalonRead(uint16_t addr, bool /* m1 */, void* /* data */) {
    return ethalonMemory[addr];
}

static void onEthalonWrite(uint16_t addr, uint8_t val, void* /* data */) {
    ethalonMemory[addr] = val;
}

static uint8_t onEthalonIn(uint16_t /* port */, void* /* data */) {
    return 0x00;
}

static void onEthalonOut(uint16_t /* port */, uint8_t /* val */, void* /* data */) {
}

static uint8_t onEthalonReadInt(void* /* data */) {
    return 0xFF;
}

Z80CorrectnessTest::Z80CorrectnessTest() : testCpu { this } {
    ethalonCpu = __ns_Cpu__new(
        onEthalonRead, nullptr,
        onEthalonWrite, nullptr,
        onEthalonIn, nullptr,
        onEthalonOut, nullptr,
        onEthalonReadInt, nullptr
    );
}

Z80CorrectnessTest::~Z80CorrectnessTest() {
    __ns_Cpu__free(ethalonCpu);
}

uint8_t Z80CorrectnessTest::onZ80MreqRd(uint16_t address, bool /* isM1 */) {
    return testMemory[address];
}

void Z80CorrectnessTest::onZ80MreqWr(uint16_t address, uint8_t value) {
    testMemory[address] = value;
}

uint8_t Z80CorrectnessTest::onZ80IorqRd(uint16_t /* port */) {
    return 0x00;
}

void Z80CorrectnessTest::onZ80IorqWr(uint16_t /* port */, uint8_t /* value */) {
}

void Z80CorrectnessTest::execute(const char* path) {
    BOOST_TEST_MESSAGE("Loading \"" << path << "\"...");

    std::ifstream ifs;
    ifs.open(path, std::ifstream::in | std::ifstream::binary);
    BOOST_REQUIRE_MESSAGE(!ifs.fail(), "Failed to open \"" << path << "\"");

    memset(testMemory, 0, 0x10000);
    testMemory[7] = 0xF0; // Zexdoc and Zexall set SP to value stored at address 6, so SP will be 0xF000

    ifs.read(reinterpret_cast<char*>(&testMemory[0x0100]), 0x10000 - 0x0100);
    memcpy(ethalonMemory, testMemory, 0x10000);

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

    compareState();

    // Execute test

    for (;;) {
        uint16_t testPC = testCpu.regs.PC;
        uint8_t testPrefix = testCpu.getOpcodePrefix();

        if (testPrefix == 0x00 && testPC == 0x0000) {
            break;
        }

        if (testPrefix == 0x00 && testPC == 0x0005) {
            uint16_t testBC = testCpu.regs.BC;
            uint16_t testDE = testCpu.regs.DE;
            uint16_t testSP = testCpu.regs.SP;
            uint16_t ethalonSP = __ns_Cpu__get_reg(ethalonCpu, CPU_SP);

            switch (static_cast<uint8_t>(testBC)) {
                case 2: {
                    bdosChar(static_cast<uint8_t>(testDE));
                    break;
                }

                case 9: {
                    for (int addr = testDE, maxAddr = std::min(0x10000, testDE + MAX_BDOS_STRING_LEN); addr < maxAddr; ++addr) {
                        char ch = static_cast<char>(testMemory[addr]);

                        if (ch == '$') {
                            break;
                        }

                        bdosChar(ch);
                    }

                    if (bdosBuffer.find(ERROR_PHRASE) != std::string::npos) {
                        bdosFlush();
                        BOOST_FAIL("\"" << ERROR_PHRASE << "\" matched");
                    }
                }
            }

            uint16_t testRetAddr = testMemory[testSP]
                | (static_cast<uint16_t>(testMemory[static_cast<uint16_t>(testSP + 1)]) << 8);

            uint16_t ethalonRetAddr = ethalonMemory[ethalonSP]
                | (static_cast<uint16_t>(ethalonMemory[static_cast<uint16_t>(ethalonSP + 1)]) << 8);

            if (testRetAddr != ethalonRetAddr) {
                bdosFlush();

                BOOST_FAIL(std::hex << std::setfill('0') << std::uppercase
                    << "testRetAddr " << std::setw(4) << testRetAddr
                    << " != ethalonRetAddr " << std::setw(4) << ethalonRetAddr
                );
            }

            testCpu.regs.PC = testRetAddr;
            testCpu.regs.MP = testRetAddr;
            testCpu.regs.SP += 2;

            __ns_Cpu__set_reg(ethalonCpu, CPU_PC, ethalonRetAddr);
            __ns_Cpu__set_reg(ethalonCpu, CPU_MP, ethalonRetAddr);
            __ns_Cpu__set_reg(ethalonCpu, CPU_SP, ethalonSP + 2);
        }

        unsigned int testTicks = testCpu.step();
        unsigned int ethalonTicks = __ns_Cpu__tick(ethalonCpu);

        compareState();

        if (testTicks != ethalonTicks) {
            bdosFlush();
            BOOST_FAIL("testTicks " << testTicks << " != ethalonTicks " << ethalonTicks);
        }
    }

    // Check INT

    for (int im = 0; im <= 2; im++) {
        testCpu.regs.IFF1 = false;
        testCpu.regs.IFF2 = false;
        testCpu.regs.IM = im;

        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF1, 0);
        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF2, 0);
        __ns_Cpu__set_reg(ethalonCpu, CPU_IM, im);

        unsigned int testTicks = testCpu.doInt();
        unsigned int ethalonTicks = __ns_Cpu__do_int(ethalonCpu);

        compareState();

        if (testTicks != ethalonTicks) {
            bdosFlush();
            BOOST_FAIL("IM " << im << ", DI: testTicks " << testTicks << " != ethalonTicks " << ethalonTicks);
        }

        testCpu.regs.IFF1 = true;
        testCpu.regs.IFF2 = true;

        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF1, 1);
        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF2, 1);

        testTicks = testCpu.doInt();
        ethalonTicks = __ns_Cpu__do_int(ethalonCpu);

        compareState();

        if (testTicks != ethalonTicks) {
            bdosFlush();
            BOOST_FAIL("IM " << im << ", EI: testTicks " << testTicks << " != ethalonTicks " << ethalonTicks);
        }
    }

    // Check NMI

    {
        testCpu.regs.IFF1 = false;
        testCpu.regs.IFF2 = false;

        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF1, 0);
        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF2, 0);

        unsigned int testTicks = testCpu.doNmi();
        unsigned int ethalonTicks = __ns_Cpu__do_nmi(ethalonCpu);

        compareState();

        if (testTicks != ethalonTicks) {
            bdosFlush();
            BOOST_FAIL("NMI, DI: testTicks " << testTicks << " != ethalonTicks " << ethalonTicks);
        }

        testCpu.regs.IFF1 = true;
        testCpu.regs.IFF2 = true;

        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF1, 1);
        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF2, 1);

        testTicks = testCpu.doNmi();
        ethalonTicks = __ns_Cpu__do_nmi(ethalonCpu);

        compareState();

        if (testTicks != ethalonTicks) {
            bdosFlush();
            BOOST_FAIL("NMI, EI: testTicks " << testTicks << " != ethalonTicks " << ethalonTicks);
        }
    }

    bdosFlush();
}

void Z80CorrectnessTest::compareState() {
    uint16_t testBC = testCpu.regs.BC;
    uint16_t testDE = testCpu.regs.DE;
    uint16_t testHL = testCpu.regs.HL;
    uint16_t testAF = testCpu.regs.AF;
    uint16_t testIX = testCpu.regs.IX;
    uint16_t testIY = testCpu.regs.IY;
    uint16_t testSP = testCpu.regs.SP;
    uint16_t testPC = testCpu.regs.PC;
    uint16_t testMP = testCpu.regs.MP;
    uint16_t testBC_ = testCpu.regs.BC_;
    uint16_t testDE_ = testCpu.regs.DE_;
    uint16_t testHL_ = testCpu.regs.HL_;
    uint16_t testAF_ = testCpu.regs.AF_;
    uint8_t testI = testCpu.regs.I;
    uint8_t testR = testCpu.regs.R;
    bool testIFF1 = testCpu.regs.IFF1;
    bool testIFF2 = testCpu.regs.IFF2;
    int testIM = testCpu.regs.IM;
    uint8_t testPrefix = testCpu.getOpcodePrefix();
    bool testIsIntPossible = testCpu.isIntPossible();
    bool testIsNmiPossible = testCpu.isNmiPossible();

    uint16_t ethalonBC = __ns_Cpu__get_reg(ethalonCpu, CPU_BC);
    uint16_t ethalonDE = __ns_Cpu__get_reg(ethalonCpu, CPU_DE);
    uint16_t ethalonHL = __ns_Cpu__get_reg(ethalonCpu, CPU_HL);
    uint16_t ethalonAF = __ns_Cpu__get_reg(ethalonCpu, CPU_AF);
    uint16_t ethalonIX = __ns_Cpu__get_reg(ethalonCpu, CPU_IX);
    uint16_t ethalonIY = __ns_Cpu__get_reg(ethalonCpu, CPU_IY);
    uint16_t ethalonSP = __ns_Cpu__get_reg(ethalonCpu, CPU_SP);
    uint16_t ethalonPC = __ns_Cpu__get_reg(ethalonCpu, CPU_PC);
    uint16_t ethalonMP = __ns_Cpu__get_reg(ethalonCpu, CPU_MP);
    uint16_t ethalonBC_ = __ns_Cpu__get_reg(ethalonCpu, CPU_BC_);
    uint16_t ethalonDE_ = __ns_Cpu__get_reg(ethalonCpu, CPU_DE_);
    uint16_t ethalonHL_ = __ns_Cpu__get_reg(ethalonCpu, CPU_HL_);
    uint16_t ethalonAF_ = __ns_Cpu__get_reg(ethalonCpu, CPU_AF_);
    uint8_t ethalonI = static_cast<uint8_t>(__ns_Cpu__get_reg(ethalonCpu, CPU_I));
    uint8_t ethalonR = static_cast<uint8_t>(__ns_Cpu__get_reg(ethalonCpu, CPU_R));
    bool ethalonIFF1 = static_cast<bool>(__ns_Cpu__get_reg(ethalonCpu, CPU_IFF1));
    bool ethalonIFF2 = static_cast<bool>(__ns_Cpu__get_reg(ethalonCpu, CPU_IFF2));
    int ethalonIM = static_cast<int>(__ns_Cpu__get_reg(ethalonCpu, CPU_IM));
    uint8_t ethalonPrefix = ethalonCpu->prefix;
    bool ethalonIsIntPossible = static_cast<bool>(__ns_Cpu__is_int_possible(ethalonCpu));
    bool ethalonIsNmiPossible = static_cast<bool>(__ns_Cpu__is_nmi_possible(ethalonCpu));

    #ifdef DUMP_EXECUTION
        BOOST_TEST_MESSAGE(std::hex << std::setfill('0') << std::uppercase
            << "AF=" << std::setw(4) << testAF
            << " BC=" << std::setw(4) << testBC
            << " DE=" << std::setw(4) << testDE
            << " HL=" << std::setw(4) << testHL
            << " IX=" << std::setw(4) << testIX
            << " IY=" << std::setw(4) << testIY
            << " AF'=" << std::setw(4) << testAF_
            << " BC'=" << std::setw(4) << testBC_
            << " DE'=" << std::setw(4) << testDE_
            << " HL'=" << std::setw(4) << testHL_
            << "\nPC=" << std::setw(4) << testPC
            << " SP=" << std::setw(4) << testSP
            << " MP=" << std::setw(4) << testMP
            << " I=" << std::setw(2) << static_cast<uint16_t>(testI)
            << " R=" << std::setw(2) << static_cast<uint16_t>(testR)
            << std::dec
            << " IFF1=" << testIFF1
            << " IFF2=" << testIFF2
            << " IM=" << testIM
            << " INT?=" << testIsIntPossible
            << " NMI?=" << testIsNmiPossible
            << std::hex
            << " PREF=" << std::setw(2) << static_cast<uint16_t>(testPrefix)
            << "\n@PC " << std::setw(2) << static_cast<uint16_t>(testMemory[testPC])
            << " " << std::setw(2) << static_cast<uint16_t>(testMemory[static_cast<uint16_t>(testPC + 1)])
            << " " << std::setw(2) << static_cast<uint16_t>(testMemory[static_cast<uint16_t>(testPC + 2)])
            << " " << std::setw(2) << static_cast<uint16_t>(testMemory[static_cast<uint16_t>(testPC + 3)])
            << " | @SP " << std::setw(2) << static_cast<uint16_t>(testMemory[testSP])
            << " " << std::setw(2) << static_cast<uint16_t>(testMemory[static_cast<uint16_t>(testSP + 1)])
            << " " << std::setw(2) << static_cast<uint16_t>(testMemory[static_cast<uint16_t>(testSP + 2)])
            << " " << std::setw(2) << static_cast<uint16_t>(testMemory[static_cast<uint16_t>(testSP + 3)])
            << "\n"
        );
    #endif

    if (testBC != ethalonBC
        || testDE != ethalonDE
        || testHL != ethalonHL
        || testAF != ethalonAF
        || testIX != ethalonIX
        || testIY != ethalonIY
        || testSP != ethalonSP
        || testPC != ethalonPC
        || testMP != ethalonMP
        || testBC_ != ethalonBC_
        || testDE_ != ethalonDE_
        || testHL_ != ethalonHL_
        || testAF_ != ethalonAF_
        || testI != ethalonI
        || testR != ethalonR
        || testIFF1 != ethalonIFF1
        || testIFF2 != ethalonIFF2
        || testIM != ethalonIM
        || testPrefix != ethalonPrefix
        || testIsIntPossible != ethalonIsIntPossible
        || testIsNmiPossible != ethalonIsNmiPossible
    ) {
        compareAndOutputWord("AF", testAF, ethalonAF);
        compareAndOutputWord("BC", testBC, ethalonBC);
        compareAndOutputWord("DE", testDE, ethalonDE);
        compareAndOutputWord("HL", testHL, ethalonHL);
        compareAndOutputWord("IX", testIX, ethalonIX);
        compareAndOutputWord("IY", testIY, ethalonIY);
        compareAndOutputWord("SP", testSP, ethalonSP);
        compareAndOutputWord("PC", testPC, ethalonPC);
        compareAndOutputWord("MP", testMP, ethalonMP);
        compareAndOutputWord("AF'", testAF_, ethalonAF_);
        compareAndOutputWord("BC'", testBC_, ethalonBC_);
        compareAndOutputWord("DE'", testDE_, ethalonDE_);
        compareAndOutputWord("HL'", testHL_, ethalonHL_);
        compareAndOutputByte("I", testI, ethalonI);
        compareAndOutputByte("R", testR, ethalonR);
        compareAndOutputInt("IFF1", testIFF1, ethalonIFF1);
        compareAndOutputInt("IFF2", testIFF2, ethalonIFF2);
        compareAndOutputInt("IM", testIM, ethalonIM);
        compareAndOutputByte("PREF", testPrefix, ethalonPrefix);
        compareAndOutputInt("INT?", testIsIntPossible, ethalonIsIntPossible);
        compareAndOutputInt("NMI?", testIsNmiPossible, ethalonIsNmiPossible);

        bdosFlush();
        BOOST_FAIL("State comparison failed");
    }
}

void Z80CorrectnessTest::bdosChar(char ch) {
    if (ch == '\r' || ch == '\n') {
        bdosFlush();
    } else {
        bdosBuffer += ch;
    }
}

void Z80CorrectnessTest::bdosFlush() {
    if (!bdosBuffer.empty()) {
        BOOST_TEST_MESSAGE(bdosBuffer);
        bdosBuffer.clear();
    }
}

BOOST_AUTO_TEST_CASE(Z80CorrectnessZexall) {
    Z80CorrectnessTest test;
    test.execute(ZEXALL_PATH);
}

BOOST_AUTO_TEST_CASE(Z80CorrectnessZexdoc) {
    Z80CorrectnessTest test;
    test.execute(ZEXDOC_PATH);
}
