#ifndef TEST_SUITE__Z80_TEST
#define TEST_SUITE__Z80_TEST

#include <cstdint>
#include <zemux_z80/z80cpu.h>
#include "test.h"
#include "z80ex.h"

#ifndef Z80EX_ZAME_WRAPPER
    #define Z80EX_CONTEXT_PARAM Z80EX_CONTEXT* cpu,
#else
    #define Z80EX_CONTEXT_PARAM
#endif

class Z80Test : public Test, public zemux::Z80CpuCallback {
public:

    Z80Test();
    ~Z80Test();

    bool run();

    uint8_t onZ80MreqRd(uint16_t address, bool isM1);
    void onZ80MreqWr(uint16_t address, uint8_t value);
    uint8_t onZ80IorqRd(uint16_t /* port */);
    void onZ80IorqWr(uint16_t /* port */, uint8_t /* value */);

    uint8_t onOtherZ80ReadByte(Z80EX_CONTEXT_PARAM uint16_t addr, int m1_state);
    void onOtherZ80WriteByte(Z80EX_CONTEXT_PARAM uint16_t addr, uint8_t value);
    uint8_t onOtherZ80InputByte(Z80EX_CONTEXT_PARAM uint16_t port);
    void onOtherZ80OutputByte(Z80EX_CONTEXT_PARAM uint16_t port, uint8_t value);
    uint8_t onOtherZ80ReadIntVec(Z80EX_CONTEXT_PARAM);

private:

    zemux::Z80Cpu cpu;
    Z80EX_CONTEXT* otherCpu;
    uint8_t memory[0x10000];
    uint8_t otherMemory[0x10000];
    bool isFinished;
    bool isOtherFinished;

    bool execute(const char* path);
    uint16_t bdos(uint8_t* mem, uint8_t regC, uint16_t regDE, uint16_t regSP);
};


#endif
