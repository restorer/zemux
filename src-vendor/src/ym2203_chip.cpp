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
 *
 *
 * Based on YM-2149F emulator for Unreal Speccy project
 * created under public domain license by SMT, jan.2006
 */

#include "ym2203_chip.h"
#include <vendor_ym2203/fm.h>

namespace zemux {

Ym2203Chip::Ym2203Chip(SoundSink* soundSink) : soundSink { soundSink } {
    chip = ym2203_init(nullptr, CLOCK_RATE, SAMPLING_RATE, nullptr, nullptr, nullptr);
}

Ym2203Chip::~Ym2203Chip() {
    ym2203_shutdown(chip);
}

void Ym2203Chip::select(uint8_t reg) {
    selectedReg = reg;
    ym2203_write(chip, 0, reg);
}

void Ym2203Chip::write(uint8_t value) {
    regs[selectedReg] = value;
    ym2203_write(chip, 0, value);
}

uint8_t Ym2203Chip::read() {
    return regs[selectedReg];
}

uint8_t Ym2203Chip::readStatus() {
    return ym2203_read(chip, 0);
}

void Ym2203Chip::reset() {
    ym2203_reset_chip(chip);
    ym2203_write(chip, 0, 0x27);
    ym2203_write(chip, 1, 0x00);
}

void Ym2203Chip::step(uint32_t ticks) {
    ym2203_update_one(chip, soundSink, ticks);
}

}
