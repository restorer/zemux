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

TapeTap::TapeTap(DataReader* reader, Loudspeaker* loudspeaker, bool shouldValidate) : Tape { reader, loudspeaker } {
    data = reader->readEntire(MAX_TAP_SIZE);
    parseChunks(shouldValidate);
}

void TapeTap::step(unsigned int micros) {
    if (currentChunkIndex >= totalChunks) {
        return;
    }

    elapsedMicros += micros;

    while (currentProcessedMicros < elapsedMicros) {
        if (currentWaitMicros > elapsedMicros - currentProcessedMicros) {
            return;
        }

        loudspeakerStep(currentWaitMicros);
        currentProcessedMicros += currentWaitMicros;

        switch (currentState) {
            case StatePilotMainLow:
                volumeBit = true;
                currentState = StatePilotMainHigh;
                break;

            case StatePilotMainHigh:
                volumeBit = false;

                if (--currentPilotCount) {
                    currentState = StatePilotMainLow;
                } else {
                    currentState = StatePilotCapLow;
                    currentWaitMicros = PILOT_CAP_LOW_MICROS;
                }
                break;

            case StatePilotCapLow:
                volumeBit = true;
                currentState = StatePilotCapHigh;
                currentWaitMicros = PILOT_CAP_HIGH_MICROS;
                break;

            case StatePilotCapHigh:
                volumeBit = false;
                initValueState();
                break;

            case StateBitLow:
                volumeBit = true;
                currentState = StateBitHigh;
                break;

            case StateBitHigh:
                volumeBit = false;
                currentMask >>= 1;

                if (currentMask) {
                    initCurrentBitState();
                } else if (--currentSizeLeft) {
                    ++currentOffset;
                    initValueState();
                } else {
                    currentState = StateDelay;
                    currentWaitMicros = DELAY_MICROS;
                }
                break;

            case StateDelay:
                if (++currentChunkIndex >= totalChunks) {
                    elapsedMicros = totalMicros;
                } else {
                    initPilotState();
                }
                break;
        }
    }
}

void TapeTap::rewindToNearest(unsigned int micros) {
    volumeBit = false;
    currentChunkIndex = 0;
    currentProcessedMicros = 0;

    while (currentChunkIndex < totalChunks && chunks[currentChunkIndex].endMicros <= micros) {
        currentProcessedMicros = chunks[currentChunkIndex].endMicros;
        currentChunkIndex++;
    }

    if (currentChunkIndex >= totalChunks) {
        elapsedMicros = totalMicros;
        return;
    }

    initPilotState();
    unsigned int endMicros = currentProcessedMicros + currentPilotCount * PILOT_MAIN_HALF_MICROS * 2;

    if (micros < endMicros) {
        unsigned int pilotPassed = (endMicros - micros) / (PILOT_MAIN_HALF_MICROS * 2);

        currentProcessedMicros += pilotPassed * (PILOT_MAIN_HALF_MICROS * 2);
        currentPilotCount -= pilotPassed;
        elapsedMicros = currentProcessedMicros;
        return;
    }

    currentProcessedMicros = endMicros;
    endMicros += PILOT_CAP_LOW_MICROS + PILOT_CAP_HIGH_MICROS;

    if (micros < endMicros) {
        currentState = StatePilotCapLow;
        currentWaitMicros = PILOT_CAP_LOW_MICROS;
        elapsedMicros = currentProcessedMicros;
        return;
    }

    currentProcessedMicros = endMicros;

    while (currentSizeLeft) {
        endMicros += computeMicrosForValue(data[currentOffset]);

        if (micros < endMicros) {
            initValueState();
            elapsedMicros = currentProcessedMicros;
            return;
        }

        currentProcessedMicros = endMicros;
        ++currentOffset;
        --currentSizeLeft;
    }

    currentState = StateDelay;
    currentWaitMicros = DELAY_MICROS - (micros - endMicros);
    currentProcessedMicros = micros;
    elapsedMicros = currentProcessedMicros;
}

void TapeTap::parseChunks(bool shouldValidate) {
    unsigned int totalSize = data.size();
    unsigned int position = 0;

    while (position < totalSize) {
        if (position + 2 > totalSize) {
            throw TapeError(ERROR_MALFORMED);
        }

        unsigned int chunkSize = data[position] | (static_cast<uint16_t>(data[position + 1]) << 8);
        position += 2;

        if (position + chunkSize > totalSize) {
            throw TapeError(ERROR_MALFORMED);
        }

        if (shouldValidate && chunkSize < 2) {
            throw TapeError(ERROR_CHUNK_TOO_SMALL);
        }

        if (chunkSize == 0) {
            continue;
        }

        unsigned int chunkLast = position + chunkSize - 1;

        if (shouldValidate) {
            uint8_t checksum = data[position];

            for (unsigned int chunkPos = position + 1; chunkPos < chunkLast; ++chunkPos) {
                checksum ^= data[chunkPos];
            }

            if (checksum != data[chunkLast]) {
                throw TapeError(ERROR_INVALID_CHECKSUM);
            }
        }

        totalMicros += (getPilotCount(data[position]) * PILOT_MAIN_HALF_MICROS * 2) +
                (PILOT_CAP_LOW_MICROS + PILOT_CAP_HIGH_MICROS) +
                DELAY_MICROS;

        for (unsigned int chunkPos = position; chunkPos <= chunkLast; ++chunkPos) {
            totalMicros += computeMicrosForValue(data[chunkPos]);
        }

        chunks.push_back(Chunk {
                .offset = position,
                .size = chunkSize,
                .endMicros = totalMicros });
    }

    totalChunks = chunks.size();

    if (totalChunks) {
        initPilotState();
    } else if (shouldValidate) {
        throw TapeError(ERROR_NO_CHUNKS);
    }
}

void TapeTap::initPilotState() {
    auto chunk = chunks[currentChunkIndex];

    currentOffset = chunk.offset;
    currentSizeLeft = chunk.size;
    currentState = StatePilotMainLow;
    currentWaitMicros = PILOT_MAIN_HALF_MICROS;
    currentPilotCount = getPilotCount(data[currentOffset]);
}

void TapeTap::initValueState() {
    currentValue = data[currentOffset];
    currentMask = 0x80;

    initCurrentBitState();
}

unsigned int TapeTap::computeMicrosForValue(uint8_t value) {
    unsigned int micros = 0;

    for (uint8_t mask = 0x80; mask; mask >>= 1) {
        micros += getHalfMicrosForBit(value & mask) * 2;
    }

    return micros;
}

}
