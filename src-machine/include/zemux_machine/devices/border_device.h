#ifndef ZEMUX_MACHINE__BORDER_DEVICE
#define ZEMUX_MACHINE__BORDER_DEVICE

/*
 * MIT License (http://www.opensource.org/licenses/mit-license.php)
 *
 * Copyright (c) 2020, Viachaslau Tratsiak (aka restorer)
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
#include "sound/sound_resampler.h"
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/sound.h>
#include <cstdint>

namespace zemux {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

static void onBorderDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);

#pragma clang diagnostic pop

class BorderDevice final : public Device, private NonCopyable {
public:

    static constexpr uint8_t MASK_COLOR = 7;

    static constexpr uint8_t BIT_TAPE = 8;
    static constexpr uint8_t BIT_SPEAKER = 0x10;

    static constexpr uint16_t VOLUME_TAPE = 0x4000;
    static constexpr uint16_t VOLUME_SPEAKER = 0xBFFF;

    BorderDevice(Bus* bus, SoundDesk* soundDesk);
    virtual ~BorderDevice() = default;

    void onAttach() override;
    void onDetach() override;
    BusIorqWrElement onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) override;

    ZEMUX_FORCE_INLINE uint8_t getPortFB() {
        return portFB;
    }

private:

    SoundDesk* soundDesk;
    SoundResampler soundResampler;
    uint8_t portFB = 0;

    void onIorqWr(uint8_t value);

    friend void onBorderDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);
};

}

#endif
