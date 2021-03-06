#ifndef ZEMUX_MACHINE__SOUND_MIXER
#define ZEMUX_MACHINE__SOUND_MIXER

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
#include <memory>
#include <vector>
#include <zemux_core/sound.h>
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>

namespace zemux {

class SoundDesk;

class SoundDeskJack final : public SoundJack {
public:

    static constexpr uint32_t VOLUME_MAX = 0x10000;
    static constexpr uint32_t VOLUME_OVERMAX = VOLUME_MAX * 4;
    static constexpr uint32_t VOLUME_STEP = 4;

    SoundDesk* desk;
    SoundCable* cable;
    uint32_t position = 0;
    uint32_t volume = 0;
    uint16_t lastLeft = 0;
    uint16_t lastRight = 0;

    SoundDeskJack(SoundDesk* desk, SoundCable* cable);
    virtual ~SoundDeskJack();

    void jackWrite(uint16_t left, uint16_t right) override;
};

class SoundDesk final : private NonCopyable {
public:

    struct Sample {
        uint32_t left;
        uint32_t right;
    };

    SoundDesk() = default;
    virtual ~SoundDesk() = default;

    void attachCable(SoundCable* cable);
    void detachCable(SoundCable* cable);
    void onFrameStarted();
    void onFrameFinished(uint32_t ticks);
    void onReconfigure(uint32_t ticksPerSecond, uint32_t samplesPerSecond);

    ZEMUX_FORCE_INLINE Sample* getBuffer() {
        return samples.get();
    }

    ZEMUX_FORCE_INLINE uint32_t getBufferSize() {
        return frameMinPosition;
    }

private:

    uint32_t ticksPerSecond_;
    uint32_t samplesPerSecond_;
    uint32_t bufferSize = 0;
    uint32_t positionMask = 0;
    std::unique_ptr<Sample[]> samples;
    std::unique_ptr<uint32_t[]> volumes;
    std::vector<std::unique_ptr<SoundDeskJack>> attachedJacks;
    uint32_t frameMinPosition = 0;
    uint32_t frameMaxPosition = 0;

    friend class SoundDeskJack;
};

}

#endif
