#ifndef TEST_SUITE__Z80_CORRECTNESS_TEST
#define TEST_SUITE__Z80_CORRECTNESS_TEST

#include <cstdint>
#include <zemux_z80/z80cpu.h>
#include "test.h"

extern "C" {
    #include <lib_z80/cpu.h>
}

class Z80CorrectnessTest : public Test, public zemux::Z80CpuCallback {
public:

    Z80CorrectnessTest();
    ~Z80CorrectnessTest();

    const char* name();
    bool run();

    uint8_t onZ80MreqRd(uint16_t address, bool /* isM1 */);
    void onZ80MreqWr(uint16_t address, uint8_t value);
    uint8_t onZ80IorqRd(uint16_t /* port */);
    void onZ80IorqWr(uint16_t /* port */, uint8_t /* value */);

private:

    zemux::Z80Cpu testCpu;
    s_Cpu* ethalonCpu;

    bool execute(const char* path);
    bool performCheck();
    void compareAndOutputB(const char* regName, uint8_t testReg, uint8_t ethalonReg);
    void compareAndOutputW(const char* regName, uint16_t testReg, uint16_t ethalonReg);
    void compareAndOutputI(const char* regName, int testReg, int ethalonReg);
};


#endif
