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

#include "devices/kempston_joystick_device.h"

namespace zemux {

uint8_t onKempstonJoystickDeviceIorqRd(void* data, int /* iorqRdLayer */, uint16_t /* port */) {
    return static_cast<KempstonJoystickDevice*>(data)->onIorqRd();
}

KempstonJoystickDevice::KempstonJoystickDevice(Bus* bus) : Device { bus } {
}

uint32_t KempstonJoystickDevice::getEventCategory() {
    return Event::CategoryKempstonJoystick;
}

EventOutput KempstonJoystickDevice::onEvent(uint32_t type, EventInput input) {
    switch (type) {
        case EventButtonDown:
            state |= static_cast<Button>(input.value);
            return EventOutput { .isHandled = true };

        case EventButtonUp:
            state &= ~static_cast<Button>(input.value);
            return EventOutput { .isHandled = true };

        case EventSetInvertedBits:
            stateModifier = input.value ? STATE_MODIFIER_INVERTED : STATE_MODIFIER_NORMAL;
            return EventOutput { .isHandled = true };

        case EventGetInvertedBits:
            return EventOutput { .isHandled = true, .value = (stateModifier == STATE_MODIFIER_INVERTED) };

        default:
            return EventOutput {};
    }
}

BusIorqRdElement KempstonJoystickDevice::onConfigureIorqRd(BusIorqRdElement prev, int, uint16_t port) {
    return (port & 0x20) ? prev : BusIorqRdElement { .callback = onKempstonJoystickDeviceIorqRd, .data = this };
}

uint8_t KempstonJoystickDevice::onIorqRd() {
    return state ^ stateModifier;
}

}
