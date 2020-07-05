#ifndef ZEMUX_PERIPHERALS__TAPE_TAP
#define ZEMUX_PERIPHERALS__TAPE_TAP

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

#include <cstdint>
#include <vector>
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/data_io.h>
#include "tape.h"

namespace zemux {

class TapeTap final : public Tape, private NonCopyable {
public:

    static const char* ERROR_MALFORMED;
    static const char* ERROR_CHUNK_TOO_SMALL;
    static const char* ERROR_INVALID_CHECKSUM;
    static const char* ERROR_NO_CHUNKS;

    TapeTap(DataReader* reader, Loudspeaker* loudspeaker, bool shouldValidate);
    ~TapeTap() = default;

    void step(unsigned int micros) override;
    void rewindToNearest(unsigned int micros) override;

private:

    static constexpr unsigned int PILOT_HEADER_COUNT = 4032;
    static constexpr unsigned int PILOT_DATA_COUNT = 1611;
    static constexpr unsigned int PILOT_MAIN_HALF_MICROS = tapeTicksToMicros(2168);
    static constexpr unsigned int PILOT_CAP_LOW_MICROS = tapeTicksToMicros(667);
    static constexpr unsigned int PILOT_CAP_HIGH_MICROS = tapeTicksToMicros(735);
    static constexpr unsigned int BIT_ZERO_HALF_MICROS = tapeTicksToMicros(855);
    static constexpr unsigned int BIT_ONE_HALF_MICROS = tapeTicksToMicros(1710);
    static constexpr unsigned int DELAY_MICROS = tapeTicksToMicros(2168 * 1000);

    struct Chunk {
        unsigned int offset;
        unsigned int size;
        unsigned int endMicros;
    };

    enum State {
        StatePilotMainLow,
        StatePilotMainHigh,
        StatePilotCapLow,
        StatePilotCapHigh,
        StateBitLow,
        StateBitHigh,
        StateDelay
    };

    std::vector<uint8_t> data;
    std::vector<Chunk> chunks;
    unsigned int totalChunks;

    unsigned int currentProcessedMicros = 0;
    unsigned int currentChunkIndex = 0;
    unsigned int currentOffset;
    unsigned int currentSizeLeft;
    State currentState;
    unsigned int currentWaitMicros;
    unsigned int currentPilotCount;
    uint8_t currentMask;
    uint8_t currentValue;

    ZEMUX_FORCE_INLINE unsigned int getPilotCount(uint8_t flagValue) {
        return (flagValue ? PILOT_DATA_COUNT : PILOT_HEADER_COUNT);
    }

    ZEMUX_FORCE_INLINE unsigned int getHalfMicrosForBit(uint8_t maskedValue) {
        return (maskedValue ? BIT_ONE_HALF_MICROS : BIT_ZERO_HALF_MICROS);
    }

    ZEMUX_FORCE_INLINE void initCurrentBitState() {
        currentState = StateBitLow;
        currentWaitMicros = getHalfMicrosForBit(currentValue & currentMask);
    }

    void parseChunks(bool shouldValidate);
    void initPilotState();
    void initValueState();
    unsigned int computeMicrosForValue(uint8_t value);
};

}

#endif
