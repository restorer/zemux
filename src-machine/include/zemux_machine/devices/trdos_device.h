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

uint8_t onTrDosDeviceMreqRdRom3DxxM1(void* data, int mreqRdLayer, uint16_t address, bool /* isM1 */);
uint8_t onTrDosDeviceMreqRdRomOverlay(void* data, int /* mreqRdLayer */, uint16_t address, bool /* isM1 */);
uint8_t onTrDosDeviceMreqRdRamM1Overlay(void* data, int mreqRdLayer, uint16_t address, bool /* isM1 */);
uint8_t onTrDosDeviceIorqRdStatus(void* data, int /* iorqRdLayer */, uint16_t /* port */);
uint8_t onTrDosDeviceIorqRdTrack(void* data, int /* iorqRdLayer */, uint16_t /* port */);
uint8_t onTrDosDeviceIorqRdSector(void* data, int /* iorqRdLayer */, uint16_t /* port */);
uint8_t onTrDosDeviceIorqRdData(void* data, int /* iorqRdLayer */, uint16_t /* port */);
uint8_t onTrDosDeviceIorqRdRqs(void* data, int /* iorqRdLayer */, uint16_t /* port */);
void onTrDosDeviceIorqWrCommand(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
void onTrDosDeviceIorqWrTrack(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
void onTrDosDeviceIorqWrSector(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
void onTrDosDeviceIorqWrData(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
void onTrDosDeviceIorqWrBdi(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);

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

    BusMreqRdElement onConfigureMreqRd(BusMreqRdElement prev, int mreqRdLayer, uint16_t address, bool isM1) override;
    BusIorqRdElement onConfigureIorqRd(BusIorqRdElement prev, int iorqRdLayer, uint16_t port) override;
    BusIorqWrElement onConfigureIorqWr(BusIorqWrElement prev, int iorqWrLayer, uint16_t port) override;

private:

    std::array<std::unique_ptr<BusMreqRdElement[]>, Bus::LAYERS_IORQ_RD - 1> prevMreqRdMapLayers;
    std::unique_ptr<uint8_t[]> rom;

    void toggle(bool isEnabled);

    uint8_t onMreqRdRom3DxxM1(int mreqRdLayer, uint16_t address);
    uint8_t onMreqRdRomOverlay(uint16_t address);
    uint8_t onMreqRdRamM1Overlay(int mreqRdLayer, uint16_t address);
    uint8_t onIorqRdStatus();
    uint8_t onIorqRdTrack();
    uint8_t onIorqRdSector();
    uint8_t onIorqRdData();
    uint8_t onIorqRdRqs();
    void onIorqWrCommand(uint8_t value);
    void onIorqWrTrack(uint8_t value);
    void onIorqWrSector(uint8_t value);
    void onIorqWrData(uint8_t value);
    void onIorqWrBdi(uint8_t value);

    friend uint8_t onTrDosDeviceMreqRdRom3DxxM1(void* data, int mreqRdLayer, uint16_t address, bool /* isM1 */);
    friend uint8_t onTrDosDeviceMreqRdRomOverlay(void* data, int /* mreqRdLayer */, uint16_t address, bool /* isM1 */);
    friend uint8_t onTrDosDeviceMreqRdRamM1Overlay(void* data, int mreqRdLayer, uint16_t address, bool /* isM1 */);
    friend uint8_t onTrDosDeviceIorqRdStatus(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    friend uint8_t onTrDosDeviceIorqRdTrack(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    friend uint8_t onTrDosDeviceIorqRdSector(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    friend uint8_t onTrDosDeviceIorqRdData(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    friend uint8_t onTrDosDeviceIorqRdRqs(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    friend void onTrDosDeviceIorqWrCommand(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    friend void onTrDosDeviceIorqWrTrack(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    friend void onTrDosDeviceIorqWrSector(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    friend void onTrDosDeviceIorqWrData(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    friend void onTrDosDeviceIorqWrBdi(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
};

}

#endif
