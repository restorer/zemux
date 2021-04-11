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

#include "devices/memory_device.h"
#include "devices/extport_device.h"
#include <zemux_core/data_io.h>

namespace zemux {

uint8_t onMemoryDeviceMreqRdRom(void* data, uint16_t address, bool /* isM1 */) {
    return static_cast<MemoryDevice*>(data)->onMreqRdRom(address);
}

uint8_t onMemoryDeviceMreqRdRamBank2(void* data, uint16_t address, bool /* isM1 */) {
    return static_cast<MemoryDevice*>(data)->onMreqRdRamBank2(address);
}

uint8_t onMemoryDeviceMreqRdRamBank5(void* data, uint16_t address, bool /* isM1 */) {
    return static_cast<MemoryDevice*>(data)->onMreqRdRamBank5(address);
}

uint8_t onMemoryDeviceMreqRdRamBankSel(void* data, uint16_t address, bool /* isM1 */) {
    return static_cast<MemoryDevice*>(data)->onMreqRdRamBankSel(address);
}

void onMemoryDeviceMreqWrRom(void* data, uint16_t address, uint8_t value) {
    auto self = static_cast<MemoryDevice*>(data);
    (self->*(self->onMreqWrRomPtr))(address, value);
}

void onMemoryDeviceMreqWrRamBank2(void* data, uint16_t address, uint8_t value) {
    static_cast<MemoryDevice*>(data)->onMreqWrRamBank2(address, value);
}

void onMemoryDeviceMreqWrRamBank5(void* data, uint16_t address, uint8_t value) {
    static_cast<MemoryDevice*>(data)->onMreqWrRamBank5(address, value);
}

void onMemoryDeviceMreqWrRamBankSel(void* data, uint16_t address, uint8_t value) {
    static_cast<MemoryDevice*>(data)->onMreqWrRamBankSel(address, value);
}

void onMemoryDeviceIorqWr(void* data, uint16_t port, uint8_t value) {
    static_cast<MemoryDevice*>(data)->onIorqWr(port, value);
}

MemoryDevice::MemoryDevice(Bus* bus) : Device { bus } {
    rom.reset(new uint8_t[SIZE_BANK * BANKS_ROM]);
    ram.reset(new uint8_t[SIZE_BANK * BANKS_RAM]);
    remap();
}

uint32_t MemoryDevice::getEventCategory() {
    return Event::CategoryMemory;
}

EventOutput MemoryDevice::onEvent(uint32_t type, EventInput input) {
    switch (type) {
        case EventSetMode:
            mode = static_cast<Mode>(input.value);
            remap();
            return EventOutput { .isHandled = true };

        case EventGetMode:
            return EventOutput { .isHandled = true, .value = mode };

        case EventLoadRomFull:
            static_cast<DataReader*>(input.pointer)->readBlock(&rom[0], SIZE_BANK * 2);
            return EventOutput { .isHandled = true };

        case EventLoadRomBank0:
            static_cast<DataReader*>(input.pointer)->readBlock(&rom[0], SIZE_BANK);
            return EventOutput { .isHandled = true };

        case EventLoadRomBank1:
            static_cast<DataReader*>(input.pointer)->readBlock(&rom[SIZE_BANK], SIZE_BANK);
            return EventOutput { .isHandled = true };

        default:
            return EventOutput {};
    }
}

void MemoryDevice::onAttach() {
    Device::onAttach();
    bus->memoryDevice = this;
}

void MemoryDevice::onDetach() {
    bus->memoryDevice = nullptr;
    Device::onDetach();
}

BusMreqRdElement MemoryDevice::onConfigureMreqRd(
        BusMreqRdElement /* prev */,
        int /* mreqRdLayer */,
        uint16_t address,
        bool /* isM1 */) {

    if (address < SIZE_BANK) {
        return BusMreqRdElement { .callback = onMemoryDeviceMreqRdRom, .data = this };
    }

    if (address < SIZE_BANK * 2) {
        return BusMreqRdElement { .callback = onMemoryDeviceMreqRdRamBank2, .data = this };
    }

    if (address < SIZE_BANK * 3) {
        return BusMreqRdElement { .callback = onMemoryDeviceMreqRdRamBank5, .data = this };
    }

    return BusMreqRdElement { .callback = onMemoryDeviceMreqRdRamBankSel, .data = this };
}

BusMreqWrElement MemoryDevice::onConfigureMreqWr(
        BusMreqWrElement /* prev */,
        int /* mreqWrLayer */,
        uint16_t address) {

    if (address < SIZE_BANK) {
        return BusMreqWrElement { .callback = onMemoryDeviceMreqWrRom, .data = this };
    }

    if (address < SIZE_BANK * 2) {
        return BusMreqWrElement { .callback = onMemoryDeviceMreqWrRamBank2, .data = this };
    }

    if (address < SIZE_BANK * 3) {
        return BusMreqWrElement { .callback = onMemoryDeviceMreqWrRamBank5, .data = this };
    }

    return BusMreqWrElement { .callback = onMemoryDeviceMreqWrRamBankSel, .data = this };
}

BusIorqWrElement MemoryDevice::onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) {
    return ((port & 0x8003) == 0x0001)
            ? BusIorqWrElement { .callback = onMemoryDeviceIorqWr, .data = this }
            : prev;
}

void MemoryDevice::onReset() {
    port7FFD = 0;
    remap();
}

void MemoryDevice::remap() {
    if (bus->extPortDevice != nullptr && bus->extPortDevice->isRamMapRom()) {
        romBankPtr = &ram[0];
        onMreqWrRomPtr = &MemoryDevice::onMreqWrRomRam;
    } else {
        romBankPtr = &rom[(mode == Mode48 || port7FFD & BIT_ROM_BANK_1) ? SIZE_BANK : 0];
        onMreqWrRomPtr = &MemoryDevice::onMreqWrRomNone;
    }

    switch (mode) {
        case Mode128:
            ramBankPtr = &ram[(port7FFD & MASK_BANK_128) * SIZE_BANK];
            break;

        case Mode512:
            ramBankPtr = &ram[
                    (((port7FFD & MASK_BANK_512) >> SHIFT_BANK_512) | (port7FFD & MASK_BANK_128)) * SIZE_BANK];

            break;

        case Mode1024:
            ramBankPtr = &ram[
                    (((port7FFD & MASK_BANK_1024) >> SHIFT_BANK_1024) | (port7FFD & MASK_BANK_128)) * SIZE_BANK];

            break;

        default: // Mode48
            ramBankPtr = &ram[0];
    }
}

uint8_t MemoryDevice::onMreqRdRom(uint16_t address) {
    return romBankPtr[address];
}

uint8_t MemoryDevice::onMreqRdRamBank2(uint16_t address) {
    // address - 0x8000 + SIZE_BANK * 2 === address
    return ram[address];
}

uint8_t MemoryDevice::onMreqRdRamBank5(uint16_t address) {
    // address - 0x4000 + SIZE_BANK * 5 === address + SIZE_BANK * 4
    return ram[address + SIZE_BANK * 4];
}

uint8_t MemoryDevice::onMreqRdRamBankSel(uint16_t address) {
    return ramBankPtr[address - SIZE_BANK * 3];
}

void MemoryDevice::onMreqWrRomNone(uint16_t /* address */, uint8_t /* value */) {
}

void MemoryDevice::onMreqWrRomRam(uint16_t address, uint8_t value) {
    ram[address] = value;
}

void MemoryDevice::onMreqWrRamBank2(uint16_t address, uint8_t value) {
    // address - 0x8000 + SIZE_BANK * 2 === address
    ram[address] = value;
}

void MemoryDevice::onMreqWrRamBank5(uint16_t address, uint8_t value) {
    // address - 0x4000 + SIZE_BANK * 5 === address + SIZE_BANK * 4
    ram[address + SIZE_BANK * 4] = value;
}

void MemoryDevice::onMreqWrRamBankSel(uint16_t address, uint8_t value) {
    ram[address - SIZE_BANK * 3] = value;
}

void MemoryDevice::onIorqWr(uint16_t port, uint8_t value) {
    if (mode == Mode48) {
        return;
    }

    bool isExtPortLock = (bus->extPortDevice != nullptr && bus->extPortDevice->is128Lock());

    if ((port7FFD & BIT_LOCK) != 0 && (mode != Mode1024 || isExtPortLock)) {
        return;
    }

    if (mode != Mode128 && port == PORT_7FFD && !isExtPortLock) {
        port7FFD = value;
    } else {
        port7FFD = value & MASK_WRITE_128;
    }

    remap();
}

}
