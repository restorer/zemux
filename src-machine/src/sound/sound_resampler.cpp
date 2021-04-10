// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

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

#include "sound/sound_resampler.h"

namespace zemux {

SoundResampler::SoundResampler(ChronometerNarrow* inChronometer) : inChronometer { inChronometer } {
}

void SoundResampler::onCableAttach(SoundJack* jack, uint32_t ticksPerSecond, uint32_t samplesPerSecond) {
    attachedJack = jack;
    onCableReconfigure(ticksPerSecond, samplesPerSecond);
}

void SoundResampler::onCableDetach() {
    attachedJack = nullptr;
}

void SoundResampler::sinkForwardTo(uint16_t left, uint16_t right, uint32_t ticks) {
    advanceByInternal(left, right, chronometer.srcForwardToDelta(ticks));
}

void SoundResampler::sinkAdvanceBy(uint16_t left, uint16_t right, uint32_t ticksDelta) {
    advanceByInternal(left, right, chronometer.srcAdvanceByDelta(ticksDelta));
}

void SoundResampler::onCableFrameFinished(uint32_t ticks) {
    for (auto samples = chronometer.srcForwardToDelta(ticks); samples--;) {
        attachedJack->jackWrite(lastLeft, lastRight);
    }

    chronometer.srcConsume(ticks);
}

void SoundResampler::onCableReconfigure(uint32_t ticksPerSecond, uint32_t samplesPerSecond) {
    chronometer.reconfigure(
            (inChronometer == nullptr) ? ticksPerSecond : inChronometer->srcToDstCeil(ticksPerSecond),
            samplesPerSecond);
}

#if SOUND_RESAMPLER_LEVEL > 0

void SoundResampler::advanceByInternal(uint16_t left, uint16_t right, uint32_t samples) {
    if (attachedJack != nullptr && samples != 0) {
        int32_t currLeft = static_cast<int32_t>(left) << FP_SHIFT;
        int32_t currRight = static_cast<int32_t>(right) << FP_SHIFT;

        int32_t deltaLeft = (left - (static_cast<int32_t>(lastLeft) << FP_SHIFT)) / samples;
        int32_t deltaRight = (right - (static_cast<int32_t>(lastRight) << FP_SHIFT)) / samples;

        while (samples--) {
            attachedJack->jackWrite(currLeft >> FP_SHIFT, currRight >> FP_SHIFT);

            currLeft += deltaLeft;
            currRight += deltaRight;
        }
    }

    lastLeft = left;
    lastRight = right;
}

#endif

}
