#ifndef ZEMUX_MACHINE__ZXM_DEVICE
#define ZEMUX_MACHINE__ZXM_DEVICE

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

#include "bus.h"
#include "device.h"
#include "event.h"
#include "sound/sound_resampler.h"
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/container.h>
#include <zemux_core/sound.h>
#include <zemux_integrated/ay_chip.h>
#include <cstdint>
#include <memory>

namespace zemux {

class Ym2203Chip;
class Saa1099Chip;

uint8_t onZxmDeviceIorqRd(void* data, int /* iorqRdLayer */, uint16_t /* port */);
void onZxmDeviceIorqWr00FF(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
void onZxmDeviceIorqWr01FF(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
void onZxmDeviceIorqWrBFFD(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
void onZxmDeviceIorqWrFFFD(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);

class ZxmDevice final : public Device, public AyChipCallback, private NonCopyable {
public:

    enum Mode {
        ModeAy = 0,
        ModeTs = 1,
        ModeTsFm = 2,
        ModeZxm = 3,
    };

    enum EventType {
        EventSetConfiguration = Event::CategoryZxm | 1,
        EventGetConfiguration = Event::CategoryZxm | 2,
    };

    struct Configuration {
        Mode mode;
        uint32_t ayRate; // divided by 8
        AyChip::ChipType ayChipType;
        AyChip::VolumeType ayVolumeType;
        AyChip::PanType ayPanType;
    };

    static constexpr int TSFM_CHIPS_COUNT = 2;

    static constexpr uint32_t PORT_00FF = 0x00FF;
    static constexpr uint32_t PORT_01FF = 0x01FF;

    static constexpr uint8_t PSEUDO_CHECK_MASK = 0xF0;
    static constexpr uint8_t PSEUDO_VALUE_MASK = 0x0F;
    static constexpr uint8_t PSEUDO_BIT_CHIP_NUM = 1;
    static constexpr unsigned int PSEUDO_SHIFT_CHIP_NUM = 0;
    static constexpr uint8_t PSEUDO_BIT_STATUS = 2;
    static constexpr uint8_t PSEUDO_BIT_FM = 4;
    static constexpr uint8_t PSEUDO_BIT_SAA = 8;
    static constexpr uint8_t SELECTED_REG_FM = 0x10;

    ZxmDevice(Bus* bus, SoundDesk* soundDesk);
    virtual ~ZxmDevice() = default;

    uint32_t getEventCategory() override;
    EventOutput onEvent(uint32_t type, EventInput input) override;

    void onAttach() override;
    void onDetach() override;
    BusIorqRdElement onConfigureIorqRd(BusIorqRdElement prev, int /* iorqRdLayer */, uint16_t port) override;
    BusIorqWrElement onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) override;
    void onConfigureTimings(uint32_t ticksPerFrame) override;
    void onFrameFinished(uint32_t ticks) override;
    void onReset() override;

    uint8_t onAyDataIn(uint8_t /* port */) override;
    void onAyDataOut(uint8_t /* port */, uint8_t /* value */) override;

private:

    SoundDesk* soundDesk;

    Mode mode = ModeTs;
    Mode attachedMode;
    uint8_t selectedReg = 0;
    uint8_t pseudoReg = PSEUDO_VALUE_MASK;

    ChronometerNarrow ayChronometer { 1, AyChip::DEFAULT_RATE };
    ChronometerNarrow ym2203Chronometer;
    ChronometerNarrow saa1099Chronometer;

    Container<SoundResampler> ayResamplers { TSFM_CHIPS_COUNT };
    Container<SoundResampler> ym2203Resamplers { TSFM_CHIPS_COUNT };
    SoundResampler saa1099Resampler;

    Container<AyChip> ayChips { TSFM_CHIPS_COUNT };
    Container<Ym2203Chip> ym2203Chips { TSFM_CHIPS_COUNT };
    std::unique_ptr<Saa1099Chip> saa1099Chip;

    uint8_t onIorqRd();
    void onIorqWr00FF(uint8_t value);
    void onIorqWr01FF(uint8_t value);
    void onIorqWrBFFD(uint8_t value);
    void onIorqWrFFFD(uint8_t value);

    ZEMUX_FORCE_INLINE int getChipNum() {
        return ((~pseudoReg) & PSEUDO_BIT_CHIP_NUM) >> PSEUDO_SHIFT_CHIP_NUM;
    }

    friend uint8_t onZxmDeviceIorqRd(void* data, int /* iorqRdLayer */, uint16_t /* port */);
    friend void onZxmDeviceIorqWr00FF(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    friend void onZxmDeviceIorqWr01FF(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    friend void onZxmDeviceIorqWrBFFD(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
    friend void onZxmDeviceIorqWrFFFD(void* data, int /* iorqWrLayer */, uint16_t /* port */, uint8_t value);
};

}

#endif
