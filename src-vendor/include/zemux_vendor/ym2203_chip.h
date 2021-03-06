#ifndef ZEMUX_VENDOR__YM2203_CHIP
#define ZEMUX_VENDOR__YM2203_CHIP

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

#include <zemux_core/non_copyable.h>
#include <zemux_core/sound.h>
#include <cstdint>

namespace zemux {

class Ym2203Chip final : private NonCopyable {
public:

    static constexpr uint32_t CLOCK_RATE = 1774400 >> 3;
    static constexpr uint32_t SAMPLING_RATE = 44100;
    static constexpr int MAX_REGS = 0x100;

    explicit Ym2203Chip(SoundSink* soundSink);
    virtual ~Ym2203Chip();

    void select(uint8_t reg);
    void write(uint8_t value);
    uint8_t read();
    uint8_t readStatus();
    void reset();
    void step(uint32_t ticks);

private:

    SoundSink* soundSink;
    void* chip;
    uint8_t selectedReg = 0;
    uint8_t regs[MAX_REGS];
};

}

#endif
