#ifndef ZEMUX_CHIPS__AY_CHIP
#define ZEMUX_CHIPS__AY_CHIP

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

#include <cstdint>
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/sound.h>

namespace zemux {

class AyChipCallback {
public:

    virtual uint8_t onAyDataIn(uint8_t port) = 0;
    virtual void onAyDataOut(uint8_t port, uint8_t value) = 0;

protected:

    constexpr AyChipCallback() = default;
    virtual ~AyChipCallback() = default;
};

ZEMUX_FORCE_INLINE constexpr uint32_t ayChipExternalToDeviceRate(uint32_t externalClockRate) {
    return externalClockRate >> 3;
}

class AyChip final : private NonCopyable {
public:

    enum ChipType {
        TypeAy,
        TypeYm
    };

    enum VolumeType {
        VolumeAy = 0,
        VolumeYm = 1
    };

    enum PanType {
        PanMono = 0,
        PanABC = 1,
        PanACB = 2,
        PanBAC = 3,
        PanBCA = 4,
        PanCAB = 5,
        PanCBA = 6
    };

    enum RegType {
        RegAPeriodFine = 0,
        RegAPeriodCoarse = 1,
        RegBPeriodFine = 2,
        RegBPeriodCoarse = 3,
        RegCPeriodFine = 4,
        RegCPeriodCoarse = 5,
        RegNoisePeriod = 6,
        RegControl = 7,
        RegAAmp = 8,
        RegBAmp = 9,
        RegCAmp = 10,
        RegEnvPeriodFine = 11,
        RegEnvPeriodCoarse = 12,
        RegEnvShape = 13,
        RegPortA = 14,
        RegPortB = 15
    };

    static constexpr uint32_t DEFAULT_RATE = ayChipExternalToDeviceRate(1774400);

    explicit AyChip(SoundSink* soundSink,
            AyChipCallback* cb = nullptr,
            ChipType chipType = TypeYm,
            VolumeType volumeType = VolumeYm,
            PanType panType = PanACB);

    [[nodiscard]] ZEMUX_FORCE_INLINE ChipType getChipType() const {
        return chipType;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE VolumeType getVolumeType() const {
        return volumeType;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE PanType getPanType() const {
        return panType;
    }

    void setChipType(ChipType type);
    void setVolumeType(VolumeType type);
    void setPanType(PanType type);

    void select(uint8_t reg);
    void write(uint8_t value);
    uint8_t read();
    void reset();
    void step(uint32_t ticks);

private:

    SoundSink* soundSink;
    AyChipCallback* cb;
    ChipType chipType;
    VolumeType volumeType;
    PanType panType;

    std::pair<uint32_t, uint32_t> volumes[3][32];
    uint8_t regs[0x10] = { 0 };
    uint8_t selectedReg = 0;

    uint_fast16_t toneAPeriod = 0;
    uint_fast16_t toneBPeriod = 0;
    uint_fast16_t toneCPeriod = 0;
    uint_fast16_t noisePeriod = 0;

    uint_fast8_t toneAMute = 0;
    uint_fast8_t toneBMute = 0;
    uint_fast8_t toneCMute = 0;
    uint_fast8_t noiseAMute = 0;
    uint_fast8_t noiseBMute = 0;
    uint_fast8_t noiseCMute = 0;
    bool isPortModeOut[2] = { false };

    uint_fast8_t toneAAmp = 0;
    uint_fast8_t envAMask = 0;
    uint_fast8_t toneBAmp = 0;
    uint_fast8_t envBMask = 0;
    uint_fast8_t toneCAmp = 0;
    uint_fast8_t envCMask = 0;
    uint_fast16_t envPeriod = 0;
    uint_fast16_t envShape = 0;

    uint_fast16_t toneATick = 0;
    uint_fast8_t toneACurrent = 0;
    uint_fast16_t toneBTick = 0;
    uint_fast8_t toneBCurrent = 0;
    uint_fast16_t toneCTick = 0;
    uint_fast8_t toneCCurrent = 0;
    uint_fast16_t noiseTick = 0;
    uint_fast16_t noiseValue = 0;
    uint_fast8_t noiseCurrent = 0;
    uint_fast16_t envTick = 0;
    int_fast8_t envDelta = 0;
    uint_fast8_t envCurrent = 0;

    void updateVolumes();
};

}

#endif
