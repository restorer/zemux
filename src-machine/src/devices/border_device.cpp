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

#include "devices/border_device.h"

namespace zemux {

void onBorderDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value) {
    static_cast<BorderDevice*>(data)->onIorqWr(value);
}

BorderDevice::BorderDevice(Bus* bus, SoundDesk* soundDesk) : Device { bus }, soundDesk { soundDesk } {
}

void BorderDevice::onAttach() {
    Device::onAttach();
    soundDesk->attachCable(&soundResampler);
}

void BorderDevice::onDetach() {
    soundDesk->detachCable(&soundResampler);
    Device::onDetach();
}

BusIorqWrElement BorderDevice::onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) {
    return (port & 1) ? prev : BusIorqWrElement { .callback = onBorderDeviceIorqWr, .data = this };
}

void BorderDevice::onIorqWr(uint8_t value) {
    uint32_t ticks = bus->getFrameTicksPassed();
    uint16_t volume = 0;

    if (value & BIT_TAPE) {
        volume += VOLUME_TAPE;
    }

    if (value & BIT_SPEAKER) {
        volume += VOLUME_SPEAKER;
    }

    soundResampler.sinkForwardTo(volume, volume, ticks);

    if ((value & MASK_COLOR) != (portFB & MASK_COLOR)) {
        // videoDevice->renderStepTo(ticks);
    }

    portFB = value;
}

}
