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
#include <zemux_core/chronometer.h>
#include <zemux_core/data_io.h>
#include "tape.h"

namespace zemux {

class TapeTap final : public Tape, private NonCopyable {
public:

    static const char* ERROR_MALFORMED;
    static const char* ERROR_CHUNK_TOO_SMALL;
    static const char* ERROR_INVALID_CHECKSUM;
    static const char* ERROR_NO_CHUNKS;

    TapeTap(DataReader* reader, SoundSink* soundSink, bool shouldValidateStrict);
    ~TapeTap() override = default;

    void step(uint32_t micros) override;
    void rewindToNearest(uint64_t micros) override;

private:

    static constexpr uint64_t PILOT_PULSE_TICKS = 2168;
    static constexpr unsigned int PILOT_HEADER_PULSES = 8063;
    static constexpr unsigned int PILOT_DATA_PULSES = 3223;
    static constexpr uint64_t SYNC_PULSE_FIRST_TICKS = 667;
    static constexpr uint64_t SYNC_PULSE_SECOND_TICKS = 735;
    static constexpr uint64_t BIT_ZERO_PULSE_TICKS = 855;
    static constexpr uint64_t BIT_ONE_PULSE_TICKS = 1710;
    static constexpr uint64_t DELAY_FIRST_TICKS = tapeMicrosToTicks(MILLIS_MICROS); // 3584
    static constexpr uint64_t DELAY_SECOND_TICKS = tapeMicrosToTicks(SECOND_MICROS - MILLIS_MICROS); // 3580416

    struct Chunk {
        unsigned int offset;
        unsigned int size;
        uint64_t endTicks;
    };

    enum State {
        StatePilot,
        StateSyncFirst,
        StateSyncSecond,
        StateBitFirst,
        StateBitSecond,
        StateDelayFirst,
        StateDelaySecond
    };

    ChronometerWide chronometer { FRAME_TICKS, FRAME_MICROS };

    std::vector<uint8_t> data;
    std::vector<Chunk> chunks;
    unsigned int totalChunks;

    unsigned int chunkIndex = 0;
    unsigned int chunkOffset;
    unsigned int chunkSizeLeft;

    State nextState;
    uint64_t nextWaitTicks;
    bool nextVolumeBit = false;

    unsigned int pilotPulsesLeft;
    uint8_t bitsMask;
    uint8_t bitsValue;

    ZEMUX_FORCE_INLINE unsigned int getPilotPulses(uint8_t flagValue) {
        return (flagValue ? PILOT_DATA_PULSES : PILOT_HEADER_PULSES);
    }

    ZEMUX_FORCE_INLINE uint64_t getPulseTicksForBit(uint8_t maskedValue) {
        return (maskedValue ? BIT_ONE_PULSE_TICKS : BIT_ZERO_PULSE_TICKS);
    }

    ZEMUX_FORCE_INLINE void initCurrentBitState() {
        nextState = StateBitFirst;
        nextWaitTicks = getPulseTicksForBit(bitsValue & bitsMask);
    }

    void parseChunks(bool shouldValidateStrict);
    void initPilotState();
    void initBitsState();
    uint64_t computeTicksForValue(uint8_t value);
};

}

#endif
