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

#include "sound/sound_desk.h"
#include <zemux_core/core.h>
#include <zemux_core/math_ext.h>
#include <zemux_core/vector_ext.h>
#include <algorithm>
#include <cstring>

namespace zemux {

SoundDeskJack::SoundDeskJack(SoundDesk* desk, SoundCable* cable) :  desk { desk }, cable { cable } {
    cable->onCableAttach(this, desk->ticksPerSecond_, desk->samplesPerSecond_);
}

SoundDeskJack::~SoundDeskJack() {
    cable->onCableDetach();
}

void SoundDeskJack::jackWrite(uint16_t left, uint16_t right) {
    if (lastLeft != left || lastRight != right) {
        volume = std::min(VOLUME_OVERMAX, volume + VOLUME_STEP);
    } else if (volume > VOLUME_STEP) {
        volume -= VOLUME_STEP;
    } else {
        volume = 0;
    }

    auto currentVolume = std::min(VOLUME_MAX, volume);
    auto& sample = desk->samples[position];

    sample.left += static_cast<uint32_t>(left) * currentVolume;
    sample.right += static_cast<uint32_t>(right) * currentVolume;
    desk->volumes[position] += currentVolume;

    position = (position + 1) & desk->positionMask;
    lastLeft = left;
    lastRight = right;
}

void SoundDesk::attachCable(SoundCable* cable) {
    attachedJacks.push_back(std::make_unique<SoundDeskJack>(this, cable));
}

void SoundDesk::detachCable(SoundCable* cable) {
    vectorEraseIf(attachedJacks, [&](const std::unique_ptr<SoundDeskJack>& jack) -> bool {
        return jack->cable == cable;
    });
}

void SoundDesk::onFrameStarted() {
    if (frameMinPosition && frameMaxPosition > frameMinPosition) {
        std::copy(&samples[frameMinPosition], &samples[frameMaxPosition], samples.get());
        std::copy(&volumes[frameMinPosition], &volumes[frameMaxPosition], volumes.get());
    }

    auto position = frameMaxPosition - frameMinPosition;
    memset(&samples[position], 0, (bufferSize - position) * sizeof(Sample));
    memset(&volumes[position], 0, (bufferSize - position) * sizeof(uint32_t));

    for (auto& jack : attachedJacks) {
        jack->position -= frameMinPosition;
    }
}

void SoundDesk::onFrameFinished(uint32_t ticks) {
    if (attachedJacks.empty()) {
        frameMinPosition = 0;
        frameMaxPosition = 0;
        return;
    }

    for (auto& jack : attachedJacks) {
        jack->cable->onCableFrameFinished(ticks);
    }

    frameMinPosition = attachedJacks[0]->position;
    frameMaxPosition = frameMinPosition;

    for (int i = 1, len = attachedJacks.size(); i < len; ++i) {
        auto position = attachedJacks[i]->position;

        if (position < frameMinPosition) {
            frameMinPosition = position;
        }

        if (position > frameMaxPosition) {
            frameMaxPosition = position;
        }
    }

    Sample* samplePtr = samples.get();
    uint32_t* ratioPtr = volumes.get();

    for (uint32_t i = frameMinPosition; i--; ++samplePtr, ++ratioPtr) {
        uint32_t ratio = std::min(SoundDeskJack::VOLUME_MAX, *ratioPtr);

        samplePtr->left /= ratio;
        samplePtr->right /= ratio;
    }
}

void SoundDesk::onReconfigure(uint32_t ticksPerSecond, uint32_t samplesPerSecond) {
    ticksPerSecond_ = ticksPerSecond;
    samplesPerSecond_ = samplesPerSecond;

    bufferSize = getNearestPot(samplesPerSecond * 2 / Core::FRAMES_PER_SECOND);
    positionMask = bufferSize - 1;

    samples.reset(new Sample[bufferSize]);
    volumes.reset(new uint32_t[bufferSize]);

    for (auto& jack : attachedJacks) {
        jack->cable->onCableReconfigure(ticksPerSecond, samplesPerSecond);
        jack->position = 0;
    }

    memset(samples.get(), 0, bufferSize * sizeof(Sample));
    memset(volumes.get(), 0, bufferSize * sizeof(uint32_t));
}

}
