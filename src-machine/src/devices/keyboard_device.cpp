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

#include "devices/keyboard_device.h"
#include <zemux_core/unroll.h>
#include <zemux_integrated/tape.h>

namespace zemux {

uint8_t onKeyboardDeviceIorqRd(void* data, int /* iorqRdLayer */, uint16_t port) {
    return static_cast<KeyboardDevice*>(data)->onIorqRd(port);
}

KeyboardDevice::KeyboardDevice(Bus* bus) : Device { bus } {
    resetKeys();
}

uint32_t KeyboardDevice::getEventCategory() {
    return Event::CategoryKeyboard;
}

EventOutput KeyboardDevice::onEvent(uint32_t type, EventInput input) {
    switch (type) {
        case EventKeyDown:
            keyboard[input.value >> 8] &= static_cast<uint8_t>(input.value);
            return EventOutput { .isHandled = true };

        case EventKeyUp:
            keyboard[input.value >> 8] |= static_cast<uint8_t>(input.value);
            return EventOutput { .isHandled = true };

        case EventResetKeys:
            resetKeys();
            return EventOutput { .isHandled = true };

        case EventSetCheapMatrixMode:
            isCheapMatrixMode = input.value;
            return EventOutput { .isHandled = true };

        case EventGetCheapMatrixMode:
            return EventOutput { .isHandled = true, .value = isCheapMatrixMode };

        default:
            return EventOutput {};
    }
}

BusIorqRdElement KeyboardDevice::onConfigureIorqRd(BusIorqRdElement prev, int /* iorqRdLayer */, uint16_t port) {
    return (port & 1) ? prev : BusIorqRdElement { .callback = onKeyboardDeviceIorqRd, .data = this };
}

uint8_t KeyboardDevice::onIorqRd(uint16_t port) {
    uint8_t result = 0xFF;
    uint8_t hiPort = port >> 8;

    UnrollWithIndex<8>::loop([&](const int index) {
        if ((hiPort & (1 << index)) == 0) {
            result &= keyboard[index];
        }
    });

    if (isCheapMatrixMode && hiPort != 0) {
        for (;;) {
            bool shouldBreak = true;

            UnrollWithIndex<8>::loop([&](const int index) {
                const uint8_t hiPortBit = (1 << index);

                if ((hiPort & hiPortBit) && ((keyboard[index] | result) ^ 0xFF)) {
                    result &= keyboard[index];
                    hiPort &= hiPortBit;
                    shouldBreak = false;
                }
            });

            if (hiPort == 0 || shouldBreak) {
                break;
            }
        }
    }

    if (bus->tape != nullptr && !bus->tape->getVolumeBit()) {
        result &= MASK_TAPE;
    }

    return result;
}

void KeyboardDevice::resetKeys() {
    for (int i = 0; i < NUM_ADDRESS_LINES; ++i) {
        keyboard[i] = 0xFF;
    }
}

}
