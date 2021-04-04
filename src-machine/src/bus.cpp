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
 */

#include "bus.h"

namespace zemux {

Bus::Bus(Z80Chip* cpu, ChronometerNarrow* cpuChronometer) : cpu { cpu }, cpuChronometer { cpuChronometer } {
    for (int i = 0; i < LAYERS_MREQ_RD; ++i) {
        mreqRdMapLayers[i].reset(new BusMreqRdElement[ELEMENTS_MREQ_RD]);
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

    onReset();
}

void Bus::onReset() {
    mreqRdLayer = 0;
    mreqWrLayer = 0;
    iorqRdLayer = 0;
    iorqWrLayer = 0;

    mreqRdMap = mreqRdMapLayers[0].get();
    mreqWrMap = mreqWrMapLayers[0].get();
    iorqRdMap = iorqRdMapLayers[0].get();
    iorqWrMap = iorqWrMapLayers[0].get();
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

uint32_t Bus::getFrameTicksPassed() {
    return cpuChronometer->getSrcTicksPassed() + cpuChronometer->dstToSrcCeil(cpu->getTstate());
}

}
