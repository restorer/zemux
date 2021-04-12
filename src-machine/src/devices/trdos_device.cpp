// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

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

#include "devices/trdos_device.h"
#include "devices/memory_device.h"
#include <zemux_core/data_io.h>

namespace zemux {

uint8_t onTrDosDeviceMreqRdRom3DxxM1(void* data, int mreqRdLayer, uint16_t address, bool /* isM1 */) {
    return static_cast<TrDosDevice*>(data)->onMreqRdRom3DxxM1(mreqRdLayer, address);
}

uint8_t onTrDosDeviceMreqRdRomOverlay(void* data, int /* mreqRdLayer */, uint16_t address, bool /* isM1 */) {
    return static_cast<TrDosDevice*>(data)->onMreqRdRomOverlay(address);
}

uint8_t onTrDosDeviceMreqRdRamM1Overlay(void* data, int mreqRdLayer, uint16_t address, bool /* isM1 */) {
    return static_cast<TrDosDevice*>(data)->onMreqRdRamM1Overlay(mreqRdLayer, address);
}

uint8_t onTrDosDeviceIorqRdStatus(void* data, int /* iorqRdLayer */, uint16_t /* port */) {
    return static_cast<TrDosDevice*>(data)->onIorqRdStatus();
}

uint8_t onTrDosDeviceIorqRdTrack(void* data, int /* iorqRdLayer */, uint16_t /* port */) {
    return static_cast<TrDosDevice*>(data)->onIorqRdTrack();
}

uint8_t onTrDosDeviceIorqRdSector(void* data, int /* iorqRdLayer */, uint16_t /* port */) {
    return static_cast<TrDosDevice*>(data)->onIorqRdSector();
}

uint8_t onTrDosDeviceIorqRdData(void* data, int /* iorqRdLayer */, uint16_t /* port */) {
    return static_cast<TrDosDevice*>(data)->onIorqRdData();
}

uint8_t onTrDosDeviceIorqRdRqs(void* data, int /* iorqRdLayer */, uint16_t /* port */) {
    return static_cast<TrDosDevice*>(data)->onIorqRdRqs();
}

void onTrDosDeviceIorqWrCommand(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value) {
    static_cast<TrDosDevice*>(data)->onIorqWrCommand(value);
}

void onTrDosDeviceIorqWrTrack(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value) {
    static_cast<TrDosDevice*>(data)->onIorqWrTrack(value);
}

void onTrDosDeviceIorqWrSector(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value) {
    static_cast<TrDosDevice*>(data)->onIorqWrSector(value);
}

void onTrDosDeviceIorqWrData(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value) {
    static_cast<TrDosDevice*>(data)->onIorqWrData(value);
}

void onTrDosDeviceIorqWrBdi(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value) {
    static_cast<TrDosDevice*>(data)->onIorqWrBdi(value);
}

TrDosDevice::TrDosDevice(Bus* bus) : Device(bus) {
    for (int i = 0; i < Bus::LAYERS_MREQ_RD - 1; ++i) {
        prevMreqRdMapLayers[i].reset(new BusMreqRdElement[Bus::ELEMENTS_MREQ_RD_BASE]);
    }

    rom.reset(new uint8_t[MemoryDevice::SIZE_BANK]);
}

uint32_t TrDosDevice::getEventCategory() {
    return Event::CategoryTrDos;
}

EventOutput TrDosDevice::onEvent(uint32_t type, EventInput input) {
    switch (type) {
        case EventLoadRom:
            static_cast<DataReader*>(input.pointer)->readBlock(&rom[0], MemoryDevice::SIZE_BANK);
            return EventOutput { .isHandled = true };

        case EventResetToTrDos:
            bus->performReset();

            if (bus->memoryDevice != nullptr && bus->memoryDevice->tryEnableBasic48Rom()) {
                toggle(true);
            }

            return EventOutput { .isHandled = true };

        default:
            return EventOutput {};
    }
}

BusMreqRdElement TrDosDevice::onConfigureMreqRd(BusMreqRdElement prev, int mreqRdLayer, uint16_t address, bool isM1) {
    // prevMreqRdMapLayers:
    //
    // 0x0000 ... 0x3CFF -- unused
    // 0x3D00 ... 0x3DFF -- onMreqRdRamM1Overlay (isM1 && !isOverlay)
    // 0x3E00 ... 0x3FFF -- unused
    // 0x4000 ... 0xFFFF -- onMreqRdRom3DxxM1 (isM1 && isOverlay)

    if (mreqRdLayer & Bus::OVERLAY_MREQ_RD_TRDOS) {
        if (address < MemoryDevice::SIZE_BANK) {
            return BusMreqRdElement { .callback = onTrDosDeviceMreqRdRomOverlay, .data = this };
        }

        if (isM1) {
            auto layer = busLayerWithoutOverlay(mreqRdLayer,
                    Bus::OVERLAY_MREQ_RD_TRDOS_LMASK,
                    Bus::OVERLAY_MREQ_RD_TRDOS_UMASK);

            prevMreqRdMapLayers[layer][address] = prev;
            return BusMreqRdElement { .callback = onTrDosDeviceMreqRdRamM1Overlay, .data = this };
        }

        return prev;
    }

    if (isM1 && (address & 0xFF00) == 0x3D00) {
        auto layer = busLayerWithoutOverlay(mreqRdLayer,
                Bus::OVERLAY_MREQ_RD_TRDOS_LMASK,
                Bus::OVERLAY_MREQ_RD_TRDOS_UMASK);

        prevMreqRdMapLayers[layer][address] = prev;
        return BusMreqRdElement { .callback = onTrDosDeviceMreqRdRom3DxxM1, .data = this };
    }

    return prev;
}

BusIorqRdElement TrDosDevice::onConfigureIorqRd(BusIorqRdElement prev, int iorqRdLayer, uint16_t port) {
    if (!(iorqRdLayer & Bus::OVERLAY_IORQ_RD_TRDOS)) {
        return prev;
    }

    switch (port) {
        case PORT_COMMAND_STATUS:
            return BusIorqRdElement { .callback = onTrDosDeviceIorqRdStatus, .data = this };

        case PORT_TRACK:
            return BusIorqRdElement { .callback = onTrDosDeviceIorqRdTrack, .data = this };

        case PORT_SECTOR:
            return BusIorqRdElement { .callback = onTrDosDeviceIorqRdSector, .data = this };

        case PORT_DATA:
            return BusIorqRdElement { .callback = onTrDosDeviceIorqRdData, .data = this };

        case PORT_BDI_RQS:
            return BusIorqRdElement { .callback = onTrDosDeviceIorqRdRqs, .data = this };

        default:
            return prev;
    }
}

BusIorqWrElement TrDosDevice::onConfigureIorqWr(BusIorqWrElement prev, int iorqWrLayer, uint16_t port) {
    if (!(iorqWrLayer & Bus::OVERLAY_IORQ_WR_TRDOS)) {
        return prev;
    }

    switch (port) {
        case PORT_COMMAND_STATUS:
            return BusIorqWrElement { .callback = onTrDosDeviceIorqWrCommand, .data = this };

        case PORT_TRACK:
            return BusIorqWrElement { .callback = onTrDosDeviceIorqWrTrack, .data = this };

        case PORT_SECTOR:
            return BusIorqWrElement { .callback = onTrDosDeviceIorqWrSector, .data = this };

        case PORT_DATA:
            return BusIorqWrElement { .callback = onTrDosDeviceIorqWrData, .data = this };

        case PORT_BDI_RQS:
            return BusIorqWrElement { .callback = onTrDosDeviceIorqWrBdi, .data = this };

        default:
            return prev;
    }
}

void TrDosDevice::toggle(bool isEnabled) {
    bus->toggleMreqRdOverlay(Bus::OVERLAY_MREQ_RD_TRDOS, isEnabled);
    bus->toggleIorqRdOverlay(Bus::OVERLAY_IORQ_RD_TRDOS, isEnabled);
    bus->toggleIorqWrOverlay(Bus::OVERLAY_IORQ_WR_TRDOS, isEnabled);
}

uint8_t TrDosDevice::onMreqRdRom3DxxM1(int mreqRdLayer, uint16_t address) {
    if (bus->memoryDevice != nullptr && bus->memoryDevice->isBasic48Rom()) {
        toggle(true);
        return rom[address];
    }

    auto layer = busLayerWithoutOverlay(mreqRdLayer,
            Bus::OVERLAY_MREQ_RD_TRDOS_LMASK,
            Bus::OVERLAY_MREQ_RD_TRDOS_UMASK);

    auto& element = prevMreqRdMapLayers[layer][address];
    return element.callback(element.data, mreqRdLayer, address, true);
}

uint8_t TrDosDevice::onMreqRdRomOverlay(uint16_t address) {
    return rom[address];
}

uint8_t TrDosDevice::onMreqRdRamM1Overlay(int mreqRdLayer, uint16_t address) {
    toggle(false);

    auto layer = busLayerWithoutOverlay(mreqRdLayer,
            Bus::OVERLAY_MREQ_RD_TRDOS_LMASK,
            Bus::OVERLAY_MREQ_RD_TRDOS_UMASK);

    auto& element = prevMreqRdMapLayers[layer][address];
    return element.callback(element.data, mreqRdLayer, address, true);
}

uint8_t TrDosDevice::onIorqRdStatus() {
    return 0xFF;
}

uint8_t TrDosDevice::onIorqRdTrack() {
    return 0xFF;
}

uint8_t TrDosDevice::onIorqRdSector() {
    return 0xFF;
}

uint8_t TrDosDevice::onIorqRdData() {
    return 0xFF;
}

uint8_t TrDosDevice::onIorqRdRqs() {
    return 0xFF;
}

void TrDosDevice::onIorqWrCommand(uint8_t /* value */) {
}

void TrDosDevice::onIorqWrTrack(uint8_t /* value */) {
}

void TrDosDevice::onIorqWrSector(uint8_t /* value */) {
}

void TrDosDevice::onIorqWrData(uint8_t /* value */) {
}

void TrDosDevice::onIorqWrBdi(uint8_t /* value */) {
}

}
