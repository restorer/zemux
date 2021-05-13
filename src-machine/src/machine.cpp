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

Machine::Machine() : bus { this, &cpuChronometer }, cpu { &bus,
        &Bus::onCpuMreqRd,
        &Bus::onCpuMreqWr,
        &Bus::onCpuIorqRd,
        &Bus::onCpuIorqWr,
        &Bus::onCpuIorqM1,
        &Bus::onCpuPutAddress } {

    brazeDevice(Device::KindMemory, std::make_unique<MemoryDevice>(&bus));
    brazeDevice(Device::KindBorder, std::make_unique<BorderDevice>(&bus, &soundDesk));
    brazeDevice(Device::KindZxKeyboard, std::make_unique<ZxKeyboardDevice>(&bus));
    brazeDevice(Device::KindKempstonJoystick, std::make_unique<KempstonJoystickDevice>(&bus));
    brazeDevice(Device::KindKempstonMouse, std::make_unique<KempstonMouseDevice>(&bus));
    brazeDevice(Device::KindCovox, std::make_unique<CovoxDevice>(&bus, &soundDesk));
    brazeDevice(Device::KindZxm, std::make_unique<ZxmDevice>(&bus, &soundDesk));
    brazeDevice(Device::KindTrDos, std::make_unique<TrDosDevice>(&bus));
    brazeDevice(Device::KindExtPort, std::make_unique<ExtPortDevice>(&bus));

    deviceMap[Device::KindMemory]->onAttach();
    deviceMap[Device::KindBorder]->onAttach();
    deviceMap[Device::KindZxKeyboard]->onAttach();
    deviceMap[Device::KindKempstonJoystick]->onAttach();
    deviceMap[Device::KindKempstonMouse]->onAttach();
    deviceMap[Device::KindCovox]->onAttach();
    deviceMap[Device::KindZxm]->onAttach();
    deviceMap[Device::KindTrDos]->onAttach();
    deviceMap[Device::KindExtPort]->onAttach();

    onBusReconfigure();
    onBusReset();
}

void Machine::renderFrame() {
    // TODO: ...

    auto frameTicks = cpuChronometer.getSrcTicksPassed();

    for (auto& device : attachedDevices) {
        device->onFrameFinished(frameTicks);
    }

    cpuChronometer.srcConsume(frameTicks);
}

void Machine::brazeDevice(Device::DeviceKind kind, std::unique_ptr<Device> device) {
    deviceMap[kind] = std::move(device);

    if (device->getEventCategory()) {
        eventListenerMap[device->getEventCategory()] = device.get();
    }
}

void Machine::onBusReconfigure() {
    attachedDevices.clear();

    for (auto& entry : deviceMap) {
        auto* device = entry.second.get();

        if (device->isAttached()) {
            attachedDevices.push_back(device);
        }
    }

    for (auto& device : attachedDevices) {
        device->onConfigureTimings(ulaFrameTicks);
    }

    bus.onMachineReconfigure(attachedDevices);
}

void Machine::onBusReset() {
    bus.onMachineReset();
    cpu.reset();

    for (auto& device : attachedDevices) {
        device->onReset();
    }
}

}
