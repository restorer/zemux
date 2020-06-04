#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "z80_test.h"

// #define DUMP_EXECUTION

static uint8_t onEthalonReadProxy(uint16_t addr, bool m1, void* data) {
    return static_cast<Z80Test*>(data)->onEthalonRead(addr, m1);
}

static void onEthalonWriteProxy(uint16_t addr, uint8_t val, void* data) {
    static_cast<Z80Test*>(data)->onEthalonWrite(addr, val);
}

static uint8_t onEthalonInProxy(uint16_t port, void* data) {
    return static_cast<Z80Test*>(data)->onEthalonIn(port);
}

static void onEthalonOutProxy(uint16_t port, uint8_t val, void* data) {
    static_cast<Z80Test*>(data)->onEthalonOut(port, val);
}

static uint8_t onEthalonReadIntProxy(void* data) {
    return static_cast<Z80Test*>(data)->onEthalonReadInt();
}

Z80Test::Z80Test() : testCpu { this } {
    ethalonCpu = __ns_Cpu__new(
        onEthalonReadProxy, this,
        onEthalonWriteProxy, this,
        onEthalonInProxy, this,
        onEthalonOutProxy, this,
        onEthalonReadIntProxy, this
    );
}

Z80Test::~Z80Test() {
    __ns_Cpu__free(ethalonCpu);
}

bool Z80Test::run() {
    return /* execute("extras/zexdoc.com") && */ execute("extras/zexall.com");
}

uint8_t Z80Test::onZ80MreqRd(uint16_t address, bool /* isM1 */) {
    return testMemory[address];
}

void Z80Test::onZ80MreqWr(uint16_t address, uint8_t value) {
    testMemory[address] = value;
}

uint8_t Z80Test::onZ80IorqRd(uint16_t /* port */) {
    return 0x00;
}

void Z80Test::onZ80IorqWr(uint16_t /* port */, uint8_t /* value */) {
}

uint8_t Z80Test::onEthalonRead(uint16_t addr, bool /* m1 */) {
    return ethalonMemory[addr];
}

void Z80Test::onEthalonWrite(uint16_t addr, uint8_t val) {
    ethalonMemory[addr] = val;
}

uint8_t Z80Test::onEthalonIn(uint16_t /* port */) {
    return 0x00;
}

void Z80Test::onEthalonOut(uint16_t /* port */, uint8_t /* val */) {
}

uint8_t Z80Test::onEthalonReadInt() {
    return 0xFF;
}

bool Z80Test::execute(const char* path) {
    std::cout << "[ Z80 ] Executing \"" << path << "\"...\n";

    std::ifstream ifs;
    ifs.open(path, std::ifstream::in | std::ifstream::binary);

    if (ifs.fail()) {
        std::cout << "[ Z80 ] Failed to open file\n";
        return false;
    }

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

    for (;;) {
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
        bool ethalonIsIntPossible = __ns_Cpu__is_int_possible(ethalonCpu);
        bool ethalonIsNmiPossible = __ns_Cpu__is_nmi_possible(ethalonCpu);

        #ifdef DUMP_EXECUTION

        printf(
            "[ Z80 ] AF=%04X BC=%04X DE=%04X HL=%04X IX=%04X IY=%04X AF'=%04X BC'=%04X DE'=%04X HL'=%04X\n",
            testAF, testBC, testDE, testHL, testIX, testIY, testAF_, testBC_, testDE_, testHL_
        );

        printf(
            "[ Z80 ] PC=%04X SP=%04X MP=%04X I=%02X R=%02X IFF1=%d IFF2=%d IM=%d INT?=%d NMI?=%d\n",
            testPC, testSP, testMP, testI, testR, testIFF1, testIFF2, testIM, testIsIntPossible, testIsNmiPossible
        );

        printf(
            "[ Z80 ] @PC %02X %02X %02X %02X | @SP %02X %02X %02X %02X\n\n",
            testMemory[testPC],
            testMemory[static_cast<uint16_t>(testPC + 1)],
            testMemory[static_cast<uint16_t>(testPC + 2)],
            testMemory[static_cast<uint16_t>(testPC + 3)],
            testMemory[testSP],
            testMemory[static_cast<uint16_t>(testSP + 1)],
            testMemory[static_cast<uint16_t>(testSP + 2)],
            testMemory[static_cast<uint16_t>(testSP + 3)]
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
            printf("\n");
            printf("[ Z80 ] test AF %04X %s ethalon AF %04X\n", testAF, testAF == ethalonAF ? "==" : "!=", ethalonAF);
            printf("[ Z80 ] test BC %04X %s ethalon BC %04X\n", testBC, testBC == ethalonBC ? "==" : "!=", ethalonBC);
            printf("[ Z80 ] test DE %04X %s ethalon DE %04X\n", testDE, testDE == ethalonDE ? "==" : "!=", ethalonDE);
            printf("[ Z80 ] test HL %04X %s ethalon HL %04X\n", testHL, testHL == ethalonHL ? "==" : "!=", ethalonHL);
            printf("[ Z80 ] test IX %04X %s ethalon IX %04X\n", testIX, testIX == ethalonIX ? "==" : "!=", ethalonIX);
            printf("[ Z80 ] test IY %04X %s ethalon IY %04X\n", testIY, testIY == ethalonIY ? "==" : "!=", ethalonIY);
            printf("[ Z80 ] test SP %04X %s ethalon SP %04X\n", testSP, testSP == ethalonSP ? "==" : "!=", ethalonSP);
            printf("[ Z80 ] test PC %04X %s ethalon PC %04X\n", testPC, testPC == ethalonPC ? "==" : "!=", ethalonPC);
            printf("[ Z80 ] test MP %04X %s ethalon MP %04X\n", testMP, testMP == ethalonMP ? "==" : "!=", ethalonMP);
            printf("[ Z80 ] test AF_ %04X %s ethalon AF_ %04X\n", testAF_, testAF_ == ethalonAF_ ? "==" : "!=", ethalonAF_);
            printf("[ Z80 ] test BC_ %04X %s ethalon BC_ %04X\n", testBC_, testBC_ == ethalonBC_ ? "==" : "!=", ethalonBC_);
            printf("[ Z80 ] test DE_ %04X %s ethalon DE_ %04X\n", testDE_, testDE_ == ethalonDE_ ? "==" : "!=", ethalonDE_);
            printf("[ Z80 ] test HL_ %04X %s ethalon HL_ %04X\n", testHL_, testHL_ == ethalonHL_ ? "==" : "!=", ethalonHL_);
            printf("[ Z80 ] test I %02X %s ethalon I %02X\n", testI, testI == ethalonI ? "==" : "!=", ethalonI);
            printf("[ Z80 ] test R %02X %s ethalon R %02X\n", testR, testR == ethalonR ? "==" : "!=", ethalonR);
            printf("[ Z80 ] test IFF1 %d %s ethalon IFF1 %d\n", testIFF1, testIFF1 == ethalonIFF1 ? "==" : "!=", ethalonIFF1);
            printf("[ Z80 ] test IFF2 %d %s ethalon IFF2 %d\n", testIFF2, testIFF2 == ethalonIFF2 ? "==" : "!=", ethalonIFF2);
            printf("[ Z80 ] test IM %d %s ethalon IM %d\n", testIM, testIM == ethalonIM ? "==" : "!=", ethalonIM);
            printf("[ Z80 ] test PREFIX %02X %s ethalon PREFIX %02X\n", testPrefix, testPrefix == ethalonPrefix ? "==" : "!=", ethalonPrefix);

            printf(
                "[ Z80 ] test INT? %d %s ethalon INT? %d\n",
                testIsIntPossible,
                testIsIntPossible == ethalonIsIntPossible ? "==" : "!=",
                ethalonIsIntPossible
            );

            printf(
                "[ Z80 ] test NMI? %d %s ethalon NMI? %d\n",
                testIsNmiPossible,
                testIsNmiPossible == ethalonIsNmiPossible ? "==" : "!=",
                ethalonIsNmiPossible
            );

            return false;
        }

        if (testPrefix == 0x00 && testPC == 0x0000) {
            break;
        }

        if (testPrefix == 0x00 && testPC == 0x0005) {
            switch (static_cast<uint8_t>(testBC)) {
                case 2: {
                    std::cout << static_cast<char>(static_cast<uint8_t>(testDE));
                    break;
                }

                case 9: {
                    for (uint16_t chAddress = testDE; chAddress < testDE + 0x100; ++chAddress) {
                        char ch = static_cast<char>(testMemory[chAddress]);

                        if (ch == '$') {
                            break;
                        }

                        std::cout << ch;
                    }
                }
            }

            uint16_t testRetAddress = testMemory[testSP]
                | (static_cast<uint16_t>(testMemory[static_cast<uint16_t>(testSP + 1)]) << 8);

            uint16_t ethalonRetAddress = ethalonMemory[ethalonSP]
                | (static_cast<uint16_t>(ethalonMemory[static_cast<uint16_t>(ethalonSP + 1)]) << 8);

            if (testRetAddress != ethalonRetAddress) {
                printf("\n[ Z80 ] testRetAddress %04X != ethalonRetAddress %04X\n", testRetAddress, ethalonRetAddress);
                return false;
            }

            testCpu.regs.PC = testRetAddress;
            testCpu.regs.MP = testRetAddress;
            testCpu.regs.SP += 2;

            __ns_Cpu__set_reg(ethalonCpu, CPU_PC, ethalonRetAddress);
            __ns_Cpu__set_reg(ethalonCpu, CPU_MP, ethalonRetAddress);
            __ns_Cpu__set_reg(ethalonCpu, CPU_SP, ethalonSP + 2);
        }

        unsigned int testTicks = testCpu.step();
        unsigned int ethalonTicks = __ns_Cpu__tick(ethalonCpu);

        if (testTicks != ethalonTicks) {
            printf("\n[ Z80 ] testTicks %04X != ethalonTicks %04X\n", testTicks, ethalonTicks);
            return false;
        }
    }

    return true;
}
