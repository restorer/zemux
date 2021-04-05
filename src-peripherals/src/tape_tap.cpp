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

#include <cstdint>
#include "tape_tap.h"

static uintmax_t MAX_TAP_SIZE = 1024 * 1024 * 16;

// #define __VERBOSE

namespace zemux {

const char* TapeTap::ERROR_MALFORMED = /* @i18n */ "tape_tap.malformed";
const char* TapeTap::ERROR_CHUNK_TOO_SMALL = /* @i18n */ "tape_tap.chunk_too_small";
const char* TapeTap::ERROR_INVALID_CHECKSUM = /* @i18n */ "tape_tap.invalid_checksum";
const char* TapeTap::ERROR_NO_CHUNKS = /* @i18n */ "tape_tap.no_chunks";

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

TapeTap::TapeTap(
        DataReader* reader,
        SoundSink* soundSink,
        bool shouldValidateStrict) : Tape { reader, soundSink } {

    data = reader->readEntire(MAX_TAP_SIZE);
    parseChunks(shouldValidateStrict);
}

#pragma clang diagnostic pop

void TapeTap::step(uint32_t micros) {
#ifdef __VERBOSE
    if (chunkIndex < totalChunks && chronometer.getDstTicksPassed() < elapsedMicros + micros) {
        printf("TAP >>> elapsedMicros = %ld, volumeBit = %d, srcTicksPassed = %ld, dstTicksPassed = %ld, chunkIndex = %d, chunkOffset = %d, chunkSizeLeft = %d, nextState = %d, nextWaitTicks = %ld, nextVolumeBit = %d, pilotPulsesLeft = %d, bitsMask = %d, bitsValue = %d, micros = %d\n",
                elapsedMicros,
                getVolumeBit(),
                chronometer.getSrcTicksPassed(),
                chronometer.getDstTicksPassed(),
                chunkIndex,
                chunkOffset,
                chunkSizeLeft,
                nextState,
                nextWaitTicks,
                nextVolumeBit,
                pilotPulsesLeft,
                bitsMask,
                bitsValue,
                micros);
    }
#endif

    elapsedMicros += micros;

    if (elapsedMicros > totalMicros) {
        elapsedMicros = totalMicros;
    }

    if (chunkIndex >= totalChunks) {
        volumeStep(false, micros);
        return;
    }

    while (chronometer.getDstTicksPassed() < elapsedMicros) {
        volumeStep(nextVolumeBit, chronometer.srcAdvanceByDelta(nextWaitTicks));

        switch (nextState) {
            case StatePilot:
                nextVolumeBit = !nextVolumeBit;

                if (!(--pilotPulsesLeft)) {
                    nextState = StateSyncFirst;
                    nextWaitTicks = SYNC_PULSE_FIRST_TICKS;
                }
                break;

            case StateSyncFirst:
                nextVolumeBit = !nextVolumeBit;
                nextState = StateSyncSecond;
                nextWaitTicks = SYNC_PULSE_SECOND_TICKS;
                break;

            case StateSyncSecond:
                nextVolumeBit = !nextVolumeBit;
                initBitsState();
                break;

            case StateBitFirst:
                nextVolumeBit = !nextVolumeBit;
                nextState = StateBitSecond;
                break;

            case StateBitSecond:
                nextVolumeBit = !nextVolumeBit;
                bitsMask >>= 1;

                if (bitsMask) {
                    initCurrentBitState();
                } else if (--chunkSizeLeft) {
                    ++chunkOffset;
                    initBitsState();
                } else {
                    nextState = StateDelayFirst;
                    nextWaitTicks = DELAY_FIRST_TICKS;
                }
                break;

            case StateDelayFirst:
                nextVolumeBit = false;
                nextState = StateDelaySecond;
                nextWaitTicks = DELAY_SECOND_TICKS;
                break;

            case StateDelaySecond:
                if (++chunkIndex < totalChunks) {
                    initPilotState();
                }
                break;
        }
    }
}

void TapeTap::rewindToNearest(uint64_t micros) {
    uint64_t desiredTicks = chronometer.dstToSrcCeil(micros);
    uint64_t processedTicks = 0;
    chunkIndex = 0;

    while (chunkIndex < totalChunks && chunks[chunkIndex].endTicks <= desiredTicks) {
        processedTicks = chunks[chunkIndex].endTicks;
        chunkIndex++;
    }

    if (chunkIndex >= totalChunks) {
        chronometer.setSrcTicksPassed(processedTicks);
        elapsedMicros = totalMicros;
        volumeStep(false, 0);
        return;
    }

    initPilotState();
    uint64_t endTicks = processedTicks + pilotPulsesLeft * PILOT_PULSE_TICKS;

    if (desiredTicks < endTicks) {
        unsigned int processedPulses = (desiredTicks - processedTicks) / PILOT_PULSE_TICKS;

        nextVolumeBit = (processedPulses % 2) != 0;
        processedTicks += processedPulses * PILOT_PULSE_TICKS;
        pilotPulsesLeft -= processedPulses;

        chronometer.setSrcTicksPassed(processedTicks);
        elapsedMicros = chronometer.getDstTicksPassed();
        volumeStep(processedPulses != 0 && !nextVolumeBit, 0);
        return;
    }

    processedTicks = endTicks;
    endTicks += SYNC_PULSE_FIRST_TICKS + SYNC_PULSE_SECOND_TICKS;
    pilotPulsesLeft = 0;
    nextVolumeBit = (pilotPulsesLeft % 2) == 0;

    if (desiredTicks < endTicks) {
        nextState = StateSyncFirst;
        nextWaitTicks = SYNC_PULSE_FIRST_TICKS;

        chronometer.setSrcTicksPassed(processedTicks);
        elapsedMicros = chronometer.getDstTicksPassed();
        volumeStep(!nextVolumeBit, 0); // last pulse of pilot
        return;
    }

    processedTicks = endTicks;
    // nextVolumeBit remains the same after sync

    while (chunkSizeLeft) {
        endTicks += computeTicksForValue(data[chunkOffset]);

        if (desiredTicks < endTicks) {
            initBitsState();

            chronometer.setSrcTicksPassed(processedTicks);
            elapsedMicros = chronometer.getDstTicksPassed();
            volumeStep(!nextVolumeBit, 0); // last pulse of sync or of previous byte
            return;
        }

        processedTicks = endTicks;
        ++chunkOffset;
        --chunkSizeLeft;
    }

    --chunkOffset;
    endTicks += DELAY_FIRST_TICKS;
    // nextVolumeBit remains the same after bytes

    if (desiredTicks < endTicks) {
        nextState = StateDelayFirst;
        nextWaitTicks = DELAY_FIRST_TICKS;

        chronometer.setSrcTicksPassed(processedTicks);
        elapsedMicros = chronometer.getDstTicksPassed();
        volumeStep(!nextVolumeBit, 0); // last pulse of last byte
        return;
    }

    nextVolumeBit = false;
    nextState = StateDelaySecond;
    nextWaitTicks = DELAY_SECOND_TICKS - (desiredTicks - endTicks);

    chronometer.setSrcTicksPassed(desiredTicks);
    elapsedMicros = chronometer.getDstTicksPassed();
    volumeStep((pilotPulsesLeft % 2) == 0, 0); // pulse of "delay first" is the same as pulse of "sync first"
}

void TapeTap::parseChunks(bool shouldValidateStrict) {
    unsigned int totalSize = data.size();
    unsigned int position = 0;
    uint64_t totalTicks = 0;

    while (position < totalSize) {
        if (position + 2 > totalSize) {
            throw TapeError(ERROR_MALFORMED);
        }

        unsigned int chunkSize = data[position] | (static_cast<uint16_t>(data[position + 1]) << 8);
        position += 2;

        if (position + chunkSize > totalSize) {
            throw TapeError(ERROR_MALFORMED);
        }

        if (shouldValidateStrict && chunkSize < 2) {
            throw TapeError(ERROR_CHUNK_TOO_SMALL);
        }

        if (chunkSize == 0) {
            continue;
        }

        unsigned int chunkLast = position + chunkSize - 1;

        if (shouldValidateStrict) {
            uint8_t checksum = data[position];

            for (unsigned int chunkPos = position + 1; chunkPos < chunkLast; ++chunkPos) {
                checksum ^= data[chunkPos];
            }

            if (checksum != data[chunkLast]) {
                throw TapeError(ERROR_INVALID_CHECKSUM);
            }
        }

        totalTicks += (getPilotPulses(data[position]) * PILOT_PULSE_TICKS) +
                (SYNC_PULSE_FIRST_TICKS + SYNC_PULSE_SECOND_TICKS) +
                (DELAY_FIRST_TICKS + DELAY_SECOND_TICKS);

        for (unsigned int chunkPos = position; chunkPos <= chunkLast; ++chunkPos) {
            totalTicks += computeTicksForValue(data[chunkPos]);
        }

        chunks.push_back(Chunk {
                .offset = position,
                .size = chunkSize,
                .endTicks = totalTicks });

        position += chunkSize;
    }

    totalMicros = chronometer.srcToDstCeil(totalTicks);
    totalChunks = chunks.size();

    if (totalChunks) {
        initPilotState();
    } else if (shouldValidateStrict) {
        throw TapeError(ERROR_NO_CHUNKS);
    }
}

void TapeTap::initPilotState() {
    auto chunk = chunks[chunkIndex];

    chunkOffset = chunk.offset;
    chunkSizeLeft = chunk.size;

    nextState = StatePilot;
    nextWaitTicks = PILOT_PULSE_TICKS;

    pilotPulsesLeft = getPilotPulses(data[chunkOffset]);
}

void TapeTap::initBitsState() {
    bitsValue = data[chunkOffset];
    bitsMask = 0x80;
    initCurrentBitState();
}

uint64_t TapeTap::computeTicksForValue(uint8_t value) {
    uint64_t ticks = 0;

    for (uint8_t mask = 0x80; mask; mask >>= 1) {
        ticks += getPulseTicksForBit(value & mask) * 2;
    }

    return ticks;
}

}
