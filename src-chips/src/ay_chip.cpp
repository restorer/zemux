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
 *
 *
 * Based on YM-2149F emulator for Unreal Speccy project
 * created under public domain license by SMT, jan.2006
 */

#include <utility>
#include <algorithm>
#include "ay_chip.h"

static const uint16_t VOLUME_TABLES[][32] = {
        { // AY
                0x0000, 0x0000, 0x0340, 0x0340, 0x04C0, 0x04C0, 0x06F2, 0x06F2,
                0x0A44, 0x0A44, 0x0F13, 0x0F13, 0x1510, 0x1510, 0x227E, 0x227E,
                0x289F, 0x289F, 0x414E, 0x414E, 0x5B21, 0x5B21, 0x7258, 0x7258,
                0x905E, 0x905E, 0xB550, 0xB550, 0xD7A0, 0xD7A0, 0xFFFF, 0xFFFF },
        { // YM
                0x0000, 0x0000, 0x00EF, 0x01D0, 0x0290, 0x032A, 0x03EE, 0x04D2,
                0x0611, 0x0782, 0x0912, 0x0A36, 0x0C31, 0x0EB6, 0x1130, 0x13A0,
                0x1751, 0x1BF5, 0x20E2, 0x2594, 0x2CA1, 0x357F, 0x3E45, 0x475E,
                0x5502, 0x6620, 0x7730, 0x8844, 0xA1D2, 0xC102, 0xE0A2, 0xFFFF }};

static constexpr double PAN_FULL_VOLUME = 3.0;
static constexpr auto PAN_MONO = std::make_pair(1.0, 1.0);
static constexpr auto PAN_LEFT = std::make_pair(1.7, 0.2); // (1.0, 0.1)
static constexpr auto PAN_MIDDLE = std::make_pair(1.1, 1.1); // (0.66, 0.66)
static constexpr auto PAN_RIGHT = std::make_pair(0.2, 1.7); // (0.1, 1.0)

static const std::pair<double, double> PAN_TABLES[][3] = {
        { PAN_MONO, PAN_MONO, PAN_MONO }, // Mono
        { PAN_LEFT, PAN_MIDDLE, PAN_RIGHT }, // ABC
        { PAN_LEFT, PAN_RIGHT, PAN_MIDDLE }, // ACB
        { PAN_MIDDLE, PAN_LEFT, PAN_RIGHT }, // BAC
        { PAN_RIGHT, PAN_LEFT, PAN_MIDDLE }, // BCA
        { PAN_MIDDLE, PAN_RIGHT, PAN_LEFT }, // CAB
        { PAN_RIGHT, PAN_MIDDLE, PAN_LEFT } // CBA
};

namespace zemux {

AyChip::AyChip(
        Loudspeaker* loudspeaker,
        AyChipCallback* cb,
        ChipType chipType,
        VolumeType volumeType,
        PanType panType
) : loudspeaker { loudspeaker },
        cb { cb },
        chipType { chipType },
        volumeType { volumeType },
        panType { panType } {

    updateVolumes();
    reset();
}

void AyChip::setChipType(ChipType type) {
    chipType = type;
}

void AyChip::setVolumeType(VolumeType type) {
    volumeType = type;
    updateVolumes();
}

void AyChip::setPanType(PanType type) {
    panType = type;
    updateVolumes();
}

void AyChip::select(uint8_t reg) {
    if (chipType == TypeAy) {
        selectedReg = reg & 0x0F;
    } else {
        selectedReg = reg;
    }
}

void AyChip::write(uint8_t value) {
    if (selectedReg > 0x0F) {
        return;
    }

    if (selectedReg == RegAPeriodCoarse ||
            selectedReg == RegBPeriodCoarse ||
            selectedReg == RegCPeriodCoarse ||
            selectedReg == RegEnvShape) {

        value &= 0x0F;
    } else if (selectedReg == RegNoisePeriod ||
            selectedReg == RegAAmp ||
            selectedReg == RegBAmp ||
            selectedReg == RegCAmp) {

        value &= 0x1F;
    }

    if (regs[selectedReg] == value && selectedReg != RegEnvShape) {
        return;
    }

    regs[selectedReg] = value;

    switch (selectedReg) {
        case RegAPeriodFine:
            [[fallthrough]];

        case RegAPeriodCoarse:
            toneAPeriod = regs[RegAPeriodFine] | (static_cast<unsigned int>(regs[RegAPeriodCoarse]) << 8);
            break;

        case RegBPeriodFine:
            [[fallthrough]];

        case RegBPeriodCoarse:
            toneBPeriod = regs[RegBPeriodFine] | (static_cast<unsigned int>(regs[RegBPeriodCoarse]) << 8);
            break;

        case RegCPeriodFine:
            [[fallthrough]];

        case RegCPeriodCoarse:
            toneCPeriod = regs[RegCPeriodFine] | (static_cast<unsigned int>(regs[RegCPeriodCoarse]) << 8);
            break;

        case RegNoisePeriod:
            noisePeriod = static_cast<unsigned int>(value) << 1;
            break;

        case RegControl:
            toneAMute = 0 - (static_cast<unsigned int>(value >> 0) & 1);
            toneBMute = 0 - (static_cast<unsigned int>(value >> 1) & 1);
            toneCMute = 0 - (static_cast<unsigned int>(value >> 2) & 1);
            noiseAMute = 0 - (static_cast<unsigned int>(value >> 3) & 1);
            noiseBMute = 0 - (static_cast<unsigned int>(value >> 4) & 1);
            noiseCMute = 0 - (static_cast<unsigned int>(value >> 5) & 1);
            isPortModeOut[0] = (value >> 6) & 1;
            isPortModeOut[1] = (value >> 6) & 1;
            break;

        case RegAAmp:
            envAMask = 0 - static_cast<unsigned int>((value & 0x10) >> 4);
            toneAAmp = ((static_cast<unsigned int>(value & 0x0F) << 1) + 1) & ~envAMask;
            break;

        case RegBAmp:
            envBMask = 0 - static_cast<unsigned int>((value & 0x10) >> 4);
            toneBAmp = ((static_cast<unsigned int>(value & 0x0F) << 1) + 1) & ~envBMask;
            break;

        case RegCAmp:
            envCMask = 0 - static_cast<unsigned int>((value & 0x10) >> 4);
            toneCAmp = ((static_cast<unsigned int>(value & 0x0F) << 1) + 1) & ~envCMask;
            break;

        case RegEnvPeriodFine:
            [[fallthrough]];

        case RegEnvPeriodCoarse:
            envPeriod = regs[RegEnvPeriodFine] | (static_cast<unsigned int>(regs[RegEnvPeriodCoarse]) << 8);
            break;

        case RegEnvShape:
            envTick = 0;

            if (value & 4) {
                // Attack
                envCurrent = 0;
                envDelta = 1;
            } else {
                // Decay
                envCurrent = 31;
                envDelta = -1;
            }

            break;

        case RegPortA:
            [[fallthrough]];

        case RegPortB:
            if (cb != nullptr) {
                uint8_t port = selectedReg - RegPortA;

                if (isPortModeOut[port]) {
                    cb->onAyDataOut(port, value);
                }
            }

            break;
    }
}

uint8_t AyChip::read() {
    if (selectedReg > 0x0F) {
        return 0xFF;
    }

    if (selectedReg >= RegPortA && cb != nullptr) {
        uint8_t port = selectedReg - RegPortA;

        if (!isPortModeOut[port]) {
            return cb->onAyDataIn(0);
        }
    }

    return regs[selectedReg];
}

void AyChip::reset() {
    for (uint8_t i = 0; i < 0x10; ++i) {
        selectedReg = i;
        write(0);
    }

    selectedReg = 0;
}

void AyChip::step(unsigned int ticks) {
    while (ticks--) {
        if (++toneATick >= toneAPeriod) {
            toneATick = 0;
            toneACurrent ^= ~0;
        }

        if (++toneBTick >= toneBPeriod) {
            toneBTick = 0;
            toneBCurrent ^= ~0;
        }

        if (++toneCTick >= toneCPeriod) {
            toneCTick = 0;
            toneCCurrent ^= ~0;
        }

        if (++noiseTick >= noisePeriod) {
            noiseTick = 0;
            noiseValue = ((noiseValue << 1) + 1) ^ (((noiseValue >> 16) ^ (noiseValue >> 13)) & 1);
            noiseCurrent = 0 - ((noiseValue >> 16) & 1);
        }

        if (++envTick >= envPeriod) {
            envTick = 0;
            envCurrent += envDelta;

            if (envCurrent & ~31) {
                if (envShape & 0b1000001011111111) {
                    envCurrent = 0;
                    envDelta = 0;
                } else if (envShape & 0b0001000100000000) {
                    envCurrent &= 31;
                } else if (envShape & 0b0100010000000000) {
                    envDelta -= envDelta;
                    envCurrent += envDelta;
                } else {
                    // 0b0010100000000000
                    envCurrent = 31;
                    envDelta = 0;
                }
            }
        }

        unsigned int amp = ((envAMask & envCurrent) | toneAAmp) &
                (toneACurrent | toneAMute) &
                (noiseCurrent | noiseAMute);

        uint32_t left = volumes[0][amp].first;
        uint32_t right = volumes[0][amp].second;

        amp = ((envBMask & envCurrent) | toneBAmp) & (toneBCurrent | toneBMute) & (noiseCurrent | noiseBMute);
        left += volumes[1][amp].first;
        right += volumes[1][amp].second;

        amp = ((envCMask & envCurrent) | toneCAmp) & (toneCCurrent | toneCMute) & (noiseCurrent | noiseCMute);
        left += volumes[2][amp].first;
        right += volumes[2][amp].second;

        loudspeaker->onLoudspeakerStep(std::min(0xFFFFu, left), std::min(0xFFFFu, right));
    }
}

void AyChip::updateVolumes() {
    const uint16_t* volumeTable = VOLUME_TABLES[volumeType];
    const std::pair<double, double>* panTable = PAN_TABLES[panType];

    for (int chan = 0; chan < 3; ++chan) {
        for (int amp = 0; amp < 32; ++amp) {
            volumes[chan][amp].first = static_cast<double>(volumeTable[amp]) *
                    panTable[chan].first /
                    PAN_FULL_VOLUME;

            volumes[chan][amp].second = static_cast<double>(volumeTable[amp]) *
                    panTable[chan].second /
                    PAN_FULL_VOLUME;
        }
    }
}

}
