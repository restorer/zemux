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
 *
 *
 * Based on YM-2149F emulator for Unreal Speccy project
 * created under public domain license by SMT, jan.2006
 */

#include "saa1099_chip.h"
#include <vendor_saa1099/SAASound.h>

namespace zemux {

Saa1099Chip::Saa1099Chip(SoundSink* soundSink) : soundSink { soundSink } {
    chip = CreateCSAASound();
    chip->SetSoundParameters(SAAP_NOFILTER | SAAP_44100 | SAAP_16BIT | SAAP_STEREO);
}

Saa1099Chip::~Saa1099Chip() {
    DestroyCSAASound(chip);
}

void Saa1099Chip::writeAddress(uint8_t reg) {
    chip->WriteAddress(reg);
}

void Saa1099Chip::writeData(uint8_t data) {
    chip->WriteData(data);
}

void Saa1099Chip::reset() {
    chip->Clear();
}

void Saa1099Chip::step(uint32_t ticks) {
    chip->GenerateMany(soundSink, ticks);
}

}