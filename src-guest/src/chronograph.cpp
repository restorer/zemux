#include "chronograph.h"

Chronograph::Chronograph(
    unsigned int systemClockRate,
    unsigned int deviceClockRate
) : systemClockRate { systemClockRate }, deviceClockRate { deviceClockRate } {
    update();
}

void Chronograph::setDeviceToSystemRatio(int ratio) {
    deviceToSystemRatio = ratio;
    update();
}

void Chronograph::setSystemClockRate(unsigned int clockRate) {
    systemClockRate = clockRate;
    update();
}

void Chronograph::setDeviceClockRate(unsigned int clockRate) {
    deviceClockRate = clockRate;
    update();
}

void Chronograph::systemConsume(unsigned int systemTicks) {
    systemTicksPassed -= systemTicks;
    deviceTicksPassed -= convertSystemToDevice(systemTicks);
}

void Chronograph::deviceConsume(unsigned int deviceTicks) {
    deviceTicksPassed -= deviceTicks;
    systemTicksPassed -= convertDeviceToSystem(deviceTicks);
}

void Chronograph::update() {
    uint_fast64_t systemClockActual;
    uint_fast64_t deviceClockActual;

    if (deviceToSystemRatio < -1) {
        systemClockActual = systemClockRate * (-deviceToSystemRatio);
        deviceClockActual = deviceClockRate;
    } else if (deviceToSystemRatio > 1) {
        systemClockActual = systemClockRate;
        deviceClockActual = deviceClockRate * deviceToSystemRatio;
    } else {
        systemClockActual = systemClockRate;
        deviceClockActual = deviceClockRate;
    }

    systemToDeviceMultiplier = (deviceClockActual << FP_MATH_SHIFT) / systemClockActual;
    deviceToSystemMultiplier = (systemClockActual << FP_MATH_SHIFT) / deviceClockActual;
}
