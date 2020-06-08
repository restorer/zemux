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
#include <zemux_guest/chronometer.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

BOOST_AUTO_TEST_CASE(Chronometer) {
    zemux::Chronometer chrono(1, 3);

    BOOST_TEST_MESSAGE("Initial");

    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 0);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 0);

    BOOST_TEST_MESSAGE("System");

    BOOST_REQUIRE(chrono.systemForwardTo(1) == 3);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 3);

    BOOST_REQUIRE(chrono.systemForwardTo(2) == 6);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 6);

    BOOST_REQUIRE(chrono.systemForwardTo(1) == 6);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 6);

    BOOST_REQUIRE(chrono.systemAdvanceBy(1) == 9);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 9);

    BOOST_REQUIRE(chrono.systemAdvanceBy(2) == 15);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 5);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 15);

    BOOST_REQUIRE(chrono.systemAdvanceByDelta(1) == 3);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 6);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 18);

    BOOST_REQUIRE(chrono.systemAdvanceByDelta(2) == 6);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 8);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 24);

    BOOST_REQUIRE(chrono.systemForwardToDelta(9) == 3);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 9);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 27);

    chrono.systemConsume(9);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 0);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 0);

    BOOST_TEST_MESSAGE("Device");

    BOOST_REQUIRE(chrono.deviceForwardTo(1) == 1);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 1);

    BOOST_REQUIRE(chrono.deviceForwardTo(2) == 1);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 2);

    BOOST_REQUIRE(chrono.deviceForwardToDelta(3) == 0);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 3);

    BOOST_REQUIRE(chrono.deviceForwardTo(4) == 2);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 4);

    BOOST_REQUIRE(chrono.deviceForwardTo(1) == 2);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 4);

    BOOST_REQUIRE(chrono.deviceAdvanceBy(1) == 2);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 5);

    BOOST_REQUIRE(chrono.deviceAdvanceBy(2) == 3);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 7);

    BOOST_REQUIRE(chrono.deviceAdvanceByDelta(1) == 0);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 8);

    BOOST_REQUIRE(chrono.deviceAdvanceByDelta(1) == 0);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 9);

    BOOST_REQUIRE(chrono.deviceAdvanceByDelta(1) == 1);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 4);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 10);

    chrono.deviceConsume(10);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 1);

    chrono.systemConsume(1);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 1);

    BOOST_TEST_MESSAGE("Reset");

    chrono.reset();
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 0);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 0);

    BOOST_TEST_MESSAGE("Clock ratio");

    BOOST_REQUIRE(chrono.deviceAdvanceBy(1) == 1);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 1);

    chrono.setClockRatioFixedSystem(-3);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 1);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 1);

    BOOST_REQUIRE(chrono.systemAdvanceBy(1) == 2);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 2);

    chrono.setClockRatioFixedSystem(0);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 2);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 6);

    BOOST_REQUIRE(chrono.systemAdvanceBy(1) == 9);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 9);

    chrono.setClockRatioFixedSystem(3);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 3);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 27);

    BOOST_REQUIRE(chrono.systemAdvanceBy(1) == 36);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 4);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 36);

    BOOST_TEST_MESSAGE("Various ZX-Spectrum timings");

    chrono.reset();
    chrono.setClockRatioFixedSystem(0);
    chrono.setSystemClockRateFixedSystem(3584000);
    chrono.setDeviceClockRateFixedSystem(221800);

    BOOST_REQUIRE(chrono.systemAdvanceByDelta(128) == 8);
    BOOST_REQUIRE(chrono.systemAdvanceByDelta(256) == 16);
    BOOST_REQUIRE(chrono.getDeviceTicksPassed() == 24);
    BOOST_REQUIRE(chrono.systemForwardTo(3584000) == 221800);

    chrono.reset();
    chrono.setClockRatioFixedSystem(0);
    chrono.setSystemClockRateFixedSystem(44100);

    BOOST_REQUIRE(chrono.deviceAdvanceByDelta(128) == 26);
    BOOST_REQUIRE(chrono.deviceAdvanceByDelta(256) == 51);
    BOOST_REQUIRE(chrono.getSystemTicksPassed() == 77);
    BOOST_REQUIRE(chrono.deviceForwardTo(221800) == 44100);
}

#pragma clang diagnostic pop
