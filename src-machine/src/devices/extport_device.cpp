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

#include "devices/extport_device.h"
#include "devices/memory_device.h"

namespace zemux {

void onExtPortDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value) {
    static_cast<ExtPortDevice*>(data)->onIorqWr(value);
}

ExtPortDevice::ExtPortDevice(Bus* bus) : Device { bus } {
}

uint32_t ExtPortDevice::getEventCategory() {
    return Event::CategoryExtPort;
}

EventOutput ExtPortDevice::onEvent(uint32_t type, EventInput input) {
    switch (type) {
        case EventSetConfiguration: {
            auto config = static_cast<Configuration*>(input.pointer);

            isOldMode = config->isOldMode;
            isTurboEnabled = config->isTurboEnabled;

            return EventOutput { .isHandled = true };
        }

        case EventGetConfiguration: {
            auto config = static_cast<Configuration*>(input.pointer);

            config->isOldMode = isOldMode;
            config->isTurboEnabled = isTurboEnabled;

            return EventOutput { .isHandled = true };
        }

        default:
            return EventOutput {};
    }
}

void ExtPortDevice::onAttach() {
    Device::onAttach();
    bus->extPortDevice = this;

    if (bus->memoryDevice != nullptr) {
        bus->memoryDevice->remap();
    }
}

void ExtPortDevice::onDetach() {
    bus->extPortDevice = nullptr;

    if (bus->memoryDevice != nullptr) {
        bus->memoryDevice->remap();
    }

    Device::onDetach();
}

BusIorqWrElement ExtPortDevice::onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) {
    return (port == PORT_EFF7) ? BusIorqWrElement { .callback = onExtPortDeviceIorqWr, .data = this } : prev;
}

void ExtPortDevice::onReset() {
    if (isTurboEnabled) {
        // Disable turbo by default

        if ((portEFF7 & BIT_GIGASCREEN_OR_TURBO) == 0) {
            // Change clock ratio only if it may have changed using the ext port, in other case leave it as is
            bus->setCpuClockRatio(1);
        }

        portEFF7 = BIT_GIGASCREEN_OR_TURBO;
    } else {
        portEFF7 = 0;
    }
}

void ExtPortDevice::onIorqWr(uint8_t value) {
    if (isTurboEnabled && (value & BIT_GIGASCREEN_OR_TURBO) != (portEFF7 & BIT_GIGASCREEN_OR_TURBO)) {
        bus->setCpuClockRatio((value & BIT_GIGASCREEN_OR_TURBO) ? 1 : 2);
    }

    portEFF7 = value;
    // bus->reconfigureVideo();
}

}
