#ifndef ZEMUX_MACHINE__SOUND_RESAMPLER
#define ZEMUX_MACHINE__SOUND_RESAMPLER

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

#include <zemux_core/sound.h>
#include <zemux_core/chronometer.h>
#include <zemux_core/non_copyable.h>

#define SOUND_RESAMPLER 1

namespace zemux {

class SoundResampler final : public SoundSink, public SoundCable, private NonCopyable {
public:

    explicit SoundResampler(ChronometerNarrow* inChronometer = nullptr);
    virtual ~SoundResampler() = default;

    void sinkForwardTo(uint16_t left, uint16_t right, uint32_t ticks) override;
    void sinkAdvanceBy(uint16_t left, uint16_t right, uint32_t ticksDelta) override;

    void onCableAttach(SoundJack* jack, uint32_t ticksPerSecond, uint32_t samplesPerSecond) override;
    void onCableDetach() override;
    void onCableFrameFinished(uint32_t ticks) override;
    void onCableReconfigure(uint32_t ticksPerSecond, uint32_t samplesPerSecond) override;

protected:

    ChronometerNarrow* inChronometer;
    ChronometerNarrow chronometer { 1, 1 };
    SoundJack* attachedJack = nullptr;

    uint16_t lastLeft = 0;
    uint16_t lastRight = 0;

#if SOUND_RESAMPLER == 0

    ZEMUX_FORCE_INLINE void advanceByInternal(uint16_t left, uint16_t right, uint32_t samples) {
        lastLeft = left;
        lastRight = right;

        if (attachedJack != nullptr) {
            while (samples--) {
                attachedJack->jackWrite(left, right);
            }
        }
    }

#else

    static constexpr unsigned int FP_SHIFT = 15;

    void advanceByInternal(uint16_t left, uint16_t right, uint32_t samples);

#endif
};

}

#endif
