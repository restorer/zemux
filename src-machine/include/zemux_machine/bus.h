#ifndef ZEMUX_MACHINE__BUS
#define ZEMUX_MACHINE__BUS

/*
 * MIT License (http://www.opensource.org/licenses/mit-license.php)
 *
 * Copyright (c) 2021, Viachaslau Tratsiak (aka restorer)
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

#include "event.h"
#include <cstdint>
#include <memory>
#include <array>
#include <zemux_core/non_copyable.h>
#include <zemux_core/chronometer.h>
#include <zemux_chips/z80_chip.h>
#include <zemux_peripherals/tape.h>

namespace zemux {

class MemoryDevice;
class ExtPortDevice;

struct BusMreqRdElement {
    uint8_t (* callback)(void* data, uint16_t address, bool isM1);
    void* data;
};

struct BusMreqWrElement {
    void (* callback)(void* data, uint16_t address, uint8_t value);
    void* data;
};

struct BusIorqRdElement {
    uint8_t (* callback)(void* data, uint16_t port);
    void* data;
};

struct BusIorqWrElement {
    void (* callback)(void* data, uint16_t port, uint8_t value);
    void* data;
};

class Bus final : private NonCopyable {
public:

    static constexpr int ELEMENTS_MREQ_RD = 0x20000;
    static constexpr int ELEMENTS_MREQ_WR = 0x10000;
    static constexpr int ELEMENTS_IORQ_RD = 0x10000;
    static constexpr int ELEMENTS_IORQ_WR = 0x10000;

    static constexpr int LAYERS_MREQ_RD = 2;
    static constexpr int LAYERS_MREQ_WR = 1;
    static constexpr int LAYERS_IORQ_RD = 2;
    static constexpr int LAYERS_IORQ_WR = 2;

    static constexpr int OVERLAY_MREQ_RD_TRDOS = 1;
    static constexpr int OVERLAY_IORQ_RD_TRDOS = 1;
    static constexpr int OVERLAY_IORQ_WR_TRDOS = 1;

    BusMreqRdElement* mreqRdMap;
    BusMreqWrElement* mreqWrMap;
    BusIorqRdElement* iorqRdMap;
    BusIorqWrElement* iorqWrMap;

    std::array<std::unique_ptr<BusMreqRdElement[]>, LAYERS_MREQ_RD> mreqRdMapLayers;
    std::array<std::unique_ptr<BusMreqWrElement[]>, LAYERS_MREQ_WR> mreqWrMapLayers;
    std::array<std::unique_ptr<BusIorqRdElement[]>, LAYERS_IORQ_RD> iorqRdMapLayers;
    std::array<std::unique_ptr<BusIorqWrElement[]>, LAYERS_IORQ_WR> iorqWrMapLayers;

    EventEmitter* hostEmitter = nullptr;
    MemoryDevice* memoryDevice = nullptr;
    ExtPortDevice* extPortDevice = nullptr;
    Tape* tape = nullptr;

    Bus(Z80Chip* cpu, ChronometerNarrow* cpuChronometer);
    ~Bus() = default;

    void onReset();
    uint32_t getFrameTicksPassed();
    void setCpuClockRatio(int rate);
    void requestReconfigure();

    void toggleMreqRdOverlay(int mreqRdOverlay, bool isEnabled);
    void toggleMreqWrOverlay(int mreqWrOverlay, bool isEnabled);
    void toggleIorqRdOverlay(int iorqRdOverlay, bool isEnabled);
    void toggleIorqWrOverlay(int iorqWrOverlay, bool isEnabled);

private:

    Z80Chip* cpu;
    ChronometerNarrow* cpuChronometer;

    int mreqRdLayer;
    int mreqWrLayer;
    int iorqRdLayer;
    int iorqWrLayer;
};

}

#endif
