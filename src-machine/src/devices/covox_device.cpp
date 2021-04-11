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

#include "devices/covox_device.h"

namespace zemux {

void onCovoxDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value) {
    static_cast<CovoxDevice*>(data)->onIorqWr(value);
}

CovoxDevice::CovoxDevice(Bus* bus, SoundDesk* soundDesk) : Device { bus }, soundDesk { soundDesk } {
}

void CovoxDevice::onAttach() {
    Device::onAttach();
    soundDesk->attachCable(&soundResampler);
}

void CovoxDevice::onDetach() {
    soundDesk->detachCable(&soundResampler);
    Device::onDetach();
}

BusIorqWrElement CovoxDevice::onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) {
    return ((port & 0x07) == 0x03) ? BusIorqWrElement { .callback = onCovoxDeviceIorqWr, .data = this } : prev;
}

void CovoxDevice::onReset() {
    soundResampler.sinkAdvanceBy(0, 0, 0);
}

void CovoxDevice::onIorqWr(uint8_t value) {
    uint16_t volume = static_cast<uint16_t>(value) << 8;
    soundResampler.sinkForwardTo(volume, volume, bus->getFrameTicksPassed());
}

}
