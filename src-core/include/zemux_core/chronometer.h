#ifndef ZEMUX_CORE__CHRONOMETER
#define ZEMUX_CORE__CHRONOMETER

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

#include <cstdint>
#include <algorithm>
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>

namespace zemux {

class Chronometer final : private NonCopyable {
public:

    Chronometer(unsigned int srcClockRate, unsigned int dstClockRate);

    [[nodiscard]] ZEMUX_FORCE_INLINE int getClockRatio() const {
        return clockRatio;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int getSrcClockRate() const {
        return srcClockRate;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int getDstClockRate() const {
        return dstClockRate;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int getSrcTicksPassed() const {
        return srcTicksPassed;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int getDstTicksPassed() const {
        return dstTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int srcForwardToDelta(unsigned int srcTicks) {
        unsigned int prevDstTicksPassed = dstTicksPassed;
        return srcForwardTo(srcTicks) - prevDstTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int srcAdvanceBy(unsigned int srcTicksDelta) {
        return srcForwardTo(srcTicksPassed + srcTicksDelta);
    }

    ZEMUX_FORCE_INLINE unsigned int srcAdvanceByDelta(unsigned int srcTicksDelta) {
        unsigned int prevDstTicksPassed = dstTicksPassed;
        return srcAdvanceBy(srcTicksDelta) - prevDstTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int dstForwardToDelta(unsigned int dstTicks) {
        unsigned int prevSrcTicksPassed = srcTicksPassed;
        return dstForwardTo(dstTicks) - prevSrcTicksPassed;
    }

    ZEMUX_FORCE_INLINE unsigned int dstAdvanceBy(unsigned int dstTicksDelta) {
        return dstForwardTo(dstTicksPassed + dstTicksDelta);
    }

    ZEMUX_FORCE_INLINE unsigned int dstAdvanceByDelta(unsigned int dstTicksDelta) {
        unsigned int prevSrcTicksPassed = srcTicksPassed;
        return dstAdvanceBy(dstTicksDelta) - prevSrcTicksPassed;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int srcToDstCeil(unsigned int srcTicks) const {
        return (static_cast<uint_fast64_t>(srcTicks) * srcToDstMultiplier + FP_MATH_CEIL_ADDENT) >> FP_MATH_SHIFT;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int dstToSrcCeil(unsigned int dstTicks) const {
        return (static_cast<uint_fast64_t>(dstTicks) * dstToSrcMultiplier + FP_MATH_CEIL_ADDENT) >> FP_MATH_SHIFT;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int srcToDstLoops(unsigned int srcTicksDelta) const {
        return std::max(1u, srcToDstCeil(srcTicksDelta));
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int dstToSrcLoops(unsigned int dstTicksDelta) const {
        return std::max(1u, dstToSrcCeil(dstTicksDelta));
    }

    void setClockRatioFixedSrc(int ratio);
    void setClockRatioFixedDst(int ratio);
    void setSrcClockRateFixedSrc(unsigned int srcRate);
    void setSrcClockRateFixedDst(unsigned int srcRate);
    void setDstClockRateFixedSrc(unsigned int dstRate);
    void setDstClockRateFixedDst(unsigned int dstRate);
    unsigned int setSrcTicksPassed(unsigned int srcTicks);
    unsigned int setDstTicksPassed(unsigned int dstTicks);
    unsigned int srcForwardTo(unsigned int srcTicks);
    unsigned int dstForwardTo(unsigned int dstTicks);
    void srcConsume(unsigned int srcTicks);
    void dstConsume(unsigned int dstTicks);
    void reset();

private:

    static constexpr unsigned int FP_MATH_SHIFT = 24;

    static constexpr uint_fast64_t FP_MATH_CEIL_ADDENT = (static_cast<uint_fast64_t>(1) << FP_MATH_SHIFT) -
            static_cast<uint_fast64_t>(1);

    unsigned int srcClockRate;
    unsigned int dstClockRate;
    int clockRatio = 0;
    unsigned int srcTicksPassed = 0;
    unsigned int dstTicksPassed = 0;
    uint_fast64_t srcToDstMultiplier;
    uint_fast64_t dstToSrcMultiplier;

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int srcToDstFloor(unsigned int srcTicks) const {
        return (static_cast<uint_fast64_t>(srcTicks) * srcToDstMultiplier + srcTicks) >> FP_MATH_SHIFT;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int dstToSrcFloor(unsigned int dstTicks) const {
        return (static_cast<uint_fast64_t>(dstTicks) * dstToSrcMultiplier + dstTicks) >> FP_MATH_SHIFT;
    }

    void update();
};

}

#endif
