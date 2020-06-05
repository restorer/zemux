#ifndef TEST__Z80_CORRECTNESS_TEST
#define TEST__Z80_CORRECTNESS_TEST

#include <cstdint>
#include <string>
#include <zemux_z80/z80cpu.h>

extern "C" {
    #include <lib_z80/cpu.h>
}

class Z80CorrectnessTest : public zemux::Z80CpuCallback {
public:

    Z80CorrectnessTest();
    ~Z80CorrectnessTest();

    uint8_t onZ80MreqRd(uint16_t address, bool /* isM1 */);
    void onZ80MreqWr(uint16_t address, uint8_t value);
    uint8_t onZ80IorqRd(uint16_t /* port */);
    void onZ80IorqWr(uint16_t /* port */, uint8_t /* value */);

    void execute(const char* path);

private:

    zemux::Z80Cpu testCpu;
    s_Cpu* ethalonCpu;
    std::string bdosBuffer;

    void compareState();
    void bdosChar(char ch);
    void bdosFlush();
};


#endif
