#ifndef ZEMUX_MACHINE__MEMORY_DEVICE
#define ZEMUX_MACHINE__MEMORY_DEVICE

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

#include "bus.h"
#include "device.h"
#include "event.h"
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <cstdint>
#include <memory>

namespace zemux {

class MemoryDevice final : public Device, private NonCopyable {
public:

    enum EventType {
        EventSetMode = Event::CategoryMemory | 1,
        EventGetMode = Event::CategoryMemory | 2,
        EventLoadRomFull = Event::CategoryMemory | 3,
        EventLoadRomBank0 = Event::CategoryMemory | 4,
        EventLoadRomBank1 = Event::CategoryMemory | 5
    };

    enum Mode {
        Mode48 = 0,
        Mode128 = 1,
        Mode512 = 2,
        Mode1024 = 3
    };

    static constexpr int SIZE_BANK = 0x4000;
    static constexpr int BANKS_ROM = 2;
    static constexpr int BANKS_RAM = 64;
    static constexpr uint16_t PORT_7FFD = 0x7FFD;

    static constexpr uint8_t MASK_WRITE_128 = 0b0011'1111;
    static constexpr uint8_t MASK_BANK_128 = 0b0000'0111;
    static constexpr uint8_t MASK_BANK_512 = 0b1100'0000;
    static constexpr uint8_t MASK_BANK_1024 = 0b1110'0000;
    static constexpr int SHIFT_BANK_512 = 3;
    static constexpr int SHIFT_BANK_1024 = 2;
    static constexpr uint8_t BIT_SCREEN_BANK_7 = 0b0000'1000;
    static constexpr uint8_t BIT_ROM_BANK_1 = 0b0001'0000;
    static constexpr uint8_t BIT_LOCK = 0b0010'0000;

    explicit MemoryDevice(Bus* bus);
    virtual ~MemoryDevice() = default;

    uint32_t getEventCategory() override;
    EventOutput onEvent(uint32_t type, EventInput input) override;

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
    void enableBasic48Rom();

    ZEMUX_FORCE_INLINE bool isBasic48Rom() {
        return (mode == Mode48) || (port7FFD & BIT_ROM_BANK_1);
    }

private:

    Mode mode = Mode48;
    uint8_t port7FFD = 0;
    std::unique_ptr<uint8_t[]> rom;
    std::unique_ptr<uint8_t[]> ram;
    void (MemoryDevice::* onMreqWrRomPtr)(uint16_t address, uint8_t value);
    uint8_t* romBankPtr;
    uint8_t* ramBankPtr;

    void onMreqWrRomNone(uint16_t /* address */, uint8_t /* value */);
    void onMreqWrRomRam(uint16_t address, uint8_t value);

    static uint8_t onMreqRdRom(void* data, int /* mreqRdLayer */, uint16_t address, bool /* isM1 */);
    static uint8_t onMreqRdRamBank2(void* data, int /* mreqRdLayer */, uint16_t address, bool /* isM1 */);
    static uint8_t onMreqRdRamBank5(void* data, int /* mreqRdLayer */, uint16_t address, bool /* isM1 */);
    static uint8_t onMreqRdRamBankSel(void* data, int /* mreqRdLayer */, uint16_t address, bool /* isM1 */);
    static void onMreqWrRom(void* data, int /* mreqWrLayer */, uint16_t address, uint8_t value);
    static void onMreqWrRamBank2(void* data, int /* mreqWrLayer */, uint16_t address, uint8_t value);
    static void onMreqWrRamBank5(void* data, int /* mreqWrLayer */, uint16_t address, uint8_t value);
    static void onMreqWrRamBankSel(void* data, int /* mreqWrLayer */, uint16_t address, uint8_t value);
    static void onIorqWr(void* data, int /* iorqWrLayer */, uint16_t port, uint8_t value);
};

}

#endif
