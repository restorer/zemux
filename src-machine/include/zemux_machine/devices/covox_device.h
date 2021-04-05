#ifndef ZEMUX_MACHINE__COVOX_DEVICE
#define ZEMUX_MACHINE__COVOX_DEVICE

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
#include "sound_renderer.h"
#include <zemux_core/non_copyable.h>
#include <zemux_core/sound_mixer.h>

namespace zemux {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

static void onCovoxDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);

#pragma clang diagnostic pop

class CovoxDevice final : public Device, private NonCopyable {
public:

    CovoxDevice(Bus* bus, SoundMixer* soundMixer);
    virtual ~CovoxDevice() = default;

    void onAttach() override;
    void onDetach() override;
    BusIorqWrElement onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) override;
    void onReset() override;

private:

    SoundMixer* soundMixer;
    SoundRenderer soundRenderer;

    void onIorqWr(uint8_t value);

    friend void onCovoxDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);
};

}

#endif
