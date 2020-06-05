#ifndef TEST_SUITE__Z80_SPEED_TEST
#define TEST_SUITE__Z80_SPEED_TEST

#include <cstdint>
#include <zemux_z80/z80cpu.h>
#include "test.h"

extern "C" {
    #include <lib_z80/cpu.h>
}

class Z80SpeedTest : public Test, public zemux::Z80CpuCallback {
public:

    Z80SpeedTest();
    ~Z80SpeedTest();

    const char* name();
    bool run();

    uint8_t onZ80MreqRd(uint16_t address, bool /* isM1 */);
    void onZ80MreqWr(uint16_t address, uint8_t value);
    uint8_t onZ80IorqRd(uint16_t /* port */);
    void onZ80IorqWr(uint16_t /* port */, uint8_t /* value */);

private:

    zemux::Z80Cpu testCpu;
    s_Cpu* ethalonCpu;

    bool prepare(const char* path);
    void executeTest();
    void executeEthalon();
    uint16_t bdos(uint16_t bc, uint16_t de, uint16_t sp);
};


#endif
