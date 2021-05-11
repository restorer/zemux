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
#include <vector>
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/chronometer.h>

namespace zemux {

class Z80Chip;
class Tape;
class Device;
class MemoryDevice;
class ExtPortDevice;

struct BusMreqRdElement {
    uint8_t (* callback)(void* data, int mreqRdLayer, uint16_t address, bool isM1);
    void* data;
};

struct BusMreqWrElement {
    void (* callback)(void* data, int mreqWrLayer, uint16_t address, uint8_t value);
    void* data;
};

struct BusIorqRdElement {
    uint8_t (* callback)(void* data, int iorqRdLayer, uint16_t port);
    void* data;
};

struct BusIorqWrElement {
    void (* callback)(void* data, int iorqWrLayer, uint16_t port, uint8_t value);
    void* data;
};

class Bus final : private NonCopyable {
public:

    static constexpr int ELEMENTS_MREQ_RD_BASE = 0x10000;
    static constexpr int ELEMENTS_MREQ_RD_FULL = ELEMENTS_MREQ_RD_BASE * 2;
    static constexpr int ELEMENTS_MREQ_RD_BASE_MASK = 0xFFFF;
    static constexpr int ELEMENTS_MREQ_WR = 0x10000;
    static constexpr int ELEMENTS_IORQ_RD = 0x10000;
    static constexpr int ELEMENTS_IORQ_WR = 0x10000;

    static constexpr int OVERLAY_MREQ_RD_TRDOS = 0b0000'0001;
    static constexpr int OVERLAY_MREQ_RD_TRDOS_LMASK = 0b0000'0000;
    static constexpr int OVERLAY_MREQ_RD_TRDOS_UMASK = 0b1111'1110;
    static constexpr int OVERLAY_IORQ_RD_TRDOS = 0b0000'0001;
    static constexpr int OVERLAY_IORQ_WR_TRDOS = 0b0000'0001;

    static constexpr int LAYERS_MREQ_RD = 2;
    static constexpr int LAYERS_MREQ_WR = 1;
    static constexpr int LAYERS_IORQ_RD = 2;
    static constexpr int LAYERS_IORQ_WR = 2;

    BusMreqRdElement* mreqRdMap;
    BusMreqWrElement* mreqWrMap;
    BusIorqRdElement* iorqRdMap;
    BusIorqWrElement* iorqWrMap;

    std::array<std::unique_ptr<BusMreqRdElement[]>, LAYERS_MREQ_RD> mreqRdMapLayers;
    std::array<std::unique_ptr<BusMreqWrElement[]>, LAYERS_MREQ_WR> mreqWrMapLayers;
    std::array<std::unique_ptr<BusIorqRdElement[]>, LAYERS_IORQ_RD> iorqRdMapLayers;
    std::array<std::unique_ptr<BusIorqWrElement[]>, LAYERS_IORQ_WR> iorqWrMapLayers;

    std::vector<Device*> attachedDevices;

    EventEmitter* hostEmitter = nullptr;
    MemoryDevice* memoryDevice = nullptr;
    ExtPortDevice* extPortDevice = nullptr;
    Tape* tape = nullptr;

    Bus(Z80Chip* cpu, ChronometerNarrow* cpuChronometer);
    ~Bus() = default;

    uint32_t getFrameTicksPassed();
    void setCpuClockRatio(int rate);
    void performReconfigure();
    void performReset();

    void toggleMreqRdOverlay(int mreqRdOverlay, bool isEnabled);
    void toggleMreqWrOverlay(int mreqWrOverlay, bool isEnabled);
    void toggleIorqRdOverlay(int iorqRdOverlay, bool isEnabled);
    void toggleIorqWrOverlay(int iorqWrOverlay, bool isEnabled);

    ZEMUX_FORCE_INLINE int getMreqRdLayer() {
        return mreqRdLayer;
    }

    ZEMUX_FORCE_INLINE int getMreqWrLayer() {
        return mreqWrLayer;
    }

    ZEMUX_FORCE_INLINE int getIorqRdLayer() {
        return iorqRdLayer;
    }

    ZEMUX_FORCE_INLINE int getIorqWrLayer() {
        return iorqWrLayer;
    }

private:

    Z80Chip* cpu;
    ChronometerNarrow* cpuChronometer;

    int mreqRdLayer;
    int mreqWrLayer;
    int iorqRdLayer;
    int iorqWrLayer;

    void resetLayers();

    static uint8_t onMreqRdFallback(void* /* data */, int /* mreqRdLayer */, uint16_t /* address */, bool /* isM1 */);
    static void onMreqWrFallback(void* /* data */, int /* mreqWrLayer */, uint16_t /* address */, uint8_t /* value */);
    static uint8_t onIorqRdFallback(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    static void onIorqWrFallback(void* /* data */, int /* iorqWrLayer */, uint16_t /* port */, uint8_t /* value */);
};

ZEMUX_FORCE_INLINE int busLayerWithoutOverlay(int layer, int lmask, int umask) {
    return ((layer & umask) >> 1) | (layer & lmask);
}

}

#endif
