#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "z80_test.h"

// #define DUMP_EXECUTION

static constexpr int MAX_BDOS_STRING_LEN = 128;

static const char* ERROR_PHRASE = "ERROR";
static constexpr int ERROR_PHRASE_LEN = 5;

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
    return execute("extras/zexdoc.com") && execute("extras/zexall.com");
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

    if (!performCheck()) {
        return false;
    }

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
                    std::cout << static_cast<char>(static_cast<uint8_t>(testDE));
                    break;
                }

                case 9: {
                    for (int addr = testDE, maxAddr = std::min(0x10000, testDE + MAX_BDOS_STRING_LEN); addr < maxAddr; ++addr) {
                        char ch = static_cast<char>(testMemory[addr]);

                        if (ch == '$') {
                            break;
                        }

                        std::cout << ch;
                    }

                    for (int addr = testDE, maxAddr = std::min(0x10000, testDE + MAX_BDOS_STRING_LEN - ERROR_PHRASE_LEN); addr < maxAddr; ++addr) {
                        if (static_cast<char>(testMemory[addr]) == '$') {
                            break;
                        }

                        bool isMatched = true;

                        for (int i = 0; i < ERROR_PHRASE_LEN; ++i) {
                            if (ERROR_PHRASE[i] != testMemory[static_cast<uint16_t>(addr + i)]) {
                                isMatched = false;
                                break;
                            }
                        }

                        if (isMatched) {
                            std::cout << "\n[ Z80 ] \"" << ERROR_PHRASE << "\" matched\n";
                            return false;
                        }
                    }
                }
            }

            uint16_t testRetAddress = testMemory[testSP]
                | (static_cast<uint16_t>(testMemory[static_cast<uint16_t>(testSP + 1)]) << 8);

            uint16_t ethalonRetAddress = ethalonMemory[ethalonSP]
                | (static_cast<uint16_t>(ethalonMemory[static_cast<uint16_t>(ethalonSP + 1)]) << 8);

            if (testRetAddress != ethalonRetAddress) {
                std::cout << std::hex << std::uppercase
                    << "\n[ Z80 ] testRetAddress "
                    << std::setw(4) << testRetAddress
                    << " != ethalonRetAddress "
                    << std::setw(4) << ethalonRetAddress
                    << "\n";

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

        if (!performCheck()) {
            return false;
        }

        if (testTicks != ethalonTicks) {
            std::cout << "\n[ Z80 ] testTicks " << testTicks << " != ethalonTicks " << ethalonTicks << "\n";
            return false;
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

        if (!performCheck()) {
            return false;
        }

        if (testTicks != ethalonTicks) {
            std::cout << "\n[ Z80 ] IM " << im << ", DI: testTicks " << testTicks << " != ethalonTicks " << ethalonTicks << "\n";
            return false;
        }

        performCheck();

        testCpu.regs.IFF1 = true;
        testCpu.regs.IFF2 = true;

        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF1, 1);
        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF2, 1);

        testTicks = testCpu.doInt();
        ethalonTicks = __ns_Cpu__do_int(ethalonCpu);

        if (!performCheck()) {
            return false;
        }

        if (testTicks != ethalonTicks) {
            std::cout << "\n[ Z80 ] IM " << im << ", EI: testTicks " << testTicks << " != ethalonTicks " << ethalonTicks << "\n";
            return false;
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

        if (!performCheck()) {
            return false;
        }

        if (testTicks != ethalonTicks) {
            std::cout << "\n[ Z80 ] NMI, DI: testTicks " << testTicks << " != ethalonTicks " << ethalonTicks << "\n";
            return false;
        }

        performCheck();

        testCpu.regs.IFF1 = true;
        testCpu.regs.IFF2 = true;

        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF1, 1);
        __ns_Cpu__set_reg(ethalonCpu, CPU_IFF2, 1);

        testTicks = testCpu.doNmi();
        ethalonTicks = __ns_Cpu__do_nmi(ethalonCpu);

        if (!performCheck()) {
            return false;
        }

        if (testTicks != ethalonTicks) {
            std::cout << "\n[ Z80 ] NMI, EI: testTicks " << testTicks << " != ethalonTicks " << ethalonTicks << "\n";
            return false;
        }
    }

    std::cout << "\n";
    return true;
}

bool Z80Test::performCheck() {
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

    std::cout << std::hex << std::uppercase
        << "[ Z80 ] AF=" << std::setw(4) << testAF
        << " BC=" << std::setw(4) << testBC
        << " DE=" << std::setw(4) << testDE
        << " HL=" << std::setw(4) << testHL
        << " IX=" << std::setw(4) << testIX
        << " IY=" << std::setw(4) << testIY
        << " AF'=" << std::setw(4) << testAF_
        << " BC'=" << std::setw(4) << testBC_
        << " DE'=" << std::setw(4) << testDE_
        << " HL'=" << std::setw(4) << testHL_
        << "\n[ Z80 ] PC=" << std::setw(4) << testPC
        << " SP=" << std::setw(4) << testSP
        << " MP=" << std::setw(4) << testMP
        << " I=" << std::setw(2) << testI
        << " R=" << std::setw(2) << testR
        << std::dec
        << " IFF1=" << testIFF1
        << " IFF2=" << testIFF2
        << " IM=" << testIM
        << " INT?=" << testIsIntPossible
        << " NMI?=" << testIsNmiPossible
        << std::hex
        << " PREF=" << std::setw(2) << testPrefix
        << "\n[ Z80 ] @PC " << std::setw(2) << testMemory[testPC]
        << " " << std::setw(2) << testMemory[static_cast<uint16_t>(testPC + 1)]
        << " " << std::setw(2) << testMemory[static_cast<uint16_t>(testPC + 2)]
        << " " << std::setw(2) << testMemory[static_cast<uint16_t>(testPC + 3)]
        << " | @SP " << std::setw(2) << testMemory[testSP]
        << " " << std::setw(2) << testMemory[static_cast<uint16_t>(testSP + 1)]
        << " " << std::setw(2) << testMemory[static_cast<uint16_t>(testSP + 2)]
        << " " << std::setw(2) << testMemory[static_cast<uint16_t>(testSP + 3)]
        << "\n\n";

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
        compareAndOutputW("AF", testAF, ethalonAF);
        compareAndOutputW("BC", testBC, ethalonBC);
        compareAndOutputW("DE", testDE, ethalonDE);
        compareAndOutputW("HL", testHL, ethalonHL);
        compareAndOutputW("IX", testIX, ethalonIX);
        compareAndOutputW("IY", testIY, ethalonIY);
        compareAndOutputW("SP", testSP, ethalonSP);
        compareAndOutputW("PC", testPC, ethalonPC);
        compareAndOutputW("MP", testSP, ethalonMP);
        compareAndOutputW("AF'", testAF_, ethalonAF_);
        compareAndOutputW("BC'", testBC_, ethalonBC_);
        compareAndOutputW("DE'", testDE_, ethalonDE_);
        compareAndOutputW("HL'", testHL_, ethalonHL_);
        compareAndOutputB("I", testI, ethalonI);
        compareAndOutputB("R", testR, ethalonR);
        compareAndOutputI("IFF1", testIFF1, ethalonIFF1);
        compareAndOutputI("IFF2", testIFF2, ethalonIFF2);
        compareAndOutputI("IM", testIM, ethalonIM);
        compareAndOutputB("PREF", testPrefix, ethalonPrefix);
        compareAndOutputI("INT?", testIsIntPossible, ethalonIsIntPossible);
        compareAndOutputI("NMI?", testIsNmiPossible, ethalonIsNmiPossible);

        return false;
    }

    return true;
}

void Z80Test::compareAndOutputB(const char* regName, uint8_t testReg, uint8_t ethalonReg) {
    std::cout << std::hex << std::uppercase
        << "[ Z80 ] test " << regName << " " << std::setw(2) << testReg
        << (testReg == ethalonReg ? " == " : " != ")
        << "ethalon " << regName << " " << std::setw(2) << ethalonReg
        << "\n";
}

void Z80Test::compareAndOutputW(const char* regName, uint16_t testReg, uint16_t ethalonReg) {
    std::cout << std::hex << std::uppercase
        << "[ Z80 ] test " << regName << " " << std::setw(4) << testReg
        << (testReg == ethalonReg ? " == " : " != ")
        << "ethalon " << regName << " " << std::setw(4) << ethalonReg
        << "\n";
}

void Z80Test::compareAndOutputI(const char* regName, int testReg, int ethalonReg) {
    std::cout << "[ Z80 ] test " << regName << " " << testReg
        << (testReg == ethalonReg ? " == " : " != ")
        << "ethalon " << regName << " " << ethalonReg
        << "\n";
}
