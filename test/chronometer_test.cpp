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

#include <boost/test/unit_test.hpp>
#include <zemux_core/chronometer.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

BOOST_AUTO_TEST_CASE(Chronometer) {
    zemux::Chronometer chrono(1, 3);

    BOOST_TEST_MESSAGE("Initial");

    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 0);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 0);

    BOOST_TEST_MESSAGE("Source");

    BOOST_REQUIRE(chrono.srcForwardTo(1) == 3);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 3);

    BOOST_REQUIRE(chrono.srcForwardTo(2) == 6);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 6);

    BOOST_REQUIRE(chrono.srcForwardTo(1) == 6);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 6);

    BOOST_REQUIRE(chrono.srcAdvanceBy(1) == 9);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 9);

    BOOST_REQUIRE(chrono.srcAdvanceBy(2) == 15);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 5);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 15);

    BOOST_REQUIRE(chrono.srcAdvanceByDelta(1) == 3);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 6);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 18);

    BOOST_REQUIRE(chrono.srcAdvanceByDelta(2) == 6);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 8);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 24);

    BOOST_REQUIRE(chrono.srcForwardToDelta(9) == 3);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 9);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 27);

    chrono.srcConsume(9);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 0);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 0);

    BOOST_TEST_MESSAGE("Destination");

    BOOST_REQUIRE(chrono.dstForwardTo(1) == 1);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 1);

    BOOST_REQUIRE(chrono.dstForwardTo(2) == 1);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 2);

    BOOST_REQUIRE(chrono.dstForwardToDelta(3) == 0);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 3);

    BOOST_REQUIRE(chrono.dstForwardTo(4) == 2);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 4);

    BOOST_REQUIRE(chrono.dstForwardTo(1) == 2);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 4);

    BOOST_REQUIRE(chrono.dstAdvanceBy(1) == 2);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 5);

    BOOST_REQUIRE(chrono.dstAdvanceBy(2) == 3);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 7);

    BOOST_REQUIRE(chrono.dstAdvanceByDelta(1) == 0);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 8);

    BOOST_REQUIRE(chrono.dstAdvanceByDelta(1) == 0);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 9);

    BOOST_REQUIRE(chrono.dstAdvanceByDelta(1) == 1);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 4);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 10);

    chrono.dstConsume(10);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 1);

    chrono.srcConsume(1);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 1);

    BOOST_TEST_MESSAGE("Reset");

    chrono.reset();
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 0);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 0);

    BOOST_TEST_MESSAGE("Clock ratio");

    BOOST_REQUIRE(chrono.dstAdvanceBy(1) == 1);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 1);

    chrono.setClockRatioFixedSrc(-3);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 1);

    BOOST_REQUIRE(chrono.srcAdvanceBy(1) == 2);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 2);

    chrono.setClockRatioFixedSrc(0);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 6);

    BOOST_REQUIRE(chrono.srcAdvanceBy(1) == 9);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 9);

    chrono.setClockRatioFixedSrc(3);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 27);

    BOOST_REQUIRE(chrono.srcAdvanceBy(1) == 36);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 4);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 36);

    BOOST_TEST_MESSAGE("Various ZX-Spectrum timings");

    chrono.reset();
    chrono.setClockRatioFixedSrc(0);
    chrono.setSrcClockRateFixedSrc(3584000);
    chrono.setDstClockRateFixedSrc(221800);

    BOOST_REQUIRE(chrono.srcAdvanceByDelta(128) == 8);
    BOOST_REQUIRE(chrono.srcAdvanceByDelta(256) == 16);
    BOOST_REQUIRE(chrono.getDstTicksPassed() == 24);
    BOOST_REQUIRE(chrono.srcForwardTo(3584000) == 221800);

    chrono.reset();
    chrono.setClockRatioFixedSrc(0);
    chrono.setSrcClockRateFixedSrc(44100);

    BOOST_REQUIRE(chrono.dstAdvanceByDelta(128) == 26);
    BOOST_REQUIRE(chrono.dstAdvanceByDelta(256) == 51);
    BOOST_REQUIRE(chrono.getSrcTicksPassed() == 77);
    BOOST_REQUIRE(chrono.dstForwardTo(221800) == 44100);
}

#pragma clang diagnostic pop
