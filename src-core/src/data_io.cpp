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

#include "data_io.h"

namespace zemux {

const char* DataReader::ERROR_TOO_LARGE_FILE = /* @i18n */ "data_reader.too_large_file";

uintmax_t DataReader::totalSize() {
    if (!isTotalSizeCached) {
        auto position = tell();
        seek(0, End);
        cachedTotalSize = tell();
        seek(position, Begin);
    }

    return cachedTotalSize;
}

std::vector<uint8_t> DataReader::readEntire(uintmax_t maxSize) {
    auto size = avail();

    if (size > maxSize) {
        throw DataIoError(ERROR_TOO_LARGE_FILE) << std::to_string(size) << std::to_string(maxSize);
    }

    std::vector<uint8_t> buffer;
    buffer.resize(size);

    readBlock(buffer.data(), size);
    return buffer;
}

}
