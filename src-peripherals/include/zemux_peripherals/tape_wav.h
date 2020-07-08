#ifndef ZEMUX_PERIPHERALS__TAPE_WAV
#define ZEMUX_PERIPHERALS__TAPE_WAV

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

#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/math_ext.h>
#include <zemux_core/data_io.h>
#include <zemux_core/chronometer.h>
#include "tape.h"

namespace zemux {

class TapeWav final : public Tape, private NonCopyable {
public:

    static constexpr uint16_t DEFAULT_THRESHOLD = 140;

    static const char* ERROR_MALFORMED;
    static const char* ERROR_CHUNK_MISSING;
    static const char* ERROR_CHUNK_SIZE_INVALID;
    static const char* ERROR_CHUNK_TYPE_INVALID;
    static const char* ERROR_CHUNK_MULTIPLE;
    static const char* ERROR_AUDIO_FORMAT_NOT_SUPPORTED;
    static const char* ERROR_INVALID_NUM_CHANNELS;
    static const char* ERROR_INVALID_SAMPLE_RATE;
    static const char* ERROR_BITS_PER_SAMPLE_NOT_SUPPORTED;
    static const char* ERROR_INVALID_BLOCK_ALIGN;
    static const char* ERROR_INVALID_BYTE_RATE;

    TapeWav(DataReader* reader,
            Loudspeaker* loudspeaker,
            bool shouldValidateStrict,
            uint16_t threshold = DEFAULT_THRESHOLD);

    ~TapeWav() override = default;

    inline uint16_t getThreshold() const {
        return threshold;
    }

    inline void setThreshold(uint16_t value) {
        threshold = value;
    }

    void step(unsigned int micros) override;
    void rewindToNearest(unsigned int micros) override;

private:

#pragma pack(push, 1)
    struct ChunkHeader {
        uint32_t id;
        uint32_t size;
    };

    struct FmtContent {
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate; // >= numChannels * sizeof(sample) * sampleRate
        uint16_t blockAlign; // >= numChannels * sizeof(sample)
        uint16_t bitsPerSample; // == sizeof(sample) * 8
    };
#pragma pack(pop)

    static constexpr uint32_t RIFF_ID = 0x46464952; // "RIFF"
    static constexpr uint32_t RIFF_TYPE = 0x45564157; // "WAVE"
    static constexpr uint32_t FMT_ID = 0x20746D66; // "fmt "
    static constexpr uint16_t FMT_AUDIO_FORMAT_PCM = 1;
    static constexpr uint16_t FMT_NUM_CHANNELS_1 = 1;
    static constexpr uint16_t FMT_NUM_CHANNELS_2 = 2;
    static constexpr uint16_t FMT_BITS_PER_SAMPLE_8 = 8;
    static constexpr uint16_t FMT_BITS_PER_SAMPLE_16 = 16;
    static constexpr uint16_t FMT_BITS_PER_SAMPLE_24 = 24;
    static constexpr uint16_t FMT_BITS_PER_SAMPLE_32 = 32;
    static constexpr uint32_t DATA_ID = 0x61746164; // "data"

    static constexpr unsigned int RIFF_MIN_SIZE = sizeof(uint32_t) + // riff
            sizeof(ChunkHeader) + // fmt
            sizeof(FmtContent) + // fmt
            sizeof(ChunkHeader); // data

    static constexpr unsigned int TOTAL_MIN_SIZE = sizeof(ChunkHeader) + RIFF_MIN_SIZE;

    uint16_t threshold;
    uintmax_t lastPosition;
    FmtContent fmt;
    uintmax_t dataPosition;
    uintmax_t dataSize;
    Chronometer chronometer { SECOND_MICROS, SECOND_MICROS };
    unsigned int blockPadding;
    int64_t (TapeWav::* readSamplePtr)();
    uintmax_t currentDataOffset = 0;
    unsigned int currentSampleMicros = 0;

    ZEMUX_FORCE_INLINE int64_t readSampleB24Inline() {
        return extend24To32(reader->readUInt16() | (static_cast<uint32_t>(reader->readUInt8()) << 16));
    }

    void parseRiffChunk(bool shouldValidateStrict);
    void parseFmtAndData(bool shouldValidateStrict);
    void parseFmtContent(uint32_t size, bool shouldValidateStrict);
    void parseDataHeader(uint32_t size);
    ChunkHeader readChunkHeader();

    int64_t readSampleB8();
    int64_t readSampleB8C1();
    int64_t readSampleB8C2();

    int64_t readSampleB16();
    int64_t readSampleB16C1();
    int64_t readSampleB16C2();

    int64_t readSampleB24();
    int64_t readSampleB24C1();
    int64_t readSampleB24C2();

    int64_t readSampleB32();
    int64_t readSampleB32C1();
    int64_t readSampleB32C2();
};

}

#endif
