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

#include "tape_wav.h"

static std::string idToString(uint32_t id) {
    char buffer[5];

    buffer[0] = static_cast<char>(id & 0xFF);
    buffer[1] = static_cast<char>((id >> 8) & 0xFF);
    buffer[2] = static_cast<char>((id >> 16) & 0xFF);
    buffer[3] = static_cast<char>(id >> 24);
    buffer[4] = '\0';

    return std::string(buffer);
}

namespace zemux {

const char* TapeWav::ERROR_MALFORMED = /* @i18n */ "tape_wav.malformed";
const char* TapeWav::ERROR_CHUNK_MISSING = /* @i18n */ "tape_wav.chunk_missing";
const char* TapeWav::ERROR_CHUNK_SIZE_INVALID = /* @i18n */ "tape_wav.chunk_size_invalid";
const char* TapeWav::ERROR_CHUNK_TYPE_INVALID = /* @i18n */ "tape_wav.chunk_type_invalid";
const char* TapeWav::ERROR_CHUNK_MULTIPLE = /* @i18n */ "tape_wav.chunk_multiple";
const char* TapeWav::ERROR_AUDIO_FORMAT_NOT_SUPPORTED = /* @i18n */ "tape_wav.audio_format_not_supported";
const char* TapeWav::ERROR_INVALID_NUM_CHANNELS = /* @i18n */ "tape_wav.invalid_num_channels";
const char* TapeWav::ERROR_INVALID_SAMPLE_RATE = /* @i18n */ "tape_wav.invalid_sample_rate";
const char* TapeWav::ERROR_BITS_PER_SAMPLE_NOT_SUPPORTED = /* @i18n */ "tape_wav.bits_per_sample_not_supported";
const char* TapeWav::ERROR_INVALID_BLOCK_ALIGN = /* @i18n */ "tape_wav.invalid_block_align";
const char* TapeWav::ERROR_INVALID_BYTE_RATE = /* @i18n */ "tape_wav.invalid_byte_rate";

TapeWav::TapeWav(
        DataReader* reader,
        SoundSink* soundSink,
        bool shouldValidateStrict,
        uint8_t threshold
) : Tape { reader, soundSink }, threshold { threshold } {
    parseRiffChunk(shouldValidateStrict);
    parseFmtAndData(shouldValidateStrict);
    reader->seek(dataPosition);
}

void TapeWav::step(uint32_t micros) {
    if (elapsedMicros + static_cast<uint64_t>(micros) >= totalMicros) {
        micros = totalMicros - elapsedMicros;
    }

    if (!micros) {
        volumeStep(false, 0);
        return;
    }

    elapsedMicros += micros;

    int64_t sampleThreshold = (static_cast<uint64_t>(threshold) << thresholdShift) *
            static_cast<uint64_t>(fmt.numChannels);

    while (micros--) {
        unsigned int numSamples = chronometer.srcAdvanceByDelta(1);
        ++currentSampleMicros;

        if (!numSamples) {
            continue;
        }

        int64_t sample = 0;
        int64_t divider = numSamples;

        while (numSamples--) {
            if (currentDataOffset + fmt.blockAlign > dataSize) {
                elapsedMicros = totalMicros;
                return;
            }

            currentDataOffset += fmt.blockAlign;
            sample += (this->*readSamplePtr)();

            if (blockPadding) {
                reader->skip(blockPadding);
            }
        }

        volumeStep(sample / divider + sampleOffset >= sampleThreshold, currentSampleMicros);
        currentSampleMicros = 0;
    }
}

void TapeWav::rewindToNearest(uint64_t micros) {
    if (micros > totalMicros) {
        micros = totalMicros;
    }

    uint32_t postStepMicros = chronometer.dstToSrcCeil(1);

    if (micros >= postStepMicros) {
        micros -= postStepMicros;
    } else {
        postStepMicros = 0;
    }

    elapsedMicros = micros;
    chronometer.setSrcTicksPassed(micros);
    currentDataOffset = chronometer.getDstTicksPassed() * fmt.blockAlign;
    currentSampleMicros = 0;

    if (currentDataOffset + fmt.blockAlign > dataSize) {
        elapsedMicros = totalMicros;
    } else {
        reader->seek(dataPosition + currentDataOffset);
    }

    if (postStepMicros) {
        step(postStepMicros);
    } else {
        volumeStep(false, 0);
    }
}

void TapeWav::parseRiffChunk(bool shouldValidateStrict) {
    if (reader->avail() < TOTAL_MIN_SIZE) {
        throw TapeError(ERROR_MALFORMED);
    }

    auto position = reader->tell();
    ChunkHeader header = readChunkHeader();

    if (header.id != RIFF_ID) {
        throw TapeError(ERROR_CHUNK_MISSING) << idToString(RIFF_ID);
    }

    if (shouldValidateStrict) {
        if (header.size < RIFF_MIN_SIZE || header.size > reader->avail()) {
            throw TapeError(ERROR_CHUNK_SIZE_INVALID) << idToString(RIFF_ID);
        }

        lastPosition = position + sizeof(ChunkHeader) + header.size;
    } else {
        lastPosition = reader->totalSize();
    }

    if (reader->readUInt32() != RIFF_TYPE) {
        throw TapeError(ERROR_CHUNK_TYPE_INVALID) << idToString(RIFF_TYPE);
    }
}

void TapeWav::parseFmtAndData(bool shouldValidateStrict) {
    bool isFmtFound = false;
    bool isDataFound = false;

    while (lastPosition - reader->tell() >= sizeof(ChunkHeader)) {
        auto position = reader->tell();
        ChunkHeader header = readChunkHeader();

        if (position + sizeof(ChunkHeader) + header.size > lastPosition) {
            throw TapeError(ERROR_CHUNK_SIZE_INVALID) << idToString(header.id);
        }

        switch (header.id) {
            case FMT_ID:
                if (isFmtFound) {
                    if (shouldValidateStrict) {
                        throw TapeError(ERROR_CHUNK_MULTIPLE) << idToString(FMT_ID);
                    }

                    continue;
                }

                isFmtFound = true;
                parseFmtContent(header.size, shouldValidateStrict);
                break;

            case DATA_ID:
                if (isDataFound) {
                    if (shouldValidateStrict) {
                        throw TapeError(ERROR_CHUNK_MULTIPLE) << idToString(DATA_ID);
                    }

                    continue;
                }

                isDataFound = true;
                parseDataHeader(header.size);
                break;

            default:
                break;
        }

        if (!shouldValidateStrict && isFmtFound && isDataFound) {
            return;
        }

        reader->seek(position + sizeof(ChunkHeader) + header.size);
    }

    if (!isFmtFound) {
        throw std::move(TapeError(ERROR_CHUNK_MISSING) << idToString(FMT_ID));
    }

    if (!isDataFound) {
        throw TapeError(ERROR_CHUNK_MISSING) << idToString(DATA_ID);
    }
}

void TapeWav::parseFmtContent(uint32_t size, bool shouldValidateStrict) {
    if (lastPosition - reader->tell() < sizeof(FmtContent) && size < sizeof(FmtContent)) {
        throw TapeError(ERROR_CHUNK_SIZE_INVALID) << idToString(FMT_ID);
    }

    fmt.audioFormat = reader->readUInt16();
    fmt.numChannels = reader->readUInt16();
    fmt.sampleRate = reader->readUInt32();
    fmt.byteRate = reader->readUInt32();
    fmt.blockAlign = reader->readUInt16();
    fmt.bitsPerSample = reader->readUInt16();

    if (fmt.audioFormat != FMT_AUDIO_FORMAT_PCM) {
        throw TapeError(ERROR_AUDIO_FORMAT_NOT_SUPPORTED);
    }

    if (!fmt.numChannels) {
        throw TapeError(ERROR_INVALID_NUM_CHANNELS);
    }

    if (!fmt.sampleRate) {
        throw TapeError(ERROR_INVALID_SAMPLE_RATE);
    }

    switch (fmt.bitsPerSample) {
        case FMT_BITS_PER_SAMPLE_8: {
            thresholdShift = 0;
            sampleOffset = 0x80;

            switch (fmt.numChannels) {
                case FMT_NUM_CHANNELS_1:
                    readSamplePtr = &TapeWav::readSampleB8C1;
                    break;

                case FMT_NUM_CHANNELS_2:
                    readSamplePtr = &TapeWav::readSampleB8C2;
                    break;

                default:
                    readSamplePtr = &TapeWav::readSampleB8;
                    break;
            }

            break;
        }

        case FMT_BITS_PER_SAMPLE_16: {
            thresholdShift = 8;
            sampleOffset = 0x8000;

            switch (fmt.numChannels) {
                case FMT_NUM_CHANNELS_1:
                    readSamplePtr = &TapeWav::readSampleB16C1;
                    break;

                case FMT_NUM_CHANNELS_2:
                    readSamplePtr = &TapeWav::readSampleB16C2;
                    break;

                default:
                    readSamplePtr = &TapeWav::readSampleB16;
                    break;
            }

            break;
        }

        case FMT_BITS_PER_SAMPLE_24: {
            thresholdShift = 16;
            sampleOffset = 0x800000;

            switch (fmt.numChannels) {
                case FMT_NUM_CHANNELS_1:
                    readSamplePtr = &TapeWav::readSampleB24C1;
                    break;

                case FMT_NUM_CHANNELS_2:
                    readSamplePtr = &TapeWav::readSampleB24C2;
                    break;

                default:
                    readSamplePtr = &TapeWav::readSampleB24;
                    break;
            }

            break;
        }

        case FMT_BITS_PER_SAMPLE_32: {
            thresholdShift = 24;
            sampleOffset = 0x80000000;

            switch (fmt.numChannels) {
                case FMT_NUM_CHANNELS_1:
                    readSamplePtr = &TapeWav::readSampleB32C1;
                    break;

                case FMT_NUM_CHANNELS_2:
                    readSamplePtr = &TapeWav::readSampleB32C2;
                    break;

                default:
                    readSamplePtr = &TapeWav::readSampleB32;
                    break;
            }

            break;
        }

        default:
            throw TapeError(ERROR_BITS_PER_SAMPLE_NOT_SUPPORTED) << std::to_string(fmt.bitsPerSample);
    }

    sampleOffset *= static_cast<uint64_t>(fmt.numChannels);
    unsigned int minBlockAlign = fmt.numChannels * fmt.bitsPerSample / 8;

    if (fmt.blockAlign < minBlockAlign) {
        throw TapeError(ERROR_INVALID_BLOCK_ALIGN);
    }

    if (shouldValidateStrict && fmt.byteRate != fmt.blockAlign * fmt.sampleRate) {
        throw TapeError(ERROR_INVALID_BYTE_RATE);
    }

    chronometer.setDstClockRateFixedSrc(fmt.sampleRate);
    blockPadding = fmt.blockAlign - minBlockAlign;
}

void TapeWav::parseDataHeader(uint32_t size) {
    dataPosition = reader->tell();
    dataSize = size;
    totalMicros = (size / fmt.blockAlign) * SECOND_MICROS / fmt.sampleRate;
}

TapeWav::ChunkHeader TapeWav::readChunkHeader() {
    return ChunkHeader {
            .id = reader->readUInt32(),
            .size = reader->readUInt32() };
}

int64_t TapeWav::readSampleB8() {
    int64_t result = 0;

    for (uint16_t channels = fmt.numChannels; channels--;) {
        result += reader->readInt8();
    }

    return result;
}

int64_t TapeWav::readSampleB8C1() {
    return reader->readInt8();
}

int64_t TapeWav::readSampleB8C2() {
    return static_cast<int64_t>(reader->readInt8()) + static_cast<int64_t>(reader->readInt8());
}

int64_t TapeWav::readSampleB16() {
    int64_t result = 0;

    for (uint16_t channels = fmt.numChannels; channels--;) {
        result += reader->readInt16();
    }

    return result;
}

int64_t TapeWav::readSampleB16C1() {
    return reader->readInt16();
}

int64_t TapeWav::readSampleB16C2() {
    return static_cast<int64_t>(reader->readInt16()) + static_cast<int64_t>(reader->readInt16());
}

int64_t TapeWav::readSampleB24() {
    int64_t result = 0;

    for (uint16_t channels = fmt.numChannels; channels--;) {
        result += readSampleB24Inline();
    }

    return result;
}

int64_t TapeWav::readSampleB24C1() {
    return readSampleB24Inline();
}

int64_t TapeWav::readSampleB24C2() {
    return readSampleB24Inline() + readSampleB24Inline();
}

int64_t TapeWav::readSampleB32() {
    int64_t result = 0;

    for (uint16_t channels = fmt.numChannels; channels--;) {
        result += reader->readInt32();
    }

    return result;
}

int64_t TapeWav::readSampleB32C1() {
    return reader->readInt32();
}

int64_t TapeWav::readSampleB32C2() {
    return static_cast<int64_t>(reader->readInt32()) + static_cast<int64_t>(reader->readInt32());
}

}
