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
#include <cmath>
#include <zemux_core/data_io.h>
#include <zemux_peripherals/tape.h>
#include <zemux_peripherals/tape_tap.h>
#include "stub_data_io.h"
#include "stub_loudspeaker.h"

static constexpr uint32_t TOTAL_TOLERANCE_MICROS = 1000; // 1/1000 of a second
static constexpr uint32_t REWIND_TOLERANCE_MICROS = 1000 * 100; // 1/10 of a second
static constexpr uint32_t STEP_SHORT_TOLERANCE_MICROS = 30; // about 1/8 of 855 ticks (zero bit pulse)
static constexpr uint32_t STEP_WIDE_TOLERANCE_MICROS = 100; // 1/10000 of a second
static constexpr uint32_t STEP_WIDE_THRESHOLD_MICROS = 100000; // larger than all short pulses, but shorter than second delay phase

static const char* TAPE_PULSES_PATH = "../test-extras/tape.pulses";
static const char* TAPE_TAP_PATH = "../test-extras/tape.tap";

static void testTotalMicros(zemux::Tape& tape, zemux::DataReader& pulsesReader) {
    pulsesReader.seek(0);
    uint64_t totalTicks = 0;

    while (!pulsesReader.isEof()) {
        totalTicks += pulsesReader.readUInt32();
    }

    uint64_t totalMicros = zemux::tapeTicksToMicros(totalTicks);
    uint32_t diffMicros = std::abs(static_cast<int64_t>(totalMicros) - static_cast<int64_t>(tape.getTotalMicros()));

    BOOST_TEST_MESSAGE("-- totalMicros = " << totalMicros
            << ", tape.getTotalMicros() = " << tape.getTotalMicros()
            << ", diffMicros = " << diffMicros
            << " (of " << TOTAL_TOLERANCE_MICROS << " allowed)");

    BOOST_REQUIRE(diffMicros < TOTAL_TOLERANCE_MICROS);
}

static void testPulses(zemux::Tape& tape, zemux::DataReader& pulsesReader, uint64_t startMicros = 0) {
    if (startMicros > 0) {
        tape.rewindToNearest(startMicros);

        uint32_t diffMicros = std::abs(static_cast<int64_t>(startMicros) -
                static_cast<int64_t>(tape.getElapsedMicros()));

        BOOST_TEST_MESSAGE("-- startMicros = " << startMicros
                << ", tape.getElapsedMicros() = " << tape.getElapsedMicros()
                << ", diffMicros = " << diffMicros
                << " (of " << REWIND_TOLERANCE_MICROS << " allowed)");

        BOOST_REQUIRE(diffMicros < REWIND_TOLERANCE_MICROS);
        startMicros = tape.getElapsedMicros();
    }

    uint64_t ticksPassed = 0;
    pulsesReader.seek(0);

    if (pulsesReader.isEof()) {
        return;
    }

    bool volumeBit = false;

    while (!pulsesReader.isEof()) {
        uint32_t pulseTicks = pulsesReader.readUInt32();
        bool nextVolumeBit = !pulsesReader.isEof() && !volumeBit;

        if (zemux::tapeTicksToMicros(ticksPassed) < startMicros) {
            volumeBit = nextVolumeBit;
            ticksPassed += pulseTicks;
            continue;
        }

        uint32_t pulseMicros = zemux::tapeTicksToMicros(pulseTicks);

        // printf("*** ticks = %ld, pulseTicks = %d, pulseMicros = %d, volumeBit = %d, nextVolumeBit = %d\n",
        //         ticksPassed,
        //         pulseTicks,
        //         pulseMicros,
        //         volumeBit,
        //         nextVolumeBit);

        if (tape.getVolumeBit() != volumeBit) {
            BOOST_TEST_MESSAGE("-- ticksPassed = " << ticksPassed
                    << ", pulseTicks = " << pulseTicks
                    << ", volumeBit [" << volumeBit
                    << "] != tape.getVolumeBit() [" << tape.getVolumeBit() << "]");
        }

        BOOST_REQUIRE(tape.getVolumeBit() == volumeBit);
        uint32_t pulseMicrosOriginal = pulseMicros;

        for (pulseMicros += STEP_SHORT_TOLERANCE_MICROS; pulseMicros > 0; --pulseMicros) {
            tape.step(1);

            if (tape.getVolumeBit() != volumeBit) {
                break;
            }
        }

        if (tape.getVolumeBit() != nextVolumeBit) {
            BOOST_TEST_MESSAGE("-- ticksPassed = " << ticksPassed
                    << ", pulseTicks = " << pulseTicks
                    << ", pulseMicros (original) = " << pulseMicrosOriginal
                    << ", pulseMicros (left) = " << pulseMicros
                    << ", nextVolumeBit [" << nextVolumeBit
                    << "] != tape.getVolumeBit() [" << tape.getVolumeBit() << "]");
        }

        BOOST_REQUIRE(tape.getVolumeBit() == nextVolumeBit);

        uint32_t stepTolerance = (pulseMicrosOriginal > STEP_WIDE_THRESHOLD_MICROS
                ? STEP_WIDE_TOLERANCE_MICROS
                : STEP_SHORT_TOLERANCE_MICROS);

        if (pulseMicros > stepTolerance * 2) {
            BOOST_TEST_MESSAGE("-- ticksPassed = " << ticksPassed
                    << ", pulseTicks = " << pulseTicks
                    << ", pulseMicros (original) = " << pulseMicrosOriginal
                    << ", pulseMicros (left) = " << pulseMicros
                    << " (of " << (stepTolerance * 2) << " allowed)");
        }

        BOOST_REQUIRE(pulseMicros <= stepTolerance * 2);

        volumeBit = nextVolumeBit;
        ticksPassed += pulseTicks;
    }
}

static void testTape(zemux::Tape& tape) {
    FileDataReader pulsesReader { TAPE_PULSES_PATH };

    BOOST_TEST_MESSAGE("- total");
    testTotalMicros(tape, pulsesReader);

    BOOST_TEST_MESSAGE("- full");
    testPulses(tape, pulsesReader);

    BOOST_TEST_MESSAGE("- rewind 25%");
    testPulses(tape, pulsesReader, tape.getTotalMicros() / 4);

    BOOST_TEST_MESSAGE("- rewind 50%");
    testPulses(tape, pulsesReader, tape.getTotalMicros() / 2);

    // BOOST_TEST_MESSAGE("- rewind 75%");
    // testPulses(tape, pulsesReader, tape.getTotalMicros() * 3 / 4);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

BOOST_AUTO_TEST_CASE(TapeTapTest) {
    FileDataReader reader { TAPE_TAP_PATH };
    StubLoudspeaker loudspeaker;
    zemux::TapeTap tapeTap { &reader, &loudspeaker, true };
    testTape(tapeTap);
}

#pragma clang diagnostic pop
