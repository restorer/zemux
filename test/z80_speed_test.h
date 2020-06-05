#ifndef TEST__Z80_SPEED_TEST
#define TEST__Z80_SPEED_TEST

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
