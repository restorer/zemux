#ifndef ZEMUX_PERIPHERALS__TAPE
#define ZEMUX_PERIPHERALS__TAPE

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

#include <stdexcept>
#include <zemux_core/error.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/loudspeaker.h>
#include <zemux_core/data_io.h>

namespace zemux {

ZEMUX_FORCE_INLINE constexpr unsigned int tapeTicksToMicros(unsigned int ticks) {
    return ticks * 20000 / 71680;
}

class Tape {
public:

    static constexpr unsigned int SECOND_MICROS = 1000000;
    static constexpr uint16_t LOW_BIT_VOLUME = 0x0000;
    static constexpr uint16_t HIGH_BIT_VOLUME = 0xFFFF;

    inline bool getVolumeBit() const {
        return volumeBit;
    }

    inline unsigned int getTotalMicros() {
        return totalMicros;
    }

    inline unsigned int getElapsedMicros() {
        return elapsedMicros;
    }

    virtual void step(unsigned int micros) = 0;
    virtual void rewindToNearest(unsigned int micros) = 0;

protected:

    DataReader* reader;
    Loudspeaker* loudspeaker;
    bool volumeBit = false;
    unsigned int totalMicros = 0;
    unsigned int elapsedMicros = 0;

    Tape(DataReader* reader, Loudspeaker* loudspeaker);
    ~Tape() = default;

    inline void loudspeakerStep(unsigned int micros) {
        loudspeakerStep(volumeBit, micros);
    }

    inline void loudspeakerStep(bool outputBit, unsigned int micros) {
        if (micros) {
            uint16_t volume = outputBit ? HIGH_BIT_VOLUME : LOW_BIT_VOLUME;
            loudspeaker->onLoudspeakerStep(volume, volume, micros);
        }
    }
};

class TapeError final : public AbstractRuntimeError<TapeError> {
public:

    explicit TapeError(const std::string& key) noexcept: AbstractRuntimeError { key } {
    }
};

}

#endif
