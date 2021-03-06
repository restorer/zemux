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

#include "bus.h"
#include "devices/device.h"
#include <zemux_integrated/z80_chip.h>

namespace zemux {

Bus::Bus(BusOwner* owner, ChronometerNarrow* cpuChronometer) : owner { owner }, cpuChronometer { cpuChronometer } {
    for (int i = 0; i < LAYERS_MREQ_RD; ++i) {
        mreqRdMapLayers[i].reset(new BusMreqRdElement[ELEMENTS_MREQ_RD_FULL]);
    }

    for (int i = 0; i < LAYERS_MREQ_WR; ++i) {
        mreqWrMapLayers[i].reset(new BusMreqWrElement[ELEMENTS_MREQ_WR]);
    }

    for (int i = 0; i < LAYERS_IORQ_RD; ++i) {
        iorqRdMapLayers[i].reset(new BusIorqRdElement[ELEMENTS_IORQ_RD]);
    }

    for (int i = 0; i < LAYERS_IORQ_WR; ++i) {
        iorqWrMapLayers[i].reset(new BusIorqWrElement[ELEMENTS_IORQ_WR]);
    }

    onMachineReset();
}

uint32_t Bus::getFrameTicksPassed() {
    return cpuChronometer->getSrcTicksPassed() + cpuChronometer->dstToSrcCeil(cpu->getTstate());
}

void Bus::setCpuClockRatio(int ratio) {
    cpuChronometer->setClockRatioFixedSrc(ratio);
}

void Bus::requestReconfigure() {
    owner->onBusReconfigure();
}

void Bus::requestReset() {
    owner->onBusReset();
}

void Bus::toggleMreqRdOverlay(int mreqRdOverlay, bool isEnabled) {
    if (isEnabled) {
        mreqRdLayer |= mreqRdOverlay;
    } else {
        mreqRdLayer &= (~mreqRdOverlay);
    }

    mreqRdMap = mreqRdMapLayers[mreqRdLayer].get();
}

void Bus::toggleMreqWrOverlay(int mreqWrOverlay, bool isEnabled) {
    if (isEnabled) {
        mreqWrLayer |= mreqWrOverlay;
    } else {
        mreqWrLayer &= (~mreqWrOverlay);
    }

    mreqWrMap = mreqWrMapLayers[mreqWrLayer].get();
}

void Bus::toggleIorqRdOverlay(int iorqRdOverlay, bool isEnabled) {
    if (isEnabled) {
        iorqRdLayer |= iorqRdOverlay;
    } else {
        iorqRdLayer &= (~iorqRdOverlay);
    }

    iorqRdMap = iorqRdMapLayers[iorqRdOverlay].get();
}

void Bus::toggleIorqWrOverlay(int iorqWrOverlay, bool isEnabled) {
    if (isEnabled) {
        iorqWrLayer |= iorqWrOverlay;
    } else {
        iorqWrLayer &= (~iorqWrOverlay);
    }

    iorqWrMap = iorqWrMapLayers[iorqWrOverlay].get();
}

void Bus::onMachineReconfigure(std::vector<Device*>& devices) {
    for (int layer = 0; layer < LAYERS_MREQ_RD; ++layer) {
        auto layerMap = mreqRdMapLayers[layer].get();

        for (int index = 0; index < ELEMENTS_MREQ_RD_FULL; ++index) {
            layerMap[index] = BusMreqRdElement { .callback = &Bus::onFallbackMreqRd, .data = nullptr };
        }
    }

    for (int layer = 0; layer < LAYERS_MREQ_WR; ++layer) {
        auto layerMap = mreqWrMapLayers[layer].get();

        for (int index = 0; index < ELEMENTS_MREQ_WR; ++index) {
            layerMap[index] = BusMreqWrElement { .callback = &Bus::onFallbackMreqWr, .data = nullptr };
        }
    }

    for (int layer = 0; layer < LAYERS_IORQ_RD; ++layer) {
        auto layerMap = iorqRdMapLayers[layer].get();

        for (int index = 0; index < ELEMENTS_IORQ_RD; ++index) {
            layerMap[index] = BusIorqRdElement { .callback = &Bus::onFallbackIorqRd, .data = this };
        }
    }

    for (int layer = 0; layer < LAYERS_IORQ_WR; ++layer) {
        auto layerMap = iorqWrMapLayers[layer].get();

        for (int index = 0; index < ELEMENTS_IORQ_WR; ++index) {
            layerMap[index] = BusIorqWrElement { .callback = &Bus::onFallbackIorqWr, .data = nullptr };
        }
    }

    for (auto& device : devices) {
        for (int layer = 0; layer < LAYERS_MREQ_RD; ++layer) {
            auto layerMap = mreqRdMapLayers[layer].get();

            for (int index = 0; index < ELEMENTS_MREQ_RD_FULL; ++index) {
                layerMap[index] = device->onConfigureMreqRd(
                        layerMap[index],
                        layer,
                        index & Bus::ELEMENTS_MREQ_RD_BASE_MASK,
                        index >= Bus::ELEMENTS_MREQ_RD_BASE);
            }
        }

        for (int layer = 0; layer < LAYERS_MREQ_WR; ++layer) {
            auto layerMap = mreqWrMapLayers[layer].get();

            for (int index = 0; index < ELEMENTS_MREQ_WR; ++index) {
                layerMap[index] = device->onConfigureMreqWr(layerMap[index], layer, index);
            }
        }

        for (int layer = 0; layer < LAYERS_IORQ_RD; ++layer) {
            auto layerMap = iorqRdMapLayers[layer].get();

            for (int index = 0; index < ELEMENTS_IORQ_RD; ++index) {
                layerMap[index] = device->onConfigureIorqRd(layerMap[index], layer, index);
            }
        }

        for (int layer = 0; layer < LAYERS_IORQ_WR; ++layer) {
            auto layerMap = iorqWrMapLayers[layer].get();

            for (int index = 0; index < ELEMENTS_IORQ_WR; ++index) {
                layerMap[index] = device->onConfigureIorqWr(layerMap[index], layer, index);
            }
        }
    }
}

void Bus::onMachineReset() {
    mreqRdLayer = 0;
    mreqWrLayer = 0;
    iorqRdLayer = 0;
    iorqWrLayer = 0;

    mreqRdMap = mreqRdMapLayers[0].get();
    mreqWrMap = mreqWrMapLayers[0].get();
    iorqRdMap = iorqRdMapLayers[0].get();
    iorqWrMap = iorqWrMapLayers[0].get();
}

uint8_t Bus::onFallbackMreqRd(void* /* data */, int /* mreqRdLayer */, uint16_t /* address */, bool /* isM1 */) {
    return 0xFF;
}

void Bus::onFallbackMreqWr(void* /* data */, int /* mreqWrLayer */, uint16_t /* address */, uint8_t /* value */) {
}

uint8_t Bus::onFallbackIorqRd(void* /* data */, int /* iorqRdLayer */, uint16_t /* port */) {
    // TODO: port #FF
    return 0xFF;
}

void Bus::onFallbackIorqWr(void* /* data */, int /* iorqWrLayer */, uint16_t /* port */, uint8_t /* value */) {
}

uint8_t Bus::onCpuMreqRd(void* data, uint16_t address, bool isM1) {
    auto self = static_cast<Bus*>(data);
    auto elem = self->mreqRdMap[address + (isM1 ? ELEMENTS_MREQ_RD_BASE : 0)];
    return elem.callback(elem.data, self->mreqRdLayer, address, isM1);
}

void Bus::onCpuMreqWr(void* data, uint16_t address, uint8_t value) {
    auto self = static_cast<Bus*>(data);
    auto elem = self->mreqWrMap[address];
    elem.callback(elem.data, self->mreqWrLayer, address, value);
}

uint8_t Bus::onCpuIorqRd(void* data, uint16_t port) {
    auto self = static_cast<Bus*>(data);
    auto elem = self->iorqRdMap[port];
    return elem.callback(elem.data, self->iorqRdLayer, port);
}

void Bus::onCpuIorqWr(void* data, uint16_t port, uint8_t value) {
    auto self = static_cast<Bus*>(data);
    auto elem = self->iorqWrMap[port];
    elem.callback(elem.data, self->iorqWrLayer, port, value);
}

uint8_t Bus::onCpuIorqM1(void* /* data */) {
    // TODO: unstable data bus
    return 0xFF;
}

void Bus::onCpuPutAddress(void* /* data */, uint16_t /* address */, uint_fast32_t /* cycles */) {
    // TODO: slow memory
}

}
