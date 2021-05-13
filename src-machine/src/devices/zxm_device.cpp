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
#include <zemux_vendor/ym2203_chip.h>
#include <zemux_vendor/saa1099_chip.h>

namespace zemux {

ZxmDevice::ZxmDevice(Bus* bus, SoundDesk* soundDesk) : Device { bus },
        soundDesk { soundDesk },
        ym2203Chronometer { 1, Ym2203Chip::SAMPLING_RATE },
        saa1099Chronometer { 1, Saa1099Chip::SAMPLING_RATE },
        saa1099Resampler { &saa1099Chronometer } {

    ayResamplers.emplace_back(&ayChronometer);
    ayResamplers.emplace_back(&ayChronometer);

    ym2203Resamplers.emplace_back(&ym2203Chronometer);
    ym2203Resamplers.emplace_back(&ym2203Chronometer);

    ayChips.emplace_back(&ayResamplers[0], this, onAyDataIn, onAyDataOut);
    ayChips.emplace_back(&ayResamplers[1]);

    ym2203Chips[0] = new Ym2203Chip(&ym2203Resamplers[0]);
    ym2203Chips[1] = new Ym2203Chip(&ym2203Resamplers[1]);

    saa1099Chip = new Saa1099Chip(&saa1099Resampler);
}

ZxmDevice::~ZxmDevice() {
    delete ym2203Chips[0];
    delete ym2203Chips[1];

    delete saa1099Chip;
}

uint32_t ZxmDevice::getEventCategory() {
    return Event::CategoryZxm;
}

EventOutput ZxmDevice::onEvent(uint32_t type, EventInput input) {
    switch (type) {
        case EventSetConfiguration: {
            auto config = static_cast<Configuration*>(input.pointer);
            auto updateMask = config->updateMask;

            if (updateMask & Configuration::UpdateAyRate) {
                ayChronometer.setDstClockRateFixedSrc(config->ayRate);
            }

            if (updateMask & Configuration::UpdateAyChipType) {
                ayChips[0].setChipType(config->ayChipType);
                ayChips[1].setChipType(config->ayChipType);
            }

            if (updateMask & Configuration::UpdateAyVolumeType) {
                ayChips[0].setVolumeType(config->ayVolumeType);
                ayChips[1].setVolumeType(config->ayVolumeType);
            }

            if (updateMask & Configuration::UpdateAyPanType) {
                ayChips[0].setPanType(config->ayPanType);
                ayChips[1].setPanType(config->ayPanType);
            }

            if ((updateMask & Configuration::UpdateMode) && config->mode != mode) {
                mode = config->mode;
                bus->requestReconfigure();
            }

            return EventOutput { .isHandled = true };
        }

        case EventGetConfiguration: {
            auto config = static_cast<Configuration*>(input.pointer);

            config->updateMask = ~0;
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
            ? BusIorqRdElement { .callback = onIorqRd, .data = this }
            : prev;
}

BusIorqWrElement ZxmDevice::onConfigureIorqWr(BusIorqWrElement prev, int /* iorqRdLayer */, uint16_t port) {
    if (port == PORT_00FF) {
        return (mode == ModeZxm) ? BusIorqWrElement { .callback = onIorqWr00FF, .data = this } : prev;
    }

    if (port == PORT_01FF) {
        return (mode == ModeZxm) ? BusIorqWrElement { .callback = onIorqWr01FF, .data = this } : prev;
    }

    // 0xBFFD
    if ((port & 0b11000000'00000010) == 0b10000000'00000000) {
        return BusIorqWrElement { .callback = onIorqWrBFFD, .data = this };
    }

    // 0xFFFD
    if ((port & 0b11000000'00000010) == 0b11000000'00000000) {
        return BusIorqWrElement { .callback = onIorqWrFFFD, .data = this };
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
            saa1099Chip->step(saa1099Ticks);
            [[fallthrough]];

        case ModeTsFm:
            ym2203Chips[0]->step(ym2203Ticks);
            ym2203Chips[1]->step(ym2203Ticks);
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
            saa1099Chip->reset();
            [[fallthrough]];

        case ModeTsFm:
            ym2203Chips[0]->reset();
            ym2203Chips[1]->reset();
            [[fallthrough]];

        case ModeTs:
            ayChips[1].reset();
            [[fallthrough]];

        default:
            ayChips[0].reset();
    }
}

uint8_t ZxmDevice::onIorqRd(void* data, int /* iorqRdLayer */, uint16_t /* port */) {
    auto self = static_cast<ZxmDevice*>(data);
    auto mode = self->mode;

    if (mode >= ModeTsFm) {
        auto chipNum = self->getChipNum();

        if ((self->pseudoReg & (PSEUDO_BIT_STATUS | PSEUDO_BIT_FM)) == 0) {
            return self->ym2203Chips[chipNum]->readStatus();
        }

        return (self->selectedReg < SELECTED_REG_FM)
                ? self->ayChips[chipNum].read()
                : self->ym2203Chips[chipNum]->read();
    }

    return self->ayChips[mode >= ModeTs ? self->getChipNum() : 0].read();
}

void ZxmDevice::onIorqWr00FF(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value) {
    auto self = static_cast<ZxmDevice*>(data);
    auto& saa1099Chip = self->saa1099Chip;

    saa1099Chip->step(self->saa1099Chronometer.srcForwardToDelta(self->bus->getFrameTicksPassed()));
    saa1099Chip->writeData(value);
}

void ZxmDevice::onIorqWr01FF(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value) {
    auto self = static_cast<ZxmDevice*>(data);
    auto& saa1099Chip = self->saa1099Chip;

    saa1099Chip->step(self->saa1099Chronometer.srcForwardToDelta(self->bus->getFrameTicksPassed()));
    saa1099Chip->writeAddress(value);
}

void ZxmDevice::onIorqWrBFFD(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value) {
    auto self = static_cast<ZxmDevice*>(data);
    auto mode = self->mode;
    auto chipNum = (mode >= ModeTs ? self->getChipNum() : 0);

    if (mode >= ModeTsFm && self->selectedReg >= SELECTED_REG_FM) {
        auto& ym2203Chips = self->ym2203Chips;

        ym2203Chips[chipNum]->step(self->ym2203Chronometer.srcForwardToDelta(self->bus->getFrameTicksPassed()));
        ym2203Chips[chipNum]->write(value);
    } else {
        auto& ayChips = self->ayChips;

        ayChips[chipNum].step(self->ayChronometer.srcForwardToDelta(self->bus->getFrameTicksPassed()));
        ayChips[chipNum].write(value);
    }
}

void ZxmDevice::onIorqWrFFFD(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value) {
    auto self = static_cast<ZxmDevice*>(data);
    auto mode = self->mode;

    if (mode >= ModeTs && (value & PSEUDO_CHECK_MASK) == PSEUDO_CHECK_MASK) {
        self->pseudoReg = value & PSEUDO_VALUE_MASK;
        return;
    }

    self->selectedReg = value;
    auto chipNum = (mode >= ModeTs ? self->getChipNum() : 0);

    if (mode >= ModeTsFm && value >= SELECTED_REG_FM) {
        self->ym2203Chips[chipNum]->select(value);
    } else {
        self->ayChips[chipNum].select(value);
    }
}

uint8_t ZxmDevice::onAyDataIn(void* /* data */, uint8_t /* port */) {
    // Stub for AY-Mouse
    return 0;
}

void ZxmDevice::onAyDataOut(void* /* data */, uint8_t /* port */, uint8_t /* value */) {
    // Stub for AY-Mouse
}

}
