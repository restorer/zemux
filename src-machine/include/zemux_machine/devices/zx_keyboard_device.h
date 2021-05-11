#ifndef ZEMUX_MACHINE__KEYBOARD_DEVICE
#define ZEMUX_MACHINE__KEYBOARD_DEVICE

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
#include <zemux_core/force_inline.h>
#include <cstdint>

namespace zemux {

ZEMUX_FORCE_INLINE static constexpr uint16_t keyboardDeviceMakeKey(uint8_t addressLine, uint8_t dataMask) {
    return (static_cast<uint16_t>(addressLine) << 8) | static_cast<uint16_t>(dataMask);
}

class ZxKeyboardDevice final : public Device, private NonCopyable {
public:

    enum EventType {
        EventKeyDown = Event::CategoryKeyboard | 1,
        EventKeyUp = Event::CategoryKeyboard | 2,
        EventResetKeys = Event::CategoryKeyboard | 3,
        EventSetCheapMatrixMode = Event::CategoryKeyboard | 4,
        EventGetCheapMatrixMode = Event::CategoryKeyboard | 5,
    };

    enum Key {
        KeySpace = keyboardDeviceMakeKey(7, 0b0000'0001),
        KeySymbolShift = keyboardDeviceMakeKey(7, 0b0000'0010),
        KeyM = keyboardDeviceMakeKey(7, 0b0000'0100),
        KeyN = keyboardDeviceMakeKey(7, 0b0000'1000),
        KeyB = keyboardDeviceMakeKey(7, 0b0001'0000),

        KeyEnter = keyboardDeviceMakeKey(6, 0b0000'0001),
        KeyL = keyboardDeviceMakeKey(6, 0b0000'0010),
        KeyK = keyboardDeviceMakeKey(6, 0b0000'0100),
        KeyJ = keyboardDeviceMakeKey(6, 0b0000'1000),
        KeyH = keyboardDeviceMakeKey(6, 0b0001'0000),

        KeyP = keyboardDeviceMakeKey(5, 0b0000'0001),
        KeyO = keyboardDeviceMakeKey(5, 0b0000'0010),
        KeyI = keyboardDeviceMakeKey(5, 0b0000'0100),
        KeyU = keyboardDeviceMakeKey(5, 0b0000'1000),
        KeyY = keyboardDeviceMakeKey(5, 0b0001'0000),

        Key0 = keyboardDeviceMakeKey(4, 0b0000'0001),
        Key9 = keyboardDeviceMakeKey(4, 0b0000'0010),
        Key8 = keyboardDeviceMakeKey(4, 0b0000'0100),
        Key7 = keyboardDeviceMakeKey(4, 0b0000'1000),
        Key6 = keyboardDeviceMakeKey(4, 0b0001'0000),

        Key1 = keyboardDeviceMakeKey(3, 0b0000'0001),
        Key2 = keyboardDeviceMakeKey(3, 0b0000'0010),
        Key3 = keyboardDeviceMakeKey(3, 0b0000'0100),
        Key4 = keyboardDeviceMakeKey(3, 0b0000'1000),
        Key5 = keyboardDeviceMakeKey(3, 0b0001'0000),

        KeyQ = keyboardDeviceMakeKey(2, 0b0000'0001),
        KeyW = keyboardDeviceMakeKey(2, 0b0000'0010),
        KeyE = keyboardDeviceMakeKey(2, 0b0000'0100),
        KeyR = keyboardDeviceMakeKey(2, 0b0000'1000),
        KeyT = keyboardDeviceMakeKey(2, 0b0001'0000),

        KeyA = keyboardDeviceMakeKey(1, 0b0000'0001),
        KeyS = keyboardDeviceMakeKey(1, 0b0000'0010),
        KeyD = keyboardDeviceMakeKey(1, 0b0000'0100),
        KeyF = keyboardDeviceMakeKey(1, 0b0000'1000),
        KeyG = keyboardDeviceMakeKey(1, 0b0001'0000),

        KeyCapsShift = keyboardDeviceMakeKey(0, 0b0000'0001),
        KeyZ = keyboardDeviceMakeKey(0, 0b0000'0010),
        KeyX = keyboardDeviceMakeKey(0, 0b0000'0100),
        KeyC = keyboardDeviceMakeKey(0, 0b0000'1000),
        KeyV = keyboardDeviceMakeKey(0, 0b0001'0000),
    };

    static constexpr uint8_t MASK_TAPE = ~0x40;
    static constexpr int NUM_ADDRESS_LINES = 8;

    explicit ZxKeyboardDevice(Bus* bus);
    virtual ~ZxKeyboardDevice() = default;

    uint32_t getEventCategory() override;
    EventOutput onEvent(uint32_t type, EventInput input) override;

    BusIorqRdElement onConfigureIorqRd(BusIorqRdElement prev, int /* iorqRdLayer */, uint16_t port) override;

private:

    uint8_t keyboard[NUM_ADDRESS_LINES];
    bool isCheapMatrixMode = false;

    void resetKeys();

    static uint8_t onIorqRd(void* data, int /* iorqRdLayer */, uint16_t port);
};

}

#endif
