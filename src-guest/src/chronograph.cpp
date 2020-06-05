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

#include "chronograph.h"

namespace zemux {

Chronograph::Chronograph(
    unsigned int systemClockRate,
    unsigned int deviceClockRate
) : systemClockRate { systemClockRate }, deviceClockRate { deviceClockRate } {
    update();
}

void Chronograph::setClockRatio(int ratio) {
    clockRatio = ratio;
    update();
}

void Chronograph::setSystemClockRate(unsigned int rate) {
    systemClockRate = rate;
    update();
}

void Chronograph::setDeviceClockRate(unsigned int rate) {
    deviceClockRate = rate;
    update();
}

unsigned int Chronograph::systemForwardTo(unsigned int systemTicks) {
    if (systemTicks > systemTicksPassed) {
        systemTicksPassed = systemTicks;
        deviceTicksPassed = std::max(deviceTicksPassed, convertSystemToDevice(systemTicks));
    }

    return deviceTicksPassed;
}

unsigned int Chronograph::deviceForwardTo(unsigned int deviceTicks) {
    if (deviceTicks > deviceTicksPassed) {
        deviceTicksPassed = deviceTicks;
        systemTicksPassed = std::max(systemTicksPassed, convertDeviceToSystem(deviceTicks));
    }

    return systemTicksPassed;
}

void Chronograph::systemConsume(unsigned int systemTicks) {
    if (systemTicks > systemTicksPassed) {
        systemTicks = systemTicksPassed;
    }

    systemTicksPassed -= systemTicks;
    deviceTicksPassed -= convertSystemToDevice(systemTicks);
}

void Chronograph::deviceConsume(unsigned int deviceTicks) {
    if (deviceTicks > deviceTicksPassed) {
        deviceTicksPassed = deviceTicksPassed;
    }

    deviceTicksPassed -= deviceTicks;
    systemTicksPassed -= convertDeviceToSystem(deviceTicks);
}

void Chronograph::update() {
    uint_fast64_t systemActualRate;
    uint_fast64_t deviceActualRate;

    if (clockRatio < -1) {
        systemActualRate = systemClockRate * (-clockRatio);
        deviceActualRate = deviceClockRate;
    } else if (clockRatio > 1) {
        systemActualRate = systemClockRate;
        deviceActualRate = deviceClockRate * clockRatio;
    } else {
        systemActualRate = systemClockRate;
        deviceActualRate = deviceClockRate;
    }

    systemToDeviceMultiplier = (deviceActualRate << FP_MATH_SHIFT) / systemActualRate;
    deviceToSystemMultiplier = (systemActualRate << FP_MATH_SHIFT) / deviceActualRate;
}

}
