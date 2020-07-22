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

namespace zemux {

const char* TapeTap::ERROR_MALFORMED = /* @i18n */ "tape_tap.malformed";
const char* TapeTap::ERROR_CHUNK_TOO_SMALL = /* @i18n */ "tape_tap.chunk_too_small";
const char* TapeTap::ERROR_INVALID_CHECKSUM = /* @i18n */ "tape_tap.invalid_checksum";
const char* TapeTap::ERROR_NO_CHUNKS = /* @i18n */ "tape_tap.no_chunks";

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

TapeTap::TapeTap(
        DataReader* reader,
        Loudspeaker* loudspeaker,
        bool shouldValidateStrict) : Tape { reader, loudspeaker } {

    data = reader->readEntire(MAX_TAP_SIZE);
    parseChunks(shouldValidateStrict);
}

#pragma clang diagnostic pop

void TapeTap::step(uint32_t micros) {
    if (currentChunkIndex >= totalChunks) {
        volumeBit = false;
        loudspeakerStep(micros);
        return;
    }

    elapsedMicros += micros;

    while (chronometer.getDstTicksPassed() < elapsedMicros) {
        loudspeakerStep(chronometer.srcAdvanceByDelta(currentWaitTicks));

        switch (currentState) {
            case StatePilot:
                volumeBit = !volumeBit;

                if (!(--currentPilotPulsesLeft)) {
                    currentState = StateSyncFirst;
                    currentWaitTicks = SYNC_PULSE_FIRST_TICKS;
                }
                break;

            case StateSyncFirst:
                volumeBit = !volumeBit;
                currentState = StateSyncSecond;
                currentWaitTicks = SYNC_PULSE_SECOND_TICKS;
                break;

            case StateSyncSecond:
                volumeBit = !volumeBit;
                initValueState();
                break;

            case StateBitFirst:
                volumeBit = !volumeBit;
                currentState = StateBitSecond;
                break;

            case StateBitSecond:
                volumeBit = !volumeBit;
                currentMask >>= 1;

                if (currentMask) {
                    initCurrentBitState();
                } else if (--currentSizeLeft) {
                    ++currentOffset;
                    initValueState();
                } else {
                    currentState = StateDelayFirst;
                    currentWaitTicks = DELAY_FIRST_TICKS;
                }
                break;

            case StateDelayFirst:
                volumeBit = false;
                currentState = StateDelaySecond;
                currentWaitTicks = DELAY_SECOND_TICKS;
                break;

            case StateDelaySecond:
                if (++currentChunkIndex >= totalChunks) {
                    elapsedMicros = totalMicros;
                } else {
                    initPilotState();
                }
                break;
        }
    }
}

void TapeTap::rewindToNearest(uint64_t micros) {
    volumeBit = false;
    currentChunkIndex = 0;
    currentProcessedTicks = 0;
    auto desiredTicks = chronometer.dstToSrcCeil(micros);

    while (currentChunkIndex < totalChunks && chunks[currentChunkIndex].endTicks <= desiredTicks) {
        currentProcessedTicks = chunks[currentChunkIndex].endTicks;
        currentChunkIndex++;
    }

    if (currentChunkIndex >= totalChunks) {
        elapsedMicros = totalMicros;
        return;
    }

    initPilotState();
    uint64_t endTicks = currentProcessedTicks + currentPilotPulsesLeft * PILOT_PULSE_TICKS;

    if (desiredTicks < endTicks) {
        unsigned int pulsesLeft = (endTicks - desiredTicks) / PILOT_PULSE_TICKS;
        unsigned int pulsesProcessed = currentPilotPulsesLeft - pulsesLeft;

        volumeBit = (pulsesProcessed % 2) == 0;
        currentProcessedTicks += pulsesProcessed * PILOT_PULSE_TICKS;
        currentPilotPulsesLeft = pulsesLeft;
        elapsedMicros = chronometer.srcToDstCeil(currentProcessedTicks);
        return;
    }

    volumeBit = (currentPilotPulsesLeft % 2) == 0;
    currentProcessedTicks = endTicks;
    endTicks += SYNC_PULSE_FIRST_TICKS + SYNC_PULSE_SECOND_TICKS;

    if (desiredTicks < endTicks) {
        currentState = StateSyncFirst;
        currentWaitTicks = SYNC_PULSE_FIRST_TICKS;
        elapsedMicros = chronometer.srcToDstCeil(currentProcessedTicks);
        return;
    }

    // volumeBit remains the same after sync
    currentProcessedTicks = endTicks;

    while (currentSizeLeft) {
        endTicks += computeTicksForValue(data[currentOffset]);

        if (desiredTicks < endTicks) {
            initValueState();
            elapsedMicros = chronometer.srcToDstCeil(currentProcessedTicks);
            return;
        }

        currentProcessedTicks = endTicks;
        ++currentOffset;
        --currentSizeLeft;
    }

    // volumeBit remains the same after bytes
    endTicks += DELAY_FIRST_TICKS;

    if (desiredTicks < endTicks) {
        currentState = StateDelayFirst;
        currentWaitTicks = DELAY_FIRST_TICKS;
        elapsedMicros = chronometer.srcToDstCeil(currentProcessedTicks);
        return;
    }

    volumeBit = false;
    currentState = StateDelaySecond;
    currentWaitTicks = DELAY_SECOND_TICKS - (desiredTicks - endTicks);
    currentProcessedTicks = desiredTicks;
    elapsedMicros = chronometer.srcToDstCeil(currentProcessedTicks);
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

    totalMicros = chronometer.dstToSrcCeil(totalTicks);
    totalChunks = chunks.size();

    if (totalChunks) {
        initPilotState();
    } else if (shouldValidateStrict) {
        throw TapeError(ERROR_NO_CHUNKS);
    }
}

void TapeTap::initPilotState() {
    auto chunk = chunks[currentChunkIndex];

    currentOffset = chunk.offset;
    currentSizeLeft = chunk.size;
    currentState = StatePilot;
    currentWaitTicks = PILOT_PULSE_TICKS;
    currentPilotPulsesLeft = getPilotPulses(data[currentOffset]);
}

void TapeTap::initValueState() {
    currentValue = data[currentOffset];
    currentMask = 0x80;
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
