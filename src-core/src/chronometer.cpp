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

#include "chronometer.h"

namespace zemux {

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

Chronometer::Chronometer(
        unsigned int srcClockRate,
        unsigned int dstClockRate
) : srcClockRate { srcClockRate }, dstClockRate { dstClockRate } {
    update();
}

#pragma clang diagnostic pop

void Chronometer::setClockRatioFixedSrc(int ratio) {
    clockRatio = ratio;
    update();
    dstTicksPassed = srcToDstCeil(srcTicksPassed);
}

void Chronometer::setClockRatioFixedDst(int ratio) {
    clockRatio = ratio;
    update();
    srcTicksPassed = dstToSrcCeil(dstTicksPassed);
}

void Chronometer::setSrcClockRateFixedSrc(unsigned int srcRate) {
    srcClockRate = srcRate;
    update();
    dstTicksPassed = srcToDstCeil(srcTicksPassed);
}

void Chronometer::setSrcClockRateFixedDst(unsigned int srcRate) {
    srcClockRate = srcRate;
    update();
    srcTicksPassed = dstToSrcCeil(dstTicksPassed);
}

void Chronometer::setDstClockRateFixedSrc(unsigned int dstRate) {
    dstClockRate = dstRate;
    update();
    dstTicksPassed = srcToDstCeil(srcTicksPassed);
}

void Chronometer::setDstClockRateFixedDst(unsigned int dstRate) {
    dstClockRate = dstRate;
    update();
    srcTicksPassed = dstToSrcCeil(dstTicksPassed);
}

unsigned int Chronometer::setSrcTicksPassed(unsigned int srcTicks) {
    srcTicksPassed = srcTicks;
    dstTicksPassed = srcToDstCeil(srcTicks);

    return dstTicksPassed;
}

unsigned int Chronometer::setDstTicksPassed(unsigned int dstTicks) {
    dstTicksPassed = dstTicks;
    srcTicksPassed = dstToSrcCeil(dstTicks);

    return srcTicksPassed;
}

unsigned int Chronometer::srcForwardTo(unsigned int srcTicks) {
    if (srcTicks > srcTicksPassed) {
        srcTicksPassed = srcTicks;
        dstTicksPassed = std::max(dstTicksPassed, srcToDstCeil(srcTicks));
    }

    return dstTicksPassed;
}

unsigned int Chronometer::dstForwardTo(unsigned int dstTicks) {
    if (dstTicks > dstTicksPassed) {
        dstTicksPassed = dstTicks;
        srcTicksPassed = std::max(srcTicksPassed, dstToSrcCeil(dstTicks));
    }

    return srcTicksPassed;
}

void Chronometer::srcConsume(unsigned int srcTicks) {
    unsigned int dstTicksConsumed = std::min(
            dstTicksPassed,
            srcToDstFloor(std::min(srcTicks, dstToSrcFloor(dstTicksPassed))));

    srcTicksPassed -= dstToSrcCeil(dstTicksConsumed);
    dstTicksPassed -= dstTicksConsumed;
}

void Chronometer::dstConsume(unsigned int dstTicks) {
    unsigned int srcTicksConsumed = std::min(
            srcTicksPassed,
            dstToSrcFloor(std::min(dstTicks, srcToDstFloor(srcTicksPassed))));

    srcTicksPassed -= srcTicksConsumed;
    dstTicksPassed -= srcToDstCeil(srcTicksConsumed);
}

void Chronometer::reset() {
    srcTicksPassed = 0;
    dstTicksPassed = 0;
}

void Chronometer::update() {
    if (!srcClockRate || !dstClockRate) {
        srcToDstMultiplier = 0;
        dstToSrcMultiplier = 0;
        return;
    }

    uint_fast64_t srcActualRate;
    uint_fast64_t dstActualRate;

    if (clockRatio < -1) {
        srcActualRate = srcClockRate * (-clockRatio);
        dstActualRate = dstClockRate;
    } else if (clockRatio > 1) {
        srcActualRate = srcClockRate;
        dstActualRate = dstClockRate * clockRatio;
    } else {
        srcActualRate = srcClockRate;
        dstActualRate = dstClockRate;
    }

    srcToDstMultiplier = (dstActualRate << FP_MATH_SHIFT) / srcActualRate;
    dstToSrcMultiplier = (srcActualRate << FP_MATH_SHIFT) / dstActualRate;
}

}
