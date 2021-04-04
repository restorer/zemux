#ifndef ZEMUX_MACHINE__MEMORY_DEVICE
#define ZEMUX_MACHINE__MEMORY_DEVICE

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

#include "device.h"
#include "event_handler.h"
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <cstdint>
#include <memory>

namespace zemux {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

static uint8_t onMemoryDeviceMreqRdRom(void* data, uint16_t address, bool /* isM1 */);
static uint8_t onMemoryDeviceMreqRdRamBank2(void* data, uint16_t address, bool /* isM1 */);
static uint8_t onMemoryDeviceMreqRdRamBank5(void* data, uint16_t address, bool /* isM1 */);
static uint8_t onMemoryDeviceMreqRdRamBankSel(void* data, uint16_t address, bool /* isM1 */);
static void onMemoryDeviceMreqWrRom(void* data, uint16_t address, uint8_t value);
static void onMemoryDeviceMreqWrRamBank2(void* data, uint16_t address, uint8_t value);
static void onMemoryDeviceMreqWrRamBank5(void* data, uint16_t address, uint8_t value);
static void onMemoryDeviceMreqWrRamBankSel(void* data, uint16_t address, uint8_t value);
static void onMemoryDeviceIorqWr(void* data, uint16_t port, uint8_t value);

#pragma clang diagnostic pop

class MemoryDevice final : public Device, private NonCopyable {
public:

    enum Mode {
        Mode48 = 0,
        Mode128 = 1,
        Mode512 = 2,
        Mode1024 = 3
    };

    static constexpr uint32_t EVENT_SET_MODE = EventHandler::PREFIX_MEMORY_DEVICE | 1;
    static constexpr uint32_t EVENT_LOAD_ROM_FULL = EventHandler::PREFIX_MEMORY_DEVICE | 2;
    static constexpr uint32_t EVENT_LOAD_ROM_BANK_0 = EventHandler::PREFIX_MEMORY_DEVICE | 3;
    static constexpr uint32_t EVENT_LOAD_ROM_BANK_1 = EventHandler::PREFIX_MEMORY_DEVICE | 4;

    static constexpr int SIZE_BANK = 0x4000;
    static constexpr int BANKS_ROM = 2;
    static constexpr int BANKS_RAM = 64;
    static constexpr uint16_t PORT_7FFD = 0x7FFD;

    static constexpr uint8_t MASK_WRITE_128 = 0b00111111;
    static constexpr uint8_t MASK_BANK_128 = 0b00000111;
    static constexpr uint8_t MASK_BANK_512 = 0b11000000;
    static constexpr uint8_t MASK_BANK_1024 = 0b11100000;
    static constexpr int SHIFT_BANK_512 = 3;
    static constexpr int SHIFT_BANK_1024 = 2;
    static constexpr uint8_t BIT_SCREEN_BANK_7 = 0b00001000;
    static constexpr uint8_t BIT_ROM_BANK_1 = 0b00010000;
    static constexpr uint8_t BIT_LOCK = 0b00100000;

    MemoryDevice(Bus* bus);
    virtual ~MemoryDevice() = default;

    uint32_t getEventPrefix() override;
    EventData onEvent(uint32_t action, EventData input) override;

    void onAttach() override;
    void onDetach() override;

    BusMreqRdElement onConfigureMreqRd(
            BusMreqRdElement /* prev */,
            int /* mreqRdLayer */,
            uint16_t address,
            bool /* isM1 */) override;

    BusMreqWrElement onConfigureMreqWr(BusMreqWrElement /* prev */, int /* mreqWrLayer */, uint16_t address) override;
    BusIorqWrElement onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) override;
    void onReset() override;

    void remap();

private:

    Mode mode = Mode48;
    uint8_t port7FFD = 0;
    std::unique_ptr<uint8_t[]> rom;
    std::unique_ptr<uint8_t[]> ram;
    void (MemoryDevice::* onMreqWrRomPtr)(uint16_t address, uint8_t value);
    uint8_t* romBankPtr;
    uint8_t* ramBankPtr;

    uint8_t onMreqRdRom(uint16_t address);
    uint8_t onMreqRdRamBank2(uint16_t address);
    uint8_t onMreqRdRamBank5(uint16_t address);
    uint8_t onMreqRdRamBankSel(uint16_t address);

    void onMreqWrRomNone(uint16_t /* address */, uint8_t /* value */);
    void onMreqWrRomRam(uint16_t address, uint8_t value);
    void onMreqWrRamBank2(uint16_t address, uint8_t value);
    void onMreqWrRamBank5(uint16_t address, uint8_t value);
    void onMreqWrRamBankSel(uint16_t address, uint8_t value);

    void onIorqWr(uint16_t port, uint8_t value);

    friend uint8_t onMemoryDeviceMreqRdRom(void* data, uint16_t address, bool /* isM1 */);
    friend uint8_t onMemoryDeviceMreqRdRamBank2(void* data, uint16_t address, bool /* isM1 */);
    friend uint8_t onMemoryDeviceMreqRdRamBank5(void* data, uint16_t address, bool /* isM1 */);
    friend uint8_t onMemoryDeviceMreqRdRamBankSel(void* data, uint16_t address, bool /* isM1 */);
    friend void onMemoryDeviceMreqWrRom(void* data, uint16_t address, uint8_t value);
    friend void onMemoryDeviceMreqWrRamBank2(void* data, uint16_t address, uint8_t value);
    friend void onMemoryDeviceMreqWrRamBank5(void* data, uint16_t address, uint8_t value);
    friend void onMemoryDeviceMreqWrRamBankSel(void* data, uint16_t address, uint8_t value);
    friend void onMemoryDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);
};

}

#endif
