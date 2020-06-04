#ifndef ZEMUX__GUEST__CHRONOGRAPH
#define ZEMUX__GUEST__CHRONOGRAPH

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

#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>

namespace zemux {

class Chronograph final : private NonCopyable {
public:

    Chronograph(unsigned int systemClockRate, unsigned int deviceClockRate);

    ZEMUX_FORCE_INLINE int getDeviceToSystemRatio() {
        return deviceToSystemRatio;
    }

    ZEMUX_FORCE_INLINE unsigned int getSystemClockRate() {
        return systemClockRate;
    }

    ZEMUX_FORCE_INLINE unsigned int getDeviceClockRate() {
        return deviceClockRate;
    }

    ZEMUX_FORCE_INLINE unsigned int getSystemTicksPassed() {
        return systemTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int getDeviceTicksPassed() {
        return deviceTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int systemForwardTo(unsigned int systemTicks) {
        systemTicksPassed = systemTicks;
        deviceTicksPassed = std::max(deviceTicksPassed, convertSystemToDevice(systemTicks));
        return deviceTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int systemAdvanceBy(unsigned int systemTicksDelta) {
        return systemForwardTo(systemTicksPassed + systemTicksDelta);
    }

    ZEMUX_FORCE_INLINE unsigned int systemAdvanceByRel(unsigned int systemTicksDelta) {
        unsigned int prevSystemTicksPassed = systemTicksPassed;
        return systemAdvanceBy(prevSystemTicksPassed + systemTicksDelta) - prevSystemTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int deviceForwardTo(unsigned int deviceTicks) {
        deviceTicksPassed = deviceTicks;
        systemTicksPassed = std::max(systemTicksPassed, convertDeviceToSystem(deviceTicks));
        return systemTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int deviceAdvanceBy(unsigned int deviceTicksDelta) {
        return deviceForwardTo(deviceTicksPassed + deviceTicksDelta);
    }

    ZEMUX_FORCE_INLINE unsigned int deviceAdvanceByRel(unsigned int deviceTicksDelta) {
        unsigned int prevDeviceTicksPassed = deviceTicksPassed;
        return deviceForwardTo(prevDeviceTicksPassed + deviceTicksDelta) - prevDeviceTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int convertSystemToDeviceDelta(unsigned int systemTicksDelta) {
        return std::max(1U, convertSystemToDevice(systemTicksDelta));
    }

    ZEMUX_FORCE_INLINE unsigned int convertDeviceToSystemDelta(unsigned int deviceTicksDelta) {
        return std::max(1U, convertDeviceToSystem(deviceTicksDelta));
    }

    void setDeviceToSystemRatio(int ratio);
    void setSystemClockRate(unsigned int clockRate);
    void setDeviceClockRate(unsigned int clockRate);
    void systemConsume(unsigned int systemTicks);
    void deviceConsume(unsigned int deviceTicks);

private:

    static constexpr FP_MATH_SHIFT = 16;
    static constexpr FP_MATH_CEIL_ADDENT = 0xFFFF;

    unsigned int systemClockRate;
    unsigned int deviceClockRate;
    uint_fast64_t systemToDeviceMultiplier;
    uint_fast64_t deviceToSystemMultiplier;
    int deviceToSystemRatio = 0;
    unsigned int systemTicksPassed = 0;
    unsigned int deviceTicksPassed = 0;

    ZEMUX_FORCE_INLINE unsigned int convertSystemToDevice(unsigned int systemTicks) {
        return (static_cast<uint_fast64_t>(systemTicks) * systemToDeviceMultiplier + FP_MATH_CEIL_ADDENT) >> FP_MATH_SHIFT;
    }

    ZEMUX_FORCE_INLINE unsigned int convertDeviceToSystem(unsigned int deviceTicks) {
        return (static_cast<uint_fast64_t>(deviceTicks) * deviceToSystemMultiplier + FP_MATH_CEIL_ADDENT) >> FP_MATH_SHIFT;
    }

    void update();
};

}

#endif
