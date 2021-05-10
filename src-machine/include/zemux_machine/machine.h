#ifndef ZEMUX_MACHINE__GUEST_MACHINE
#define ZEMUX_MACHINE__GUEST_MACHINE

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

#include <memory>
#include <vector>
#include <zemux_core/non_copyable.h>
#include "bus.h"
#include "video/video_surface.h"
#include "sound/sound_desk.h"

namespace zemux {

class Machine final : private NonCopyable {
public:

    Bus bus;
    VideoSurface videoSurface;
    SoundDesk soundDesk;

private:

    // Pentagon:
    uint32_t ulaLineTotalTicks = 224;
    uint32_t ulaHBlankTicks = 32;
    uint32_t ulaVBlankTicks = 16 * ulaLineTotalTicks;
    uint32_t ulaFrameTicks = ulaVBlankTicks + (64 + 192 + 48) * ulaLineTotalTicks;
    uint32_t ulaIntBeginTicks = 0;
    uint32_t ulaIntEndTicks = ulaIntBeginTicks + 32;

    // Scorpion:
    // uint32_t ulaLineTotalTicks = 224;
    // uint32_t ulaHBlankTicks = 40;
    // uint32_t ulaVBlankTicks = 16 * ulaLineTotalTicks;
    // uint32_t ulaFrameTicks = ulaVBlankTicks + (64 + 192 + 40) * ulaLineTotalTicks;
    // uint32_t ulaIntBeginTicks = ulaVBlankTicks;
    // uint32_t ulaIntEndTicks = ulaIntBeginTicks + 32;

    // Spectrum48:
    // uint32_t ulaLineTotalTicks = 224;
    // uint32_t ulaHBlankTicks = 48;
    // uint32_t ulaVBlankTicks = 16 * ulaLineTotalTicks;
    // uint32_t ulaFrameTicks = ulaVBlankTicks + (48 + 192 + 56) * ulaLineTotalTicks;
    // uint32_t ulaIntBeginTicks = 0;
    // uint32_t ulaIntEndTicks = ulaIntBeginTicks + 32;

    // Spectrum128:
    // uint32_t ulaLineTotalTicks = 224;
    // uint32_t ulaHBlankTicks = 48;
    // uint32_t ulaVBlankTicks = 15 * ulaLineTotalTicks;
    // uint32_t ulaFrameTicks = ulaVBlankTicks + (48 + 192 + 56) * ulaLineTotalTicks;
    // uint32_t ulaIntBeginTicks = 0;
    // uint32_t ulaIntEndTicks = ulaIntBeginTicks + 32;

    uint32_t ulaLineVisibleTicks = ulaLineTotalTicks - ulaHBlankTicks;
};

}

#endif
