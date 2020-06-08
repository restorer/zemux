#ifndef ZEMUX__AY__AY_CHIP
#define ZEMUX__AY__AY_CHIP

/*
 * MIT License (http://www.opensource.org/licenses/mit-license.php)
 *
 * Copyright (c) 2020, Viachaslau Tratsiak (aka restorer)
 *
 * Based on YM-2149F emulator for Unreal Speccy project
 * created under public domain license by SMT, jan.2006
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
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/loudspeaker.h>

namespace zemux {

class AyChipDataCallback {
public:

    virtual uint8_t onAyDataIn(uint8_t port) = 0;
    virtual void onAyDataOut(uint8_t port, uint8_t value) = 0;

protected:

    constexpr AyChipDataCallback() = default;
    ~AyChipDataCallback() = default;
};

enum AyChipType {
    TypeAy,
    TypeYm
};

enum AyChipVolume {
    VolumeAy = 0,
    VolumeYm = 1
};

enum AyChipPan {
    PanMono = 0,
    PanABC = 1,
    PanACB = 2,
    PanBAC = 3,
    PanBCA = 4,
    PanCAB = 5,
    PanCBA = 6
};

enum AyChipReg {
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

ZEMUX_FORCE_INLINE constexpr unsigned int ayChipExternalToDeviceRate(unsigned int externalClockRate) {
    return externalClockRate >> 3;
}

class AyChip final : private NonCopyable {
public:

    static constexpr unsigned int DEFAULT_RATE = ayChipExternalToDeviceRate(1774400);

    AyChip(Loudspeaker* loudspeaker,
            AyChipDataCallback* dataCb = nullptr,
            AyChipType chipKind = TypeYm,
            AyChipVolume volumeType = VolumeYm,
            AyChipPan panType = PanACB);

    ZEMUX_FORCE_INLINE AyChipType getChipType() {
        return chipType;
    }

    ZEMUX_FORCE_INLINE AyChipVolume getVolumeType() {
        return volumeType;
    }

    ZEMUX_FORCE_INLINE AyChipPan getPanType() {
        return panType;
    }

    void setChipType(AyChipType type);
    void setVolumeType(AyChipVolume type);
    void setPanType(AyChipPan type);

    void select(uint8_t reg);
    void write(uint8_t value);
    uint8_t read();
    void reset();
    void step(unsigned int ticks);

private:

    Loudspeaker* loudspeaker;
    AyChipDataCallback* dataCb;
    AyChipType chipType;
    AyChipVolume volumeType;
    AyChipPan panType;

    std::pair<uint32_t, uint32_t> volumes[3][32];
    uint8_t regs[0x10] = { 0 };
    uint8_t selectedReg = 0;

    unsigned int toneAPeriod = 0;
    unsigned int toneBPeriod = 0;
    unsigned int toneCPeriod = 0;
    unsigned int noisePeriod = 0;

    unsigned int toneAMute = 0;
    unsigned int toneBMute = 0;
    unsigned int toneCMute = 0;
    unsigned int noiseAMute = 0;
    unsigned int noiseBMute = 0;
    unsigned int noiseCMute = 0;
    bool isPortModeOut[2] = { false };

    unsigned int toneAAmp = 0;
    unsigned int envAMask = 0;
    unsigned int toneBAmp = 0;
    unsigned int envBMask = 0;
    unsigned int toneCAmp = 0;
    unsigned int envCMask = 0;
    unsigned int envPeriod = 0;
    unsigned int envShape = 0;

    unsigned int toneATick = 0;
    unsigned int toneACurrent = 0;
    unsigned int toneBTick = 0;
    unsigned int toneBCurrent = 0;
    unsigned int toneCTick = 0;
    unsigned int toneCCurrent = 0;
    unsigned int noiseTick = 0;
    unsigned int noiseValue = 0;
    unsigned int noiseCurrent = 0;
    unsigned int envTick = 0;
    int envDelta = 0;
    unsigned int envCurrent = 0;

    void updateVolumes();
};

}

#endif
