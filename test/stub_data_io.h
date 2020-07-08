#ifndef ZEMUX_TEST__STUB_DATA_IO
#define ZEMUX_TEST__STUB_DATA_IO

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

#include <string>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <boost/format.hpp>
#include <zemux_core/non_copyable.h>
#include <zemux_core/data_io.h>

class FileDataReader final : public zemux::DataReader, private zemux::NonCopyable {
public:

    explicit FileDataReader(const std::string& path) {
        ifs.open(path, std::ifstream::in | std::ifstream::binary);

        if (ifs.fail()) {
            ifs.close();
            throw std::runtime_error((boost::format("Failed to read from \"%s\"") % path).str());
        }
    }

    ~FileDataReader() override {
        ifs.close();
    }

    bool isEof() override {
        return ifs.eof();
    }

    uint8_t readUInt8() override {
        return ifs.get();
    }

    uintmax_t readBlock(void* buffer, uintmax_t maxSize) override {
        ifs.read(static_cast<char*>(buffer), maxSize);
        return ifs.gcount();
    }

    uintmax_t tell() override {
        return ifs.tellg();
    }

    void seek(intmax_t offset, SeekDirection direction) override {
        ifs.seekg(offset, direction == Begin ? std::ios::beg : (direction == End ? std::ios::end : std::ios::cur));
    }

private:

    std::ifstream ifs;
};

#endif
