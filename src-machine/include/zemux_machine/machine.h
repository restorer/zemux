#ifndef ZEMUX_MACHINE__GUEST_MACHINE
#define ZEMUX_MACHINE__GUEST_MACHINE

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

#include <memory>
#include <map>
#include <zemux_core/non_copyable.h>
#include <zemux_core/chronometer.h>
#include <zemux_integrated/z80_chip.h>
#include "bus.h"
#include "event.h"
#include "devices/device.h"
#include "video/video_surface.h"
#include "sound/sound_desk.h"

namespace zemux {

class Machine final : public BusOwner, private NonCopyable {
public:

    ChronometerNarrow cpuChronometer { 1, 1 };
    Bus bus;
    Z80Chip cpu;

    VideoSurface videoSurface;
    SoundDesk soundDesk;

    Machine();
    ~Machine() = default;

    void renderFrame();

    void onBusReconfigure() override;
    void onBusReset() override;

private:

    std::map<int, std::unique_ptr<Device>> deviceMap;
    std::map<int, EventListener*> eventListenerMap;
    std::vector<Device*> attachedDevices;

    // Pentagon:
    uint32_t ulaLineTotalTicks = 224;
    uint32_t ulaHBlankTicks = 32;
    uint32_t ulaVBlankTicks = 16 * ulaLineTotalTicks;
    uint32_t ulaFrameTicks = ulaVBlankTicks + (64 + 192 + 48) * ulaLineTotalTicks;
    uint32_t ulaIntBeginTicks = 0;
    uint32_t ulaIntEndTicks = ulaIntBeginTicks + 32;

    // Scorpion:
    // uint32_t ulaLineTotalTicks = 224;
    // uint32_t ulaHBlankTicks = 40;
    // uint32_t ulaVBlankTicks = 16 * ulaLineTotalTicks;
    // uint32_t ulaFrameTicks = ulaVBlankTicks + (64 + 192 + 40) * ulaLineTotalTicks;
    // uint32_t ulaIntBeginTicks = ulaVBlankTicks;
    // uint32_t ulaIntEndTicks = ulaIntBeginTicks + 32;

    // Spectrum48:
    // uint32_t ulaLineTotalTicks = 224;
    // uint32_t ulaHBlankTicks = 48;
    // uint32_t ulaVBlankTicks = 16 * ulaLineTotalTicks;
    // uint32_t ulaFrameTicks = ulaVBlankTicks + (48 + 192 + 56) * ulaLineTotalTicks;
    // uint32_t ulaIntBeginTicks = 0;
    // uint32_t ulaIntEndTicks = ulaIntBeginTicks + 32;

    // Spectrum128:
    // uint32_t ulaLineTotalTicks = 224;
    // uint32_t ulaHBlankTicks = 48;
    // uint32_t ulaVBlankTicks = 15 * ulaLineTotalTicks;
    // uint32_t ulaFrameTicks = ulaVBlankTicks + (48 + 192 + 56) * ulaLineTotalTicks;
    // uint32_t ulaIntBeginTicks = 0;
    // uint32_t ulaIntEndTicks = ulaIntBeginTicks + 32;

    uint32_t ulaLineVisibleTicks = ulaLineTotalTicks - ulaHBlankTicks;

    void brazeDevice(Device::DeviceKind kind, std::unique_ptr<Device> device);
};

}

#endif
