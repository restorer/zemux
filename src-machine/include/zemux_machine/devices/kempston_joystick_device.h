#ifndef ZEMUX_MACHINE__KEMPSTON_DEVICE
#define ZEMUX_MACHINE__KEMPSTON_DEVICE

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
#include "event.h"
#include <zemux_core/non_copyable.h>
#include <cstdint>

namespace zemux {

uint8_t onKempstonJoystickDeviceIorqRd(void* data, uint16_t /* port */);

class KempstonJoystickDevice final : public Device, private NonCopyable {
public:

    enum EventType {
        EventButtonDown = Event::CategoryKempstonJoystick | 1,
        EventButtonUp = Event::CategoryKempstonJoystick | 2,
        EventSetInvertedBits = Event::CategoryKempstonJoystick | 3,
        EventGetInvertedBits = Event::CategoryKempstonJoystick | 4,
    };

    enum Button {
        Right = 1,
        Left = 2,
        Down = 4,
        Up = 8,
        Fire = 16,
    };

    static constexpr uint8_t STATE_MODIFIER_NORMAL = 0x00;
    static constexpr uint8_t STATE_MODIFIER_INVERTED = 0xFF;

    explicit KempstonJoystickDevice(Bus* bus);
    virtual ~KempstonJoystickDevice() = default;

    uint32_t getEventCategory() override;
    EventOutput onEvent(uint32_t type, EventInput input) override;

    BusIorqRdElement onConfigureIorqRd(BusIorqRdElement prev, int /* iorqRdLayer */, uint16_t port) override;

private:

    uint8_t state = 0;
    uint8_t stateModifier = STATE_MODIFIER_NORMAL;

    uint8_t onIorqRd();

    friend uint8_t onKempstonJoystickDeviceIorqRd(void* data, uint16_t /* port */);
};

}

#endif
