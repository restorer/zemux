#ifndef ZEMUX_CORE__CHRONOMETER
#define ZEMUX_CORE__CHRONOMETER

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

#include <cstdint>
#include <algorithm>
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>

namespace zemux {

template<typename TNarrow, typename TWide, unsigned int FpMathShift>
class AbstractChronometer : private NonCopyable {
public:

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

    AbstractChronometer(
            TNarrow srcClockRate,
            TNarrow dstClockRate) : srcClockRate { srcClockRate }, dstClockRate { dstClockRate } {

        update();
    }

#pragma clang diagnostic pop

    [[nodiscard]] ZEMUX_FORCE_INLINE int getClockRatio() const {
        return clockRatio;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE TNarrow getSrcClockRate() const {
        return srcClockRate;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE TNarrow getDstClockRate() const {
        return dstClockRate;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE TNarrow getSrcTicksPassed() const {
        return srcTicksPassed;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE TNarrow getDstTicksPassed() const {
        return dstTicksPassed;
    }

    ZEMUX_FORCE_INLINE TNarrow srcForwardToDelta(TNarrow srcTicks) {
        auto prevDstTicksPassed = dstTicksPassed;
        return srcForwardTo(srcTicks) - prevDstTicksPassed;
    }

    ZEMUX_FORCE_INLINE TNarrow srcAdvanceBy(TNarrow srcTicksDelta) {
        return srcForwardTo(srcTicksPassed + srcTicksDelta);
    }

    ZEMUX_FORCE_INLINE TNarrow srcAdvanceByDelta(TNarrow srcTicksDelta) {
        auto prevDstTicksPassed = dstTicksPassed;
        return srcAdvanceBy(srcTicksDelta) - prevDstTicksPassed;
    }

    ZEMUX_FORCE_INLINE TNarrow dstForwardToDelta(TNarrow dstTicks) {
        auto prevSrcTicksPassed = srcTicksPassed;
        return dstForwardTo(dstTicks) - prevSrcTicksPassed;
    }

    ZEMUX_FORCE_INLINE TNarrow dstAdvanceBy(TNarrow dstTicksDelta) {
        return dstForwardTo(dstTicksPassed + dstTicksDelta);
    }

    ZEMUX_FORCE_INLINE TNarrow dstAdvanceByDelta(TNarrow dstTicksDelta) {
        auto prevSrcTicksPassed = srcTicksPassed;
        return dstAdvanceBy(dstTicksDelta) - prevSrcTicksPassed;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE TNarrow srcToDstCeil(TNarrow srcTicks) const {
        return (static_cast<TWide>(srcTicks) * srcToDstMultiplier + FpMathCeilAddent) >> FpMathShift;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE TNarrow dstToSrcCeil(TNarrow dstTicks) const {
        return (static_cast<TWide>(dstTicks) * dstToSrcMultiplier + FpMathCeilAddent) >> FpMathShift;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE TNarrow srcToDstLoops(TNarrow srcTicksDelta) const {
        return std::max(static_cast<TNarrow>(1), srcToDstCeil(srcTicksDelta));
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE TNarrow dstToSrcLoops(TNarrow dstTicksDelta) const {
        return std::max(static_cast<TNarrow>(1), dstToSrcCeil(dstTicksDelta));
    }

    void setClockRatioFixedSrc(int ratio) {
        clockRatio = ratio;
        update();
        dstTicksPassed = srcToDstCeil(srcTicksPassed);
    }

    void setClockRatioFixedDst(int ratio) {
        clockRatio = ratio;
        update();
        srcTicksPassed = dstToSrcCeil(dstTicksPassed);
    }

    void setSrcClockRateFixedSrc(TNarrow srcRate) {
        srcClockRate = srcRate;
        update();
        dstTicksPassed = srcToDstCeil(srcTicksPassed);
    }

    void setSrcClockRateFixedDst(TNarrow srcRate) {
        srcClockRate = srcRate;
        update();
        srcTicksPassed = dstToSrcCeil(dstTicksPassed);
    }

    void setDstClockRateFixedSrc(TNarrow dstRate) {
        dstClockRate = dstRate;
        update();
        dstTicksPassed = srcToDstCeil(srcTicksPassed);
    }

    void setDstClockRateFixedDst(TNarrow dstRate) {
        dstClockRate = dstRate;
        update();
        srcTicksPassed = dstToSrcCeil(dstTicksPassed);
    }

    TNarrow setSrcTicksPassed(TNarrow srcTicks) {
        srcTicksPassed = srcTicks;
        dstTicksPassed = srcToDstCeil(srcTicks);
        return dstTicksPassed;
    }

    TNarrow setDstTicksPassed(TNarrow dstTicks) {
        dstTicksPassed = dstTicks;
        srcTicksPassed = dstToSrcCeil(dstTicks);
        return srcTicksPassed;
    }

    TNarrow srcForwardTo(TNarrow srcTicks) {
        if (srcTicks > srcTicksPassed) {
            srcTicksPassed = srcTicks;
            dstTicksPassed = std::max(dstTicksPassed, srcToDstCeil(srcTicks));
        }

        return dstTicksPassed;
    }

    TNarrow dstForwardTo(TNarrow dstTicks) {
        if (dstTicks > dstTicksPassed) {
            dstTicksPassed = dstTicks;
            srcTicksPassed = std::max(srcTicksPassed, dstToSrcCeil(dstTicks));
        }

        return srcTicksPassed;
    }

    void srcConsume(TNarrow srcTicks) {
        auto dstTicksConsumed = std::min(
                dstTicksPassed,
                srcToDstFloor(std::min(srcTicks, dstToSrcFloor(dstTicksPassed))));

        srcTicksPassed -= dstToSrcCeil(dstTicksConsumed);
        dstTicksPassed -= dstTicksConsumed;
    }

    void dstConsume(TNarrow dstTicks) {
        auto srcTicksConsumed = std::min(
                srcTicksPassed,
                dstToSrcFloor(std::min(dstTicks, srcToDstFloor(srcTicksPassed))));

        srcTicksPassed -= srcTicksConsumed;
        dstTicksPassed -= srcToDstCeil(srcTicksConsumed);
    }

    void reset() {
        srcTicksPassed = 0;
        dstTicksPassed = 0;
    }

    void reconfigure(TNarrow srcRate, TNarrow dstRate) {
        srcClockRate = srcRate;
        dstClockRate = dstRate;

        srcTicksPassed = 0;
        dstTicksPassed = 0;

        update();
    }

private:

    static constexpr TWide FpMathCeilAddent = (static_cast<TWide>(1) << FpMathShift) - static_cast<TWide>(1);

    TNarrow srcClockRate;
    TNarrow dstClockRate;
    int clockRatio = 0;
    TNarrow srcTicksPassed = 0;
    TNarrow dstTicksPassed = 0;
    TWide srcToDstMultiplier;
    TWide dstToSrcMultiplier;

    [[nodiscard]] ZEMUX_FORCE_INLINE TNarrow srcToDstFloor(TNarrow srcTicks) const {
        auto srcTicksWide = static_cast<TWide>(srcTicks);
        return (srcTicksWide * srcToDstMultiplier + srcTicksWide) >> FpMathShift;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE TNarrow dstToSrcFloor(TNarrow dstTicks) const {
        auto dstTicksWide = static_cast<TWide>(dstTicks);
        return (dstTicksWide * dstToSrcMultiplier + dstTicksWide) >> FpMathShift;
    }

    void update() {
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

        srcToDstMultiplier = (dstActualRate << FpMathShift) / srcActualRate;
        dstToSrcMultiplier = (srcActualRate << FpMathShift) / dstActualRate;
    }
};

// Precise, but can handle only narrow values (up to 32 bits).
class ChronometerNarrow final : public AbstractChronometer<uint_fast32_t, uint_fast64_t, 32> {
public:

    ChronometerNarrow(
            uint_fast32_t srcClockRate,
            uint_fast32_t dstClockRate) : AbstractChronometer { srcClockRate, dstClockRate } {
    }
};

// Can handle wider values (up to 48 bits), but has lower calculations accuracy.
class ChronometerWide final : public AbstractChronometer<uint_fast64_t, uint_fast64_t, 16> {
public:

    ChronometerWide(
            uint_fast64_t srcClockRate,
            uint_fast64_t dstClockRate) : AbstractChronometer { srcClockRate, dstClockRate } {
    }
};

}

#endif
