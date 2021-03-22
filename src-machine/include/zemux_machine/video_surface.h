#ifndef ZEMUX_MACHINE__VIDEO_SURFACE
#define ZEMUX_MACHINE__VIDEO_SURFACE

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
#include <memory>
#include <zemux_core/force_inline.h>

namespace zemux {

class VideoSurface {
public:

    static constexpr int ROWS = 312;
    static constexpr int COLS = 384 * 2;

    std::unique_ptr<uint32_t[]> canvas;
    uint32_t* beam;

    ZEMUX_FORCE_INLINE void startFrame() {
        beam = canvas.get();
    }

    void finishFrame(uint32_t c) {
        uint32_t* last = &canvas[ROWS * COLS];

        while (beam != last) {
            *(beam++) = c;
        }
    }

    ZEMUX_FORCE_INLINE static constexpr uint32_t combineRgb(uint8_t r, uint8_t g, uint8_t b) {
        return (static_cast<uint32_t>(r) << 0x10) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b);
    }

    ZEMUX_FORCE_INLINE static constexpr uint8_t extractR(uint32_t c) {
        return static_cast<uint8_t>(c >> 0x10);
    }

    ZEMUX_FORCE_INLINE static constexpr uint8_t extractG(uint32_t c) {
        return static_cast<uint8_t>(c >> 8);
    }

    ZEMUX_FORCE_INLINE static constexpr uint8_t extractB(uint32_t c) {
        return static_cast<uint8_t>(c);
    }
};

}

#endif
