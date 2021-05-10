#ifndef ZEMUX_CORE__SOUND_SINK
#define ZEMUX_CORE__SOUND_SINK

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

#include <cstdint>

namespace zemux {

class SoundJack {
public:

    virtual void jackWrite(uint16_t left, uint16_t right) = 0;

protected:

    constexpr SoundJack() = default;
    virtual ~SoundJack() = default;
};

class SoundSink {
public:

    virtual void sinkForwardTo(uint16_t left, uint16_t right, uint32_t ticks) = 0;
    virtual void sinkAdvanceBy(uint16_t left, uint16_t right, uint32_t ticksDelta) = 0;

protected:

    constexpr SoundSink() = default;
    virtual ~SoundSink() = default;
};

class SoundCable {
public:

    virtual void onCableAttach(SoundJack* jack, uint32_t ticksPerSecond, uint32_t samplesPerSecond) = 0;
    virtual void onCableDetach() = 0;
    virtual void onCableFrameFinished(uint32_t ticks) = 0;
    virtual void onCableReconfigure(uint32_t ticksPerSecond, uint32_t samplesPerSecond) = 0;

protected:

    constexpr SoundCable() = default;
    virtual ~SoundCable() = default;
};

}

#endif
