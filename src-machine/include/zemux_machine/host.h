#ifndef ZEMUX_MACHINE__HOST
#define ZEMUX_MACHINE__HOST

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

namespace zemux {

namespace Host {

enum EventType {
    EventGetMouseState = Event::CategoryHost | 1,
};

}

struct HostMouseState {
    static constexpr int BUTTON_BIT_LEFT = 1;
    static constexpr int BUTTON_BIT_RIGHT = 2;
    static constexpr int BUTTON_BIT_MIDDLE = 3;

    int32_t motionX;
    int32_t motionY;
    int buttons;
};

}

#endif
