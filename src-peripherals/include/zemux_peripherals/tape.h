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
#include <cstdint>
#include <zemux_core/error.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/loudspeaker.h>
#include <zemux_core/data_io.h>

namespace zemux {

class Tape {
public:

    static constexpr uint64_t MILLIS_MICROS = 1000;
    static constexpr uint64_t SECOND_MICROS = 1000 * MILLIS_MICROS;

    static constexpr uint64_t FRAME_TICKS = 71680;
    static constexpr uint64_t FRAME_MICROS = 20000;

    static constexpr uint16_t LOW_BIT_VOLUME = 0x0000;
    static constexpr uint16_t HIGH_BIT_VOLUME = 0xFFFF;

    [[nodiscard]] ZEMUX_FORCE_INLINE bool getVolumeBit() const {
        return volumeBit;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE uint64_t getTotalMicros() const {
        return totalMicros;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE uint64_t getElapsedMicros() const {
        return elapsedMicros;
    }

    virtual void step(uint32_t micros) = 0;
    virtual void rewindToNearest(uint64_t micros) = 0;

protected:

    DataReader* reader;
    Loudspeaker* loudspeaker;
    uint64_t totalMicros = 0;
    uint64_t elapsedMicros = 0;

    Tape(DataReader* reader, Loudspeaker* loudspeaker);
    virtual ~Tape() = default;

    void volumeStep(bool outputBit, uint32_t micros) {
        volumeBit = outputBit;

        if (micros) {
            uint16_t volume = outputBit ? HIGH_BIT_VOLUME : LOW_BIT_VOLUME;
            loudspeaker->onLoudspeakerStep(volume, volume, micros);
        }
    }

private:

    bool volumeBit = false;
};

class TapeError final : public AbstractRuntimeError<TapeError> {
public:

    explicit TapeError(const std::string& key) noexcept: AbstractRuntimeError { key } {
    }
};

ZEMUX_FORCE_INLINE constexpr uint64_t tapeTicksToMicros(uint64_t ticks) {
    return ticks * Tape::FRAME_MICROS / Tape::FRAME_TICKS;
}

ZEMUX_FORCE_INLINE constexpr uint64_t tapeMicrosToTicks(uint64_t micros) {
    return micros * Tape::FRAME_TICKS / Tape::FRAME_MICROS;
}

}

#endif
