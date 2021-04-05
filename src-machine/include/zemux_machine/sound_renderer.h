#ifndef ZEMUX_MACHINE__SOUND_RENDERER
#define ZEMUX_MACHINE__SOUND_RENDERER

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

#include <zemux_core/sound_sink.h>
#include <zemux_core/sound_source.h>
#include <zemux_core/non_copyable.h>

namespace zemux {

class SoundRenderer final : public SoundSink, public SoundSource, private NonCopyable {
public:

    SoundRenderer() = default;
    virtual ~SoundRenderer() = default;

    void soundForwardTo(uint16_t left, uint16_t right, uint32_t ticks) override;
    void soundAdvanceBy(uint16_t left, uint16_t right, uint32_t ticksDelta) override;

    void onSourceAttach(SoundMixer* mixer) override;
    void onSourceDetach() override;
    void completeSourceSamplesTo(uint32_t ticks) override;
    uint32_t getSourceSamplesMixed() override;
    void consumeSourceSamples(uint32_t samples) override;
    void reconfigureSource(uint32_t ticksPerFrame, uint32_t samplesPerSecond) override;

private:

    SoundMixer* attachedMixer = nullptr;
};

}

#endif
