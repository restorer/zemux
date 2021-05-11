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

#include "machine.h"
#include "devices/border_device.h"
#include "devices/covox_device.h"
#include "devices/extport_device.h"
#include "devices/kempston_joystick_device.h"
#include "devices/kempston_mouse_device.h"
#include "devices/zx_keyboard_device.h"
#include "devices/memory_device.h"
#include "devices/trdos_device.h"
#include "devices/zxm_device.h"

namespace zemux {

Machine::Machine() : cpu { this,
        &onCpuMreqRd,
        &onCpuMreqWr,
        &onCpuIorqRd,
        &onCpuIorqWr,
        &onCpuIorqM1,
        &onCpuPutAddress }, bus { &cpu, &cpuChronometer } {

    prepareDevice(Device::KindMemory, std::make_unique<MemoryDevice>(&bus));
    prepareDevice(Device::KindBorder, std::make_unique<BorderDevice>(&bus, &soundDesk));
    prepareDevice(Device::KindZxKeyboard, std::make_unique<ZxKeyboardDevice>(&bus));
    prepareDevice(Device::KindKempstonJoystick, std::make_unique<KempstonJoystickDevice>(&bus));
    prepareDevice(Device::KindKempstonMouse, std::make_unique<KempstonMouseDevice>(&bus));
    prepareDevice(Device::KindCovox, std::make_unique<CovoxDevice>(&bus, &soundDesk));
    prepareDevice(Device::KindZxm, std::make_unique<ZxmDevice>(&bus, &soundDesk));
    prepareDevice(Device::KindTrDos, std::make_unique<TrDosDevice>(&bus));
    prepareDevice(Device::KindExtPort, std::make_unique<ExtPortDevice>(&bus));
}

void Machine::renderFrame() {
}

void Machine::prepareDevice(Device::DeviceKind kind, std::unique_ptr<Device> device) {
    deviceMap[kind] = std::move(device);

    if (device->getEventCategory()) {
        eventListenerMap[device->getEventCategory()] = device.get();
    }
}

void Machine::reconfigure() {
    auto& attachedDevices = bus.attachedDevices;

    for (auto it = attachedDevices.begin(); it != attachedDevices.end(); ++it) {
        (*it)->onDetach();
    }

    attachedDevices.clear();

    for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it) {
        auto device = it->second.get();

        if (device->isAttached()) {
            attachedDevices.push_back(device);
            device->onConfigureTimings(ulaFrameTicks);
        }
    }

    bus.performReconfigure();
}

uint8_t Machine::onCpuMreqRd(void* data, uint16_t address, bool isM1) {
    auto self = static_cast<Machine*>(data);
    auto& bus = self->bus;

    auto elem = bus.mreqRdMap[address + (isM1 ? Bus::ELEMENTS_MREQ_RD_BASE : 0)];
    return elem.callback(elem.data, bus.getMreqRdLayer(), address, isM1);
}

void Machine::onCpuMreqWr(void* data, uint16_t address, uint8_t value) {
    auto self = static_cast<Machine*>(data);
    auto& bus = self->bus;

    auto elem = bus.mreqWrMap[address];
    elem.callback(elem.data, bus.getMreqWrLayer(), address, value);
}

uint8_t Machine::onCpuIorqRd(void* data, uint16_t port) {
    auto self = static_cast<Machine*>(data);
    auto& bus = self->bus;

    auto elem = bus.iorqRdMap[port];
    return elem.callback(elem.data, bus.getIorqRdLayer(), port);
}

void Machine::onCpuIorqWr(void* data, uint16_t port, uint8_t value) {
    auto self = static_cast<Machine*>(data);
    auto& bus = self->bus;

    auto elem = bus.iorqWrMap[port];
    elem.callback(elem.data, bus.getIorqWrLayer(), port, value);
}

uint8_t Machine::onCpuIorqM1(void* /* data */) {
    // TODO: unstable data bus
    return 0xFF;
}

void Machine::onCpuPutAddress(void* /* data */, uint16_t /* address */, uint_fast32_t /* cycles */) {
}

}
