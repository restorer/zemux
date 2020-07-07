#ifndef ZEMUX_CORE__DATA_IO
#define ZEMUX_CORE__DATA_IO

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
#include <vector>
#include "force_inline.h"
#include "error.h"

namespace zemux {

class DataReader {
public:

    static const char* ERROR_TOO_LARGE_FILE;

    enum SeekDirection {
        Begin,
        Current,
        End,
    };

    virtual bool isEof() = 0;
    virtual uint8_t readUInt8() = 0;
    virtual uintmax_t readBlock(void* buffer, uintmax_t size) = 0;
    virtual uintmax_t tell() = 0;
    virtual void seek(intmax_t offset, SeekDirection direction) = 0;

    ZEMUX_FORCE_INLINE int8_t readInt8() {
        return static_cast<int8_t>(readUInt8());
    }

    inline uint16_t readUInt16() {
        return readUInt8() | (static_cast<uint16_t>(readUInt8()) << 8);
    }

    ZEMUX_FORCE_INLINE int16_t readInt16() {
        return static_cast<int16_t>(readUInt16());
    }

    inline uint32_t readUInt32() {
        return readUInt16() | (static_cast<uint32_t>(readUInt16()) << 16);
    }

    ZEMUX_FORCE_INLINE int32_t readInt32() {
        return static_cast<int32_t>(readUInt32());
    }

    ZEMUX_FORCE_INLINE void seek(intmax_t offset) {
        seek(offset, Begin);
    }

    ZEMUX_FORCE_INLINE void skip(uintmax_t size) {
        seek(size, Current);
    }

    inline uintmax_t avail() {
        return totalSize() - tell();
    }

    uintmax_t totalSize();
    std::vector<uint8_t> readEntire(uintmax_t maxSize);

protected:

    constexpr DataReader() = default;
    ~DataReader() = default;

    bool isTotalSizeCached = false;
    uintmax_t cachedTotalSize = 0;

};

class DataIoError final : public AbstractRuntimeError<DataIoError> {
public:

    explicit DataIoError(const std::string& key) : AbstractRuntimeError { key } {
    }
};

}

#endif
