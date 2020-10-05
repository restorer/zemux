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

static constexpr uint32_t STEP_TOLERANCE_MICROS = 32;
static constexpr uint32_t REWIND_TOLERANCE_MICROS = 1000 * 1000;

static const char* TAPE_PULSES_PATH = "../test-extras/tape.pulses";
static const char* TAPE_TAP_PATH = "../test-extras/tape.tap";

static void testTotalMicros(zemux::Tape& tape, zemux::DataReader& pulsesReader) {
    pulsesReader.seek(0);
    uint64_t totalMicros = 0;

    while (!pulsesReader.isEof()) {
        totalMicros += pulsesReader.readUInt32();
    }

    BOOST_TEST_MESSAGE(">>>> tape.getTotalMicros() = " << tape.getTotalMicros() << ", totalMicros = " << totalMicros);
    BOOST_REQUIRE(tape.getTotalMicros() == totalMicros);
}

static void testPulses(zemux::Tape& tape, zemux::DataReader& pulsesReader, uint64_t startMicros = 0) {
    if (startMicros > 0) {
        tape.rewindToNearest(startMicros);
        BOOST_REQUIRE(std::abs(static_cast<int64_t>(startMicros) - static_cast<int64_t>(tape.getElapsedMicros())) < REWIND_TOLERANCE_MICROS);
        startMicros = tape.getElapsedMicros();
    }

    bool volumeBit = false;
    pulsesReader.seek(0);

    while (!pulsesReader.isEof()) {
        uint32_t pulseMicros = pulsesReader.readUInt32();

        if (startMicros > pulseMicros) {
            startMicros -= pulseMicros;
        } else {
            startMicros = 0;
            pulseMicros += STEP_TOLERANCE_MICROS;
            BOOST_REQUIRE(tape.getVolumeBit() == volumeBit);

            while (pulseMicros--) {
                tape.step(1);

                if (tape.getVolumeBit() != volumeBit) {
                    break;
                }
            }

            BOOST_REQUIRE(tape.getVolumeBit() != volumeBit);
            BOOST_REQUIRE(pulseMicros < STEP_TOLERANCE_MICROS * 2);
        }

        volumeBit = !volumeBit;
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

    BOOST_TEST_MESSAGE("- rewind 75%");
    testPulses(tape, pulsesReader, tape.getTotalMicros() * 3 / 4);
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
