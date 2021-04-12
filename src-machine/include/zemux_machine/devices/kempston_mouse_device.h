#ifndef ZEMUX_MACHINE__KEMPSTON_MOUSE_DEVICE
#define ZEMUX_MACHINE__KEMPSTON_MOUSE_DEVICE

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

class KempstonMouseDevice final : public Device, private NonCopyable {
public:

    enum EventType {
        EventWheelScrolled = Event::CategoryKempstonMouse | 1,
        EventSetConfiguration = Event::CategoryKempstonMouse | 2,
        EventGetConfiguration = Event::CategoryKempstonMouse | 3,
    };

    enum WheelDirection {
        DirectionNormal = 0,
        DirectionReverse = 1,
    };

    enum MiddleButtonMode {
        ModeNone = 0,
        ModeMiddle = 1,
        ModeMiddleAsLeftAndRight = 2,
        ModeLeftAndRightAsMiddle = 3,
    };

    struct Configuration {
        enum UpdateField {
            UpdateIsWheelEnabled = 1,
            UpdateWheelDirection = 2,
            UpdateIsLeftSwappedWithRight = 4,
            UpdateMiddleButtonMode = 8,
            UpdateMotionRatioX = 16,
            UpdateMotionRatioY = 32,
        };

        uint8_t updateMask;
        bool isWheelEnabled;
        WheelDirection wheelDirection;
        bool isLeftSwappedWithRight;
        MiddleButtonMode middleButtonMode;
        uint8_t motionRatioX; // 255 - full speed, 0 - almost stopped
        uint8_t motionRatioY; // 255 - full speed, 0 - almost stopped
    };

    static constexpr uint16_t PORT_FBDF = 0xFBDF;
    static constexpr uint16_t PORT_FFDF = 0xFFDF;
    static constexpr uint16_t PORT_FADF = 0xFADF;

    static constexpr uint8_t WHEEL_COUNTER_MASK_ENABLED = 0x0F;
    static constexpr uint8_t WHEEL_COUNTER_MASK_DISABLED = 0xFF;

    static constexpr uint8_t BUTTON_MASK_LEFT = ~1;
    static constexpr uint8_t BUTTON_MASK_RIGHT = ~2;
    static constexpr uint8_t BUTTON_MASK_MIDDLE = ~4;
    static constexpr uint8_t BUTTON_MASK_NONE = 0xFF;

    explicit KempstonMouseDevice(Bus* bus);
    virtual ~KempstonMouseDevice() = default;

    uint32_t getEventCategory() override;
    EventOutput onEvent(uint32_t type, EventInput input) override;

    BusIorqRdElement onConfigureIorqRd(BusIorqRdElement prev, int /* iorqRdLayer */, uint16_t port) override;

private:

    int32_t wheelDirectionMultiplier = 1;
    uint8_t wheelCounterMask = WHEEL_COUNTER_MASK_ENABLED;
    uint8_t buttonMaskLeft = BUTTON_MASK_LEFT;
    uint8_t buttonMaskRight = BUTTON_MASK_RIGHT;
    uint8_t buttonMaskMiddle = BUTTON_MASK_MIDDLE;
    int64_t motionMultiplierX = 256;
    int64_t motionMultiplierY = 256;
    int32_t lastMotionX = 0;
    int32_t lastMotionY = 0;
    int hostButtonMaskSpecial = 0;

    uint8_t portFBDF = 128;
    uint8_t portFFDF = 96;
    uint8_t portFADF = 255;
    uint8_t wheelCounter = 255;

    void update();

    static uint8_t onIorqRdFBDF(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    static uint8_t onIorqRdFFDF(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    static uint8_t onIorqRdFADF(void* data, int /* iorqRdLayer */, uint16_t /* port */);
};

}

#endif
