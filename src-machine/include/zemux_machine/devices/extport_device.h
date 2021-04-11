#ifndef ZEMUX_MACHINE__EXTPORT_DEVICE
#define ZEMUX_MACHINE__EXTPORT_DEVICE

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

#include "bus.h"
#include "device.h"
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <cstdint>

namespace zemux {

// Non-static to be able to include this header in other units.
void onExtPortDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);

class ExtPortDevice final : public Device, private NonCopyable {
public:

    enum EventType {
        EventSetConfiguration = Event::CategoryExtPort | 1,
        EventGetConfiguration = Event::CategoryExtPort | 2,
    };

    struct Configuration {
        bool isOldMode;
        bool isTurboEnabled;
    };

    static constexpr uint16_t PORT_EFF7 = 0xEFF7;

    static constexpr uint8_t BIT_16_COLORS = 1; // 16 colors (4bits per pixel)
    static constexpr uint8_t BIT_512x192 = 2; // 512x192 monochrome
    static constexpr uint8_t BIT_128_LOCK = 4; // disable memory above 128k
    static constexpr uint8_t BIT_RAM_MAP_ROM = 8; // first ram page (0 bank) maps to rom
    static constexpr uint8_t BIT_GIGASCREEN_OR_TURBO = 16; // gigascreen or turbo (0 - on, 1 - off) pentagon sl 2.2
    static constexpr uint8_t BIT_MULTICOLOR = 32; // hardware multicolor
    static constexpr uint8_t BIT_384x304 = 64; // 384x304 mode
    static constexpr uint8_t BIT_CMOS = 128; // cmos

    static constexpr uint8_t BIT_OLD_MULTICOLOR = 1; // hardware multicolor
    static constexpr uint8_t BIT_OLD_CACHE_RO = 8; // read only cache
    static constexpr uint8_t BIT_OLD_16_COLORS = 32; // 16 colors (4bits per pixel)

    explicit ExtPortDevice(Bus* bus);
    virtual ~ExtPortDevice() = default;

    uint32_t getEventCategory() override;
    EventOutput onEvent(uint32_t type, EventInput input) override;

    void onAttach() override;
    void onDetach() override;
    BusIorqWrElement onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) override;
    void onReset() override;

    ZEMUX_FORCE_INLINE bool is16Colors() {
        return portEFF7 & (isOldMode ? BIT_OLD_16_COLORS : BIT_16_COLORS);
    }

    ZEMUX_FORCE_INLINE bool is512x192() {
        return portEFF7 & BIT_512x192;
    }

    ZEMUX_FORCE_INLINE bool is128Lock() {
        return portEFF7 & BIT_128_LOCK;
    }

    ZEMUX_FORCE_INLINE bool isRamMapRom() {
        return portEFF7 & BIT_RAM_MAP_ROM;
    }

    ZEMUX_FORCE_INLINE bool isGigascreen() {
        return !isTurboEnabled && (portEFF7 & BIT_GIGASCREEN_OR_TURBO);
    }

    ZEMUX_FORCE_INLINE bool isMulticolor() {
        return portEFF7 & (isOldMode ? BIT_OLD_MULTICOLOR : BIT_MULTICOLOR);
    }

    ZEMUX_FORCE_INLINE bool is384x304() {
        return portEFF7 & BIT_384x304;
    }

    ZEMUX_FORCE_INLINE bool isCmos() {
        return portEFF7 & BIT_CMOS;
    }

private:

    uint8_t portEFF7 = 0;
    bool isOldMode = false;
    bool isTurboEnabled = false;

    void onIorqWr(uint8_t value);

    friend void onExtPortDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);
};

}

#endif
