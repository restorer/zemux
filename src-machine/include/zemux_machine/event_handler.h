#ifndef ZEMUX_MACHINE__ACTION_HANDLER
#define ZEMUX_MACHINE__ACTION_HANDLER

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

#include <memory>

namespace zemux {

union EventData {
    uint32_t value;
    void* pointer;
};

class EventHandler {
public:

    static constexpr int SHIFT_PREFIX = 16;
    static constexpr uint32_t PREFIX_MEMORY_DEVICE = 1 << SHIFT_PREFIX;

    virtual uint32_t getEventPrefix() {
        return 0;
    }

    virtual EventData onEvent([[maybe_unused]] uint32_t event, [[maybe_unused]] EventData input) {
        return EventData {};
    }

protected:

    constexpr EventHandler() = default;
    virtual ~EventHandler() = default;
};

}

#endif
