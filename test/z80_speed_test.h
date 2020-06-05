#ifndef TEST__Z80_SPEED_TEST
#define TEST__Z80_SPEED_TEST

#include <cstdint>
#include <string>
#include <zemux_z80/z80cpu.h>

extern "C" {
    #include <lib_z80/cpu.h>
}

class Z80SpeedTest : public zemux::Z80CpuCallback {
public:

    Z80SpeedTest();
    ~Z80SpeedTest();

    void measure(const char* path);

    uint8_t onZ80MreqRd(uint16_t address, bool /* isM1 */);
    void onZ80MreqWr(uint16_t address, uint8_t value);
    uint8_t onZ80IorqRd(uint16_t /* port */);
    void onZ80IorqWr(uint16_t /* port */, uint8_t /* value */);

private:

    zemux::Z80Cpu testCpu;
    s_Cpu* ethalonCpu;
    std::string bdosBuffer;

    void prepare(const char* path);
    void executeTest();
    void executeEthalon();
    uint16_t bdos(uint16_t bc, uint16_t de, uint16_t sp);
    void bdosChar(char ch);
    void bdosFlush();
};


#endif
