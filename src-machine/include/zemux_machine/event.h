#ifndef ZEMUX_MACHINE__ACTION_HANDLER
#define ZEMUX_MACHINE__ACTION_HANDLER

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

namespace zemux {

namespace Event {

static constexpr int SHIFT_CATEGORY = 16;

enum Category {
    CategoryHost = 1 << SHIFT_CATEGORY,
    CategoryMemory = 2 << SHIFT_CATEGORY,
    CategoryKempstonJoystick = 3 << SHIFT_CATEGORY,
    CategoryKempstonMouse = 4 << SHIFT_CATEGORY,
    CategoryExtPort = 5 << SHIFT_CATEGORY,
    CategoryZxm = 6 << SHIFT_CATEGORY,
    CategoryKeyboard = 7 << SHIFT_CATEGORY,
};

}

union EventInput {
    int32_t value;
    void* pointer;
};

struct EventOutput {
    bool isHandled = false;
    int32_t value = 0;
};

class EventEmitter {
public:

    virtual EventOutput emitEvent(uint32_t event, EventInput input) = 0;
};

class EventListener {
public:

    virtual uint32_t getEventCategory() {
        return 0;
    }

    virtual EventOutput onEvent([[maybe_unused]] uint32_t type, [[maybe_unused]] EventInput input) {
        return EventOutput {};
    }

protected:

    constexpr EventListener() = default;
    virtual ~EventListener() = default;
};

}

#endif
