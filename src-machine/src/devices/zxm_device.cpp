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

#include "devices/zxm_device.h"

namespace zemux {

static uint8_t onZxmDeviceIorqRd(void* data, uint16_t /* port */) {
    return static_cast<ZxmDevice*>(data)->onIorqRd();
}

static void onZxmDeviceIorqWr00FF(void* data, uint16_t /* port */, uint8_t value) {
    static_cast<ZxmDevice*>(data)->onIorqWr00FF(value);
}

static void onZxmDeviceIorqWr01FF(void* data, uint16_t /* port */, uint8_t value) {
    static_cast<ZxmDevice*>(data)->onIorqWr01FF(value);
}

static void onZxmDeviceIorqWrBFFD(void* data, uint16_t /* port */, uint8_t value) {
    static_cast<ZxmDevice*>(data)->onIorqWrBFFD(value);
}

static void onZxmDeviceIorqWrFFFD(void* data, uint16_t /* port */, uint8_t value) {
    static_cast<ZxmDevice*>(data)->onIorqWrFFFD(value);
}

ZxmDevice::ZxmDevice(Bus* bus, SoundDesk* soundDesk) : Device { bus }, soundDesk { soundDesk } {
    ayResamplers.emplace_back(&ayChronometer);
    ayResamplers.emplace_back(&ayChronometer);

    ym2203Resamplers.emplace_back(&ym2203Chronometer);
    ym2203Resamplers.emplace_back(&ym2203Chronometer);

    ayChips.emplace_back(&ayResamplers[0], this);
    ayChips.emplace_back(&ayResamplers[1]);

    ym2203Chips.emplace_back(&ym2203Resamplers[0]);
    ym2203Chips.emplace_back(&ym2203Resamplers[1]);
}

uint32_t ZxmDevice::getEventCategory() {
    return Event::CategoryZxm;
}

EventOutput ZxmDevice::onEvent(uint32_t type, EventInput input) {
    switch (type) {
        case EventSetConfiguration: {
            auto config = static_cast<Configuration*>(input.pointer);
            ayChronometer.setDstClockRateFixedSrc(config->ayRate);

            for (auto i = 0; i < TSFM_CHIPS_COUNT; ++i) {
                ayChips[i].setChipType(config->ayChipType);
                ayChips[i].setVolumeType(config->ayVolumeType);
                ayChips[i].setPanType(config->ayPanType);
            }

            if (config->mode != mode) {
                mode = config->mode;
                bus->requestReconfigure();
            }

            return EventOutput { .isHandled = true };
        }

        case EventGetConfiguration: {
            auto config = static_cast<Configuration*>(input.pointer);

            config->mode = mode;
            config->ayRate = ayChronometer.getDstClockRate();
            config->ayChipType = ayChips[0].getChipType();
            config->ayVolumeType = ayChips[0].getVolumeType();
            config->ayPanType = ayChips[0].getPanType();

            return EventOutput { .isHandled = true };
        }

        default:
            return EventOutput {};
    }
}

void ZxmDevice::onAttach() {
    Device::onAttach();
    attachedMode = mode;

    switch (mode) {
        case ModeZxm:
            soundDesk->attachCable(&saa1099Resampler);
            [[fallthrough]];

        case ModeTsFm:
            soundDesk->attachCable(&ym2203Resamplers[0]);
            soundDesk->attachCable(&ym2203Resamplers[1]);
            [[fallthrough]];

        case ModeTs:
            soundDesk->attachCable(&ayResamplers[1]);
            [[fallthrough]];

        default:
            soundDesk->attachCable(&ayResamplers[0]);
    }
}

void ZxmDevice::onDetach() {
    switch (attachedMode) {
        case ModeZxm:
            soundDesk->detachCable(&saa1099Resampler);
            [[fallthrough]];

        case ModeTsFm:
            soundDesk->detachCable(&ym2203Resamplers[0]);
            soundDesk->detachCable(&ym2203Resamplers[1]);
            [[fallthrough]];

        case ModeTs:
            soundDesk->detachCable(&ayResamplers[1]);
            [[fallthrough]];

        default:
            soundDesk->detachCable(&ayResamplers[0]);
    }

    Device::onDetach();
}

BusIorqRdElement ZxmDevice::onConfigureIorqRd(BusIorqRdElement prev, int /* iorqRdLayer */, uint16_t port) {
    // 0xFFFD
    return ((port & 0b11000000'00000010) == 0b11000000'00000000)
            ? BusIorqRdElement { .callback = onZxmDeviceIorqRd, .data = this }
            : prev;
}

BusIorqWrElement ZxmDevice::onConfigureIorqWr(BusIorqWrElement prev, int /* iorqRdLayer */, uint16_t port) {
    if (port == PORT_00FF) {
        return (mode == ModeZxm) ? BusIorqWrElement { .callback = onZxmDeviceIorqWr00FF, .data = this } : prev;
    }

    if (port == PORT_01FF) {
        return (mode == ModeZxm) ? BusIorqWrElement { .callback = onZxmDeviceIorqWr01FF, .data = this } : prev;
    }

    // 0xBFFD
    if ((port & 0b11000000'00000010) == 0b10000000'00000000) {
        return BusIorqWrElement { .callback = onZxmDeviceIorqWrBFFD, .data = this };
    }

    // 0xFFFD
    if ((port & 0b11000000'00000010) == 0b11000000'00000000) {
        return BusIorqWrElement { .callback = onZxmDeviceIorqWrFFFD, .data = this };
    }

    return prev;
}

void ZxmDevice::onConfigureTimings(uint32_t ticksPerFrame) {
    ayChronometer.setSrcClockRateFixedDst(ticksPerFrame);
    ym2203Chronometer.setSrcClockRateFixedDst(ticksPerFrame);
    saa1099Chronometer.setSrcClockRateFixedDst(ticksPerFrame);
}

void ZxmDevice::onFrameFinished(uint32_t ticks) {
    auto ayTicks = ayChronometer.srcForwardToDelta(ticks);
    auto ym2203Ticks = ym2203Chronometer.srcForwardToDelta(ticks);
    auto saa1099Ticks = saa1099Chronometer.srcForwardToDelta(ticks);

    switch (mode) {
        case ModeZxm:
            saa1099Chip.step(saa1099Ticks);
            [[fallthrough]];

        case ModeTsFm:
            ym2203Chips[0].step(ym2203Ticks);
            ym2203Chips[1].step(ym2203Ticks);
            [[fallthrough]];

        case ModeTs:
            ayChips[1].step(ayTicks);
            [[fallthrough]];

        default:
            ayChips[0].step(ayTicks);
    }

    ayChronometer.srcConsume(ticks);
    ym2203Chronometer.srcConsume(ticks);
    saa1099Chronometer.srcConsume(ticks);
}

void ZxmDevice::onReset() {
    selectedReg = 0;
    pseudoReg = PSEUDO_VALUE_MASK;

    switch (mode) {
        case ModeZxm:
            saa1099Chip.reset();
            [[fallthrough]];

        case ModeTsFm:
            ym2203Chips[0].reset();
            ym2203Chips[1].reset();
            [[fallthrough]];

        case ModeTs:
            ayChips[1].reset();
            [[fallthrough]];

        default:
            ayChips[0].reset();
    }
}

uint8_t ZxmDevice::onAyDataIn(uint8_t /* port */) {
    // Stub for AY-Mouse
    return 0;
}

void ZxmDevice::onAyDataOut(uint8_t /* port */, uint8_t /* value */) {
    // Stub for AY-Mouse
}

uint8_t ZxmDevice::onIorqRd() {
    if (mode >= ModeTsFm) {
        auto chipNum = getChipNum();

        if ((pseudoReg & (PSEUDO_BIT_STATUS | PSEUDO_BIT_FM)) == 0) {
            return ym2203Chips[chipNum].readStatus();
        }

        return (selectedReg < SELECTED_REG_FM)
                ? ayChips[chipNum].read()
                : ym2203Chips[chipNum].read();
    }

    return ayChips[mode >= ModeTs ? getChipNum() : 0].read();
}

void ZxmDevice::onIorqWr00FF(uint8_t value) {
    saa1099Chip.step(saa1099Chronometer.srcForwardToDelta(bus->getFrameTicksPassed()));
    saa1099Chip.writeData(value);
}

void ZxmDevice::onIorqWr01FF(uint8_t value) {
    saa1099Chip.step(saa1099Chronometer.srcForwardToDelta(bus->getFrameTicksPassed()));
    saa1099Chip.writeAddress(value);
}

void ZxmDevice::onIorqWrBFFD(uint8_t value) {
    auto chipNum = (mode >= ModeTs ? getChipNum() : 0);

    if (mode >= ModeTsFm && selectedReg >= SELECTED_REG_FM) {
        ym2203Chips[chipNum].step(ym2203Chronometer.srcForwardToDelta(bus->getFrameTicksPassed()));
        ym2203Chips[chipNum].write(value);
    } else {
        ayChips[chipNum].step(ayChronometer.srcForwardToDelta(bus->getFrameTicksPassed()));
        ayChips[chipNum].write(value);
    }
}

void ZxmDevice::onIorqWrFFFD(uint8_t value) {
    if (mode >= ModeTs && (value & PSEUDO_CHECK_MASK) == PSEUDO_CHECK_MASK) {
        pseudoReg = value & PSEUDO_VALUE_MASK;
        return;
    }

    selectedReg = value;
    auto chipNum = (mode >= ModeTs ? getChipNum() : 0);

    if (mode >= ModeTsFm && value >= SELECTED_REG_FM) {
        ym2203Chips[chipNum].select(value);
    } else {
        ayChips[chipNum].select(value);
    }
}

}
