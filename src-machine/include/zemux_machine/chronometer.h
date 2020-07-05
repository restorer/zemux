#ifndef ZEMUX_MACHINE__CHRONOMETER
#define ZEMUX_MACHINE__CHRONOMETER

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
#include <algorithm>
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>

namespace zemux {

class Chronometer final : private NonCopyable {
public:

    Chronometer(unsigned int systemClockRate, unsigned int deviceClockRate);

    [[nodiscard]] ZEMUX_FORCE_INLINE int getClockRatio() const {
        return clockRatio;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int getSystemClockRate() const {
        return systemClockRate;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int getDeviceClockRate() const {
        return deviceClockRate;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int getSystemTicksPassed() const {
        return systemTicksPassed;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int getDeviceTicksPassed() const {
        return deviceTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int systemForwardToDelta(unsigned int systemTicks) {
        unsigned int prevDeviceTicksPassed = deviceTicksPassed;
        return systemForwardTo(systemTicks) - prevDeviceTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int systemAdvanceBy(unsigned int systemTicksDelta) {
        return systemForwardTo(systemTicksPassed + systemTicksDelta);
    }

    ZEMUX_FORCE_INLINE unsigned int systemAdvanceByDelta(unsigned int systemTicksDelta) {
        unsigned int prevDeviceTicksPassed = deviceTicksPassed;
        return systemAdvanceBy(systemTicksDelta) - prevDeviceTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int deviceForwardToDelta(unsigned int deviceTicks) {
        unsigned int prevSystemTicksPassed = systemTicksPassed;
        return deviceForwardTo(deviceTicks) - prevSystemTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int deviceAdvanceBy(unsigned int deviceTicksDelta) {
        return deviceForwardTo(deviceTicksPassed + deviceTicksDelta);
    }

    ZEMUX_FORCE_INLINE unsigned int deviceAdvanceByDelta(unsigned int deviceTicksDelta) {
        unsigned int prevSystemTicksPassed = systemTicksPassed;
        return deviceAdvanceBy(deviceTicksDelta) - prevSystemTicksPassed;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int systemToDeviceCeil(unsigned int systemTicks) const {
        return (static_cast<uint_fast64_t>(systemTicks) * systemToDeviceMultiplier + FP_MATH_CEIL_ADDENT)
                >> FP_MATH_SHIFT;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int deviceToSystemCeil(unsigned int deviceTicks) const {
        return (static_cast<uint_fast64_t>(deviceTicks) * deviceToSystemMultiplier + FP_MATH_CEIL_ADDENT)
                >> FP_MATH_SHIFT;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int systemToDeviceLoops(unsigned int systemTicksDelta) const {
        return std::max(1u, systemToDeviceCeil(systemTicksDelta));
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int deviceToSystemLoops(unsigned int deviceTicksDelta) const {
        return std::max(1u, deviceToSystemCeil(deviceTicksDelta));
    }

    void setClockRatioFixedSystem(int ratio);
    void setClockRatioFixedDevice(int ratio);
    void setSystemClockRateFixedSystem(unsigned int rate);
    void setSystemClockRateFixedDevice(unsigned int rate);
    void setDeviceClockRateFixedSystem(unsigned int rate);
    void setDeviceClockRateFixedDevice(unsigned int rate);
    unsigned int systemForwardTo(unsigned int systemTicks);
    unsigned int deviceForwardTo(unsigned int deviceTicks);
    void systemConsume(unsigned int systemTicks);
    void deviceConsume(unsigned int deviceTicks);
    void reset();

private:

    static constexpr unsigned int FP_MATH_SHIFT = 24;

    static constexpr uint_fast64_t FP_MATH_CEIL_ADDENT = (static_cast<uint_fast64_t>(1) << FP_MATH_SHIFT) -
            static_cast<uint_fast64_t>(1);

    unsigned int systemClockRate;
    unsigned int deviceClockRate;
    int clockRatio = 0;
    unsigned int systemTicksPassed = 0;
    unsigned int deviceTicksPassed = 0;
    uint_fast64_t systemToDeviceMultiplier;
    uint_fast64_t deviceToSystemMultiplier;

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int systemToDeviceFloor(unsigned int systemTicks) const {
        return (static_cast<uint_fast64_t>(systemTicks) * systemToDeviceMultiplier + systemTicks) >> FP_MATH_SHIFT;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int deviceToSystemFloor(unsigned int deviceTicks) const {
        return (static_cast<uint_fast64_t>(deviceTicks) * deviceToSystemMultiplier + deviceTicks) >> FP_MATH_SHIFT;
    }

    void update();
};

}

#endif
