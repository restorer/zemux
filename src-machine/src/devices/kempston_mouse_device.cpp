// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

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

#include "devices/kempston_mouse_device.h"
#include "host.h"

namespace zemux {

KempstonMouseDevice::KempstonMouseDevice(Bus* bus) : Device { bus } {
}

uint32_t KempstonMouseDevice::getEventCategory() {
    return Event::CategoryKempstonMouse;
}

EventOutput KempstonMouseDevice::onEvent(uint32_t type, EventInput input) {
    switch (type) {
        case EventWheelScrolled: {
            int wheelDirection = wheelDirectionMultiplier * input.value;

            if (wheelDirection < 0) {
                ++wheelCounter;
            } else if (wheelDirection > 0) {
                --wheelCounter;
            }

            return EventOutput { .isHandled = true };
        }

        case EventSetConfiguration: {
            auto config = static_cast<Configuration*>(input.pointer);
            auto updateMask = config->updateMask;

            if (updateMask & Configuration::UpdateIsWheelEnabled) {
                wheelCounterMask = config->isWheelEnabled ? WHEEL_COUNTER_MASK_ENABLED : WHEEL_COUNTER_MASK_DISABLED;
            }

            if (updateMask & Configuration::UpdateWheelDirection) {
                wheelDirectionMultiplier = (config->wheelDirection == DirectionReverse) ? -1 : 1;
            }

            if (updateMask & Configuration::UpdateIsLeftSwappedWithRight) {
                buttonMaskLeft = (config->isLeftSwappedWithRight ? BUTTON_MASK_RIGHT : BUTTON_MASK_LEFT);
                buttonMaskRight = (config->isLeftSwappedWithRight ? BUTTON_MASK_LEFT : BUTTON_MASK_RIGHT);
            }

            if (updateMask & Configuration::UpdateMiddleButtonMode) {
                switch (config->middleButtonMode) {
                    case ModeMiddle:
                    case ModeLeftAndRightAsMiddle:
                        buttonMaskMiddle = BUTTON_MASK_MIDDLE;
                        break;

                    case ModeMiddleAsLeftAndRight:
                        buttonMaskMiddle = BUTTON_MASK_LEFT & BUTTON_MASK_RIGHT;
                        break;

                    default:
                        buttonMaskMiddle = BUTTON_MASK_NONE;
                }

                hostButtonMaskSpecial = (config->middleButtonMode == ModeLeftAndRightAsMiddle)
                        ? (HostMouseState::BUTTON_BIT_LEFT | HostMouseState::BUTTON_BIT_RIGHT)
                        : 0;
            }

            if (updateMask & Configuration::UpdateMotionRatioX) {
                motionMultiplierX = static_cast<int64_t>(config->motionRatioX) + 1;
            }

            if (updateMask & Configuration::UpdateMotionRatioY) {
                motionMultiplierY = static_cast<int64_t>(config->motionRatioY) + 1;
            }

            return EventOutput { .isHandled = true };
        }

        case EventGetConfiguration: {
            auto config = static_cast<Configuration*>(input.pointer);

            config->updateMask = ~0;
            config->isWheelEnabled = (wheelCounterMask == WHEEL_COUNTER_MASK_ENABLED);
            config->wheelDirection = (wheelDirectionMultiplier < 0) ? DirectionReverse : DirectionNormal;
            config->isLeftSwappedWithRight = (buttonMaskLeft == BUTTON_MASK_RIGHT);

            if (hostButtonMaskSpecial) {
                config->middleButtonMode = ModeLeftAndRightAsMiddle;
            } else {
                switch (buttonMaskMiddle) {
                    case BUTTON_MASK_MIDDLE:
                        config->middleButtonMode = ModeMiddle;
                        break;

                    case (BUTTON_MASK_LEFT & BUTTON_MASK_RIGHT):
                        config->middleButtonMode = ModeMiddleAsLeftAndRight;
                        break;

                    default:
                        config->middleButtonMode = ModeNone;
                }
            }

            config->motionRatioX = motionMultiplierX - 1;
            config->motionRatioY = motionMultiplierY - 1;

            return EventOutput { .isHandled = true };
        }

        default:
            return EventOutput {};
    }
}

BusIorqRdElement KempstonMouseDevice::onConfigureIorqRd(BusIorqRdElement prev, int /* iorqRdLayer */, uint16_t port) {
    if (port == PORT_FBDF) {
        return BusIorqRdElement { .callback = onIorqRdFBDF, .data = this };
    }

    if (port == PORT_FFDF) {
        return BusIorqRdElement { .callback = onIorqRdFFDF, .data = this };
    }

    if (port == PORT_FADF) {
        return BusIorqRdElement { .callback = onIorqRdFADF, .data = this };
    }

    return prev;
}

void KempstonMouseDevice::update() {
    HostMouseState hostState;
    portFADF = (wheelCounter << 4) | wheelCounterMask;

    if (bus->hostEmitter != nullptr
            && bus->hostEmitter->emitEvent(Host::EventGetMouseState, EventInput { .pointer = &hostState }).isHandled) {

        int32_t deltaX = (static_cast<int64_t>(hostState.motionX - lastMotionX) * motionMultiplierX) >> 8;
        int32_t deltaY = (static_cast<int64_t>(hostState.motionY - lastMotionY) * motionMultiplierY) >> 8;

        if (deltaX != 0) {
            portFBDF += deltaX;
            lastMotionX = hostState.motionX;
        }

        if (deltaY != 0) {
            portFFDF += deltaY;
            lastMotionY = hostState.motionY;
        }

        if (hostState.buttons & HostMouseState::BUTTON_BIT_LEFT) {
            portFADF &= buttonMaskLeft;
        }

        if (hostState.buttons & HostMouseState::BUTTON_BIT_RIGHT) {
            portFADF &= buttonMaskRight;
        }

        if (hostState.buttons & HostMouseState::BUTTON_BIT_MIDDLE) {
            portFADF &= buttonMaskMiddle;
        }

        if (hostState.buttons & hostButtonMaskSpecial) {
            portFADF &= BUTTON_MASK_MIDDLE;
        }
    }
}

uint8_t KempstonMouseDevice::onIorqRdFBDF(void* data, int /* iorqRdLayer */, uint16_t /* port */) {
    auto self = static_cast<KempstonMouseDevice*>(data);

    self->update();
    return self->portFBDF;
}

uint8_t KempstonMouseDevice::onIorqRdFFDF(void* data, int /* iorqRdLayer */, uint16_t /* port */) {
    auto self = static_cast<KempstonMouseDevice*>(data);

    self->update();
    return self->portFFDF;
}

uint8_t KempstonMouseDevice::onIorqRdFADF(void* data, int /* iorqRdLayer */, uint16_t /* port */) {
    auto self = static_cast<KempstonMouseDevice*>(data);

    self->update();
    return self->portFADF;
}

}
