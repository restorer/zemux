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
 */

#include "chronometer.h"

namespace zemux {

Chronometer::Chronometer(
        unsigned int systemClockRate,
        unsigned int deviceClockRate
) : systemClockRate { systemClockRate }, deviceClockRate { deviceClockRate } {
    update();
}

void Chronometer::setClockRatioFixedSystem(int ratio) {
    clockRatio = ratio;
    update();
    deviceTicksPassed = systemToDeviceCeil(systemTicksPassed);
}

void Chronometer::setClockRatioFixedDevice(int ratio) {
    clockRatio = ratio;
    update();
    systemTicksPassed = deviceToSystemCeil(deviceTicksPassed);
}

void Chronometer::setSystemClockRateFixedSystem(unsigned int rate) {
    systemClockRate = rate;
    update();
    deviceTicksPassed = systemToDeviceCeil(systemTicksPassed);
}

void Chronometer::setSystemClockRateFixedDevice(unsigned int rate) {
    systemClockRate = rate;
    update();
    systemTicksPassed = deviceToSystemCeil(deviceTicksPassed);
}

void Chronometer::setDeviceClockRateFixedSystem(unsigned int rate) {
    deviceClockRate = rate;
    update();
    deviceTicksPassed = systemToDeviceCeil(systemTicksPassed);
}

void Chronometer::setDeviceClockRateFixedDevice(unsigned int rate) {
    deviceClockRate = rate;
    update();
    systemTicksPassed = deviceToSystemCeil(deviceTicksPassed);
}

unsigned int Chronometer::systemForwardTo(unsigned int systemTicks) {
    if (systemTicks > systemTicksPassed) {
        systemTicksPassed = systemTicks;
        deviceTicksPassed = std::max(deviceTicksPassed, systemToDeviceCeil(systemTicks));
    }

    return deviceTicksPassed;
}

unsigned int Chronometer::deviceForwardTo(unsigned int deviceTicks) {
    if (deviceTicks > deviceTicksPassed) {
        deviceTicksPassed = deviceTicks;
        systemTicksPassed = std::max(systemTicksPassed, deviceToSystemCeil(deviceTicks));
    }

    return systemTicksPassed;
}

void Chronometer::systemConsume(unsigned int systemTicks) {
    unsigned int deviceTicksConsumed = std::min(
            deviceTicksPassed,
            systemToDeviceFloor(
                    std::min(
                            systemTicks,
                            deviceToSystemFloor(deviceTicksPassed))));

    systemTicksPassed -= deviceToSystemCeil(deviceTicksConsumed);
    deviceTicksPassed -= deviceTicksConsumed;
}

void Chronometer::deviceConsume(unsigned int deviceTicks) {
    unsigned int systemTicksConsumed = std::min(
            systemTicksPassed,
            deviceToSystemFloor(
                    std::min(
                            deviceTicks,
                            systemToDeviceFloor(systemTicksPassed))));

    systemTicksPassed -= systemTicksConsumed;
    deviceTicksPassed -= systemToDeviceCeil(systemTicksConsumed);
}

void Chronometer::reset() {
    systemTicksPassed = 0;
    deviceTicksPassed = 0;
}

void Chronometer::update() {
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
