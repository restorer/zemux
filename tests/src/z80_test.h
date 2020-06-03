#ifndef TEST_SUITE__Z80_TEST
#define TEST_SUITE__Z80_TEST

#include <cstdint>
#include <zemux_z80/z80cpu.h>
#include "test.h"

class Z80Test : public Test, public zemux::Z80CpuCallback {
public:

    Z80Test();
    bool run();

    uint8_t onZ80MreqRd(uint16_t address, bool isM1);
    void onZ80MreqWr(uint16_t address, uint8_t value);
    uint8_t onZ80IorqRd(uint16_t /* port */);
    void onZ80IorqWr(uint16_t /* port */, uint8_t /* value */);

private:

    zemux::Z80Cpu cpu;
    uint8_t memory[0x10000];
    bool isFinished;

    bool execute(const char* path);
};


#endif
