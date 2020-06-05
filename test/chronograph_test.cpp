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
#include <zemux_guest/chronograph.h>

BOOST_AUTO_TEST_CASE(Chronograph) {
    zemux::Chronograph chrono(1, 3);

    BOOST_TEST_MESSAGE("Initial");

    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 0U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 0U);

    BOOST_TEST_MESSAGE("System");

    BOOST_REQUIRE(chrono.systemForwardTo(1U) == 3U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 3U);

    BOOST_REQUIRE(chrono.systemForwardTo(2U) == 6U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 6U);

    BOOST_REQUIRE(chrono.systemForwardTo(1U) == 6U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 6U);

    BOOST_REQUIRE(chrono.systemAdvanceBy(1U) == 9U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 9U);

    BOOST_REQUIRE(chrono.systemAdvanceBy(2U) == 15U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 5U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 15U);

    BOOST_REQUIRE(chrono.systemAdvanceByDelta(1U) == 3U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 6U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 18U);

    BOOST_REQUIRE(chrono.systemAdvanceByDelta(2U) == 6U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 8U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 24U);

    BOOST_REQUIRE(chrono.systemForwardToDelta(9U) == 3U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 9U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 27U);

    chrono.systemConsume(9U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 0U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 0U);

    BOOST_TEST_MESSAGE("Device");

    BOOST_REQUIRE(chrono.deviceForwardTo(1U) == 1U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 1U);

    BOOST_REQUIRE(chrono.deviceForwardTo(2U) == 1U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 2U);

    BOOST_REQUIRE(chrono.deviceForwardToDelta(3U) == 0U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 3U);

    BOOST_REQUIRE(chrono.deviceForwardTo(4U) == 2U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 4U);

    BOOST_REQUIRE(chrono.deviceForwardTo(1U) == 2U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 4U);

    BOOST_REQUIRE(chrono.deviceAdvanceBy(1U) == 2U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 5U);

    BOOST_REQUIRE(chrono.deviceAdvanceBy(2U) == 3U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 7U);

    BOOST_REQUIRE(chrono.deviceAdvanceByDelta(1U) == 0U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 8U);

    BOOST_REQUIRE(chrono.deviceAdvanceByDelta(1U) == 0U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 9U);

    BOOST_REQUIRE(chrono.deviceAdvanceByDelta(1U) == 1U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 4U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 10U);

    chrono.deviceConsume(10U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 1U);

    chrono.systemConsume(1U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 1U);

    BOOST_TEST_MESSAGE("Reset");

    chrono.reset();
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 0U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 0U);

    BOOST_TEST_MESSAGE("Clock ratio");

    BOOST_REQUIRE(chrono.deviceAdvanceBy(1U) == 1U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 1U);

    chrono.setClockRatioFixedSystem(-3);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 1U);

    BOOST_REQUIRE(chrono.systemAdvanceBy(1U) == 2U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 2U);

    chrono.setClockRatioFixedSystem(0);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 6U);

    BOOST_REQUIRE(chrono.systemAdvanceBy(1U) == 9U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 9U);

    chrono.setClockRatioFixedSystem(3);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 27U);

    BOOST_REQUIRE(chrono.systemAdvanceBy(1U) == 36U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 4U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 36U);

    BOOST_TEST_MESSAGE("Various ZX-Spectrum timings");

    chrono.reset();
    chrono.setClockRatioFixedSystem(0);
    chrono.setSystemClockRateFixedSystem(3584000U);
    chrono.setDeviceClockRateFixedSystem(221800U);

    BOOST_REQUIRE(chrono.systemAdvanceByDelta(128U) == 8U);
    BOOST_REQUIRE(chrono.systemAdvanceByDelta(256U) == 16U);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 24U);
    BOOST_REQUIRE(chrono.systemForwardTo(3584000U) == 221800U);

    chrono.reset();
    chrono.setClockRatioFixedSystem(0);
    chrono.setSystemClockRateFixedSystem(44100U);

    BOOST_REQUIRE(chrono.deviceAdvanceByDelta(128U) == 26U);
    BOOST_REQUIRE(chrono.deviceAdvanceByDelta(256U) == 51U);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 77U);
    BOOST_REQUIRE(chrono.deviceForwardTo(221800U) == 44100U);
}
