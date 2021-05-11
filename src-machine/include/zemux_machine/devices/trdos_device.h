#ifndef ZEMUX_MACHINE__TRDOS_DEVICE
#define ZEMUX_MACHINE__TRDOS_DEVICE

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
#include <zemux_core/non_copyable.h>

namespace zemux {

class TrDosDevice final : public Device, private NonCopyable {
public:

    enum EventType {
        EventLoadRom = Event::CategoryTrDos | 1,
        EventResetToTrDos = Event::CategoryTrDos | 2,
    };

    static constexpr uint16_t PORT_COMMAND_STATUS = 0x1F;
    static constexpr uint16_t PORT_TRACK = 0x3F;
    static constexpr uint16_t PORT_SECTOR = 0x5F;
    static constexpr uint16_t PORT_DATA = 0x7F;
    static constexpr uint16_t PORT_BDI_RQS = 0xFF;

    explicit TrDosDevice(Bus* bus);
    virtual ~TrDosDevice() = default;

    uint32_t getEventCategory() override;
    EventOutput onEvent(uint32_t type, EventInput input) override;

    void onAttach() override;
    void onDetach() override;

    BusMreqRdElement onConfigureMreqRd(BusMreqRdElement prev, int mreqRdLayer, uint16_t address, bool isM1) override;
    BusIorqRdElement onConfigureIorqRd(BusIorqRdElement prev, int iorqRdLayer, uint16_t port) override;
    BusIorqWrElement onConfigureIorqWr(BusIorqWrElement prev, int iorqWrLayer, uint16_t port) override;

private:

    std::array<std::unique_ptr<BusMreqRdElement[]>, Bus::LAYERS_IORQ_RD - 1> prevMreqRdMapLayers;
    std::unique_ptr<uint8_t[]> rom;

    void toggle(bool isEnabled);

    static uint8_t onMreqRdRom3DxxM1(void* data, int mreqRdLayer, uint16_t address, bool /* isM1 */);
    static uint8_t onMreqRdRomOverlay(void* data, int /* mreqRdLayer */, uint16_t address, bool /* isM1 */);
    static uint8_t onMreqRdRamM1Overlay(void* data, int mreqRdLayer, uint16_t address, bool /* isM1 */);
    static uint8_t onIorqRdStatus(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    static uint8_t onIorqRdTrack(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    static uint8_t onIorqRdSector(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    static uint8_t onIorqRdData(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    static uint8_t onIorqRdRqs(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    static void onIorqWrCommand(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    static void onIorqWrTrack(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    static void onIorqWrSector(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    static void onIorqWrData(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    static void onIorqWrBdi(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
};

}

#endif
