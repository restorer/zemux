#ifndef ZEMUX_MACHINE__DEVICE
#define ZEMUX_MACHINE__DEVICE

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

#include <zemux_core/force_inline.h>
#include "bus.h"
#include "event.h"

namespace zemux {

class Device : public EventListener {
public:

    enum DeviceKind {
        KindMemory = 1,
        KindBorder = 2,
        KindZxKeyboard = 3,
        KindKempstonJoystick = 4,
        KindKempstonMouse = 5,
        KindCovox = 6,
        KindZxm = 7,
        KindTrDos = 8,
        KindExtPort = 9,
    };

    virtual ~Device() = default;

    virtual void onAttach() {
        isAttached_ = true;
    }

    virtual void onDetach() {
        isAttached_ = false;
    }

    virtual BusMreqRdElement onConfigureMreqRd(
            BusMreqRdElement prev,
            [[maybe_unused]] int mreqRdLayer,
            [[maybe_unused]] uint16_t address,
            [[maybe_unused]] bool isM1) {

        return prev;
    }

    virtual BusMreqWrElement onConfigureMreqWr(
            BusMreqWrElement prev,
            [[maybe_unused]] int mreqWrLayer,
            [[maybe_unused]] uint16_t address) {

        return prev;
    }

    virtual BusIorqRdElement onConfigureIorqRd(
            BusIorqRdElement prev,
            [[maybe_unused]] int iorqRdLayer,
            [[maybe_unused]] uint16_t port) {

        return prev;
    }

    virtual BusIorqWrElement onConfigureIorqWr(
            BusIorqWrElement prev,
            [[maybe_unused]] int iorqWrLayer,
            [[maybe_unused]] uint16_t port) {

        return prev;
    }

    virtual void onConfigureTimings(uint32_t ticksPerFrame);
    virtual void onFrameFinished(uint32_t ticks);

    virtual void onReset() {
    }

    ZEMUX_FORCE_INLINE bool isAttached() {
        return isAttached_;
    }

protected:

    Bus* bus;

    explicit Device(Bus* bus);

private:

    bool isAttached_ = false;
};

}

#endif
