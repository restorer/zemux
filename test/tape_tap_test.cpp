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
#include <zemux_peripherals/tape_tap.h>
#include "stub_data_io.h"
#include "stub_loudspeaker.h"

static const char* TAPE_TAP_PATH = "../test-extras/tape.tap";

static constexpr unsigned int PILOT_MAIN_HALF_MICROS = 2168 * 20000 / 71680;
static constexpr unsigned int PILOT_MAIN_HEADER_MICROS = 4032 * PILOT_MAIN_HALF_MICROS * 2;
static constexpr unsigned int PILOT_MAIN_DATA_MICROS = 1611 * PILOT_MAIN_HALF_MICROS * 2;
static constexpr unsigned int PILOT_CAP_LOW_MICROS = 667 * 20000 / 71680;
static constexpr unsigned int PILOT_CAP_HIGH_MICROS = 735 * 20000 / 71680;
static constexpr unsigned int BIT_ZERO_HALF_MICROS = 855 * 20000 / 71680;
static constexpr unsigned int BIT_ONE_HALF_MICROS = 1710 * 20000 / 71680;
static constexpr unsigned int DELAY_MICROS = (2168 * 20000 / 71680) * 1000;

static constexpr unsigned int PILOT_HEADER_MICROS = PILOT_MAIN_HEADER_MICROS +
        PILOT_CAP_LOW_MICROS +
        PILOT_CAP_HIGH_MICROS;

static constexpr unsigned int PILOT_DATA_MICROS = PILOT_MAIN_DATA_MICROS +
        PILOT_CAP_LOW_MICROS +
        PILOT_CAP_HIGH_MICROS;

static constexpr unsigned int BYTE_0_MICROS = BIT_ZERO_HALF_MICROS * 16;
static constexpr unsigned int BYTE_1_MICROS = BIT_ONE_HALF_MICROS * 2 + BIT_ZERO_HALF_MICROS * 14;
static constexpr unsigned int BYTE_3_MICROS = BIT_ONE_HALF_MICROS * 4 + BIT_ZERO_HALF_MICROS * 12;
static constexpr unsigned int BYTE_32_MICROS = BIT_ONE_HALF_MICROS * 2 + BIT_ZERO_HALF_MICROS * 14;
static constexpr unsigned int BYTE_65_MICROS = BIT_ONE_HALF_MICROS * 4 + BIT_ZERO_HALF_MICROS * 12;
static constexpr unsigned int BYTE_66_MICROS = BIT_ONE_HALF_MICROS * 4 + BIT_ZERO_HALF_MICROS * 12;
static constexpr unsigned int BYTE_96_MICROS = BIT_ONE_HALF_MICROS * 4 + BIT_ZERO_HALF_MICROS * 12;
static constexpr unsigned int BYTE_97_MICROS = BIT_ONE_HALF_MICROS * 6 + BIT_ZERO_HALF_MICROS * 10;
static constexpr unsigned int BYTE_98_MICROS = BIT_ONE_HALF_MICROS * 6 + BIT_ZERO_HALF_MICROS * 10;
static constexpr unsigned int BYTE_128_MICROS = BIT_ONE_HALF_MICROS * 2 + BIT_ZERO_HALF_MICROS * 14;
static constexpr unsigned int BYTE_157_MICROS = BIT_ONE_HALF_MICROS * 10 + BIT_ZERO_HALF_MICROS * 6;
static constexpr unsigned int BYTE_158_MICROS = BIT_ONE_HALF_MICROS * 10 + BIT_ZERO_HALF_MICROS * 6;
static constexpr unsigned int BYTE_255_MICROS = BIT_ONE_HALF_MICROS * 16;

static constexpr unsigned int PART_1_HEADER_MICROS = PILOT_HEADER_MICROS +
        BYTE_0_MICROS + // flag: header
        BYTE_0_MICROS + // type: program
        BYTE_65_MICROS + BYTE_32_MICROS * 9 + // name: 'A'
        BYTE_1_MICROS + BYTE_0_MICROS + // data size
        BYTE_0_MICROS + BYTE_0_MICROS + // param 1
        BYTE_1_MICROS + BYTE_0_MICROS + // param 2
        BYTE_97_MICROS + // checksum
        DELAY_MICROS;

static constexpr unsigned int PART_1_DATA_MICROS = PILOT_DATA_MICROS +
        BYTE_255_MICROS + // flag: data
        BYTE_97_MICROS + // data
        BYTE_158_MICROS + // checksum
        DELAY_MICROS;

static constexpr unsigned int PART_2_HEADER_MICROS = PILOT_HEADER_MICROS +
        BYTE_0_MICROS + // flag: header
        BYTE_3_MICROS + // type: code
        BYTE_66_MICROS + BYTE_32_MICROS * 9 + // name: 'B'
        BYTE_1_MICROS + BYTE_0_MICROS + // data size
        BYTE_0_MICROS + BYTE_128_MICROS + // param 1
        BYTE_0_MICROS + BYTE_128_MICROS + // param 2
        BYTE_96_MICROS + // checksum
        DELAY_MICROS;

static constexpr unsigned int PART_2_DATA_MICROS = PILOT_DATA_MICROS +
        BYTE_255_MICROS + // flag: data
        BYTE_98_MICROS + // data
        BYTE_157_MICROS + // checksum
        DELAY_MICROS;

static constexpr unsigned int PART_1_MICROS = PART_1_HEADER_MICROS + PART_1_DATA_MICROS;
static constexpr unsigned int PART_2_MICROS = PART_2_HEADER_MICROS + PART_2_DATA_MICROS;
static constexpr unsigned int TOTAL_MICROS = PART_1_MICROS + PART_2_MICROS;

static void testCaseInit(zemux::TapeTap& tapeTap) {
    BOOST_REQUIRE(!tapeTap.getVolumeBit());
    tapeTap.step(50);
}

static void testCasePilot(zemux::TapeTap& tapeTap, unsigned int micros) {
    BOOST_REQUIRE(!tapeTap.getVolumeBit());
    tapeTap.step(PILOT_MAIN_HALF_MICROS);

    BOOST_REQUIRE(tapeTap.getVolumeBit());
    tapeTap.step(PILOT_MAIN_HALF_MICROS);

    BOOST_REQUIRE(!tapeTap.getVolumeBit());
    tapeTap.step(micros - PILOT_MAIN_HALF_MICROS * 2);

    BOOST_REQUIRE(!tapeTap.getVolumeBit());
    tapeTap.step(PILOT_CAP_LOW_MICROS);

    BOOST_REQUIRE(tapeTap.getVolumeBit());
    tapeTap.step(PILOT_CAP_HIGH_MICROS);
}

static void testCaseByte(zemux::TapeTap& tapeTap, uint8_t value) {
    for (uint8_t mask = 0x80; mask; mask >>= 1) {
        BOOST_REQUIRE(!tapeTap.getVolumeBit());
        tapeTap.step((value & mask) ? BIT_ONE_HALF_MICROS : BIT_ZERO_HALF_MICROS);

        BOOST_REQUIRE(tapeTap.getVolumeBit());
        tapeTap.step((value & mask) ? BIT_ONE_HALF_MICROS : BIT_ZERO_HALF_MICROS);
    }
}

static void testCaseHeaderBytes(zemux::TapeTap& tapeTap,
        uint8_t type,
        uint8_t nameFirst,
        uint16_t dataSize,
        uint16_t param1,
        uint16_t param2,
        uint8_t checksum) {

    BOOST_TEST_MESSAGE("- flag");
    testCaseByte(tapeTap, 0);

    BOOST_TEST_MESSAGE("- type");
    testCaseByte(tapeTap, type);

    BOOST_TEST_MESSAGE("- name first");
    testCaseByte(tapeTap, nameFirst);

    BOOST_TEST_MESSAGE("- name spaces");

    for (int i = 9; i--;) {
        testCaseByte(tapeTap, 32);
    }

    BOOST_TEST_MESSAGE("- data size");
    testCaseByte(tapeTap, dataSize & 0xFF);
    testCaseByte(tapeTap, dataSize >> 8);

    BOOST_TEST_MESSAGE("- param 1");
    testCaseByte(tapeTap, param1 & 0xFF);
    testCaseByte(tapeTap, param1 >> 8);

    BOOST_TEST_MESSAGE("- param 2");
    testCaseByte(tapeTap, param2 & 0xFF);
    testCaseByte(tapeTap, param2 >> 8);

    BOOST_TEST_MESSAGE("- checksum");
    testCaseByte(tapeTap, checksum);
}

static void testCaseDataBytes(zemux::TapeTap& tapeTap, uint8_t value, uint8_t checksum) {
    BOOST_TEST_MESSAGE("- flag");
    testCaseByte(tapeTap, 255);

    BOOST_TEST_MESSAGE("- value");
    testCaseByte(tapeTap, value);

    BOOST_TEST_MESSAGE("- checksum");
    testCaseByte(tapeTap, checksum);
}

static void testCaseDelay(zemux::TapeTap& tapeTap) {
    BOOST_REQUIRE(!tapeTap.getVolumeBit());
    tapeTap.step(DELAY_MICROS / 2);

    BOOST_REQUIRE(!tapeTap.getVolumeBit());
    tapeTap.step(DELAY_MICROS / 2);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

BOOST_AUTO_TEST_CASE(TapeTapTest) {
    FileDataReader reader { TAPE_TAP_PATH };
    StubLoudspeaker loudspeaker;
    zemux::TapeTap tapeTap { &reader, &loudspeaker, true };

    BOOST_REQUIRE(tapeTap.getTotalMicros() == TOTAL_MICROS);
    testCaseInit(tapeTap);

    BOOST_TEST_MESSAGE("First header pilot");
    testCasePilot(tapeTap, PILOT_MAIN_HEADER_MICROS);

    BOOST_TEST_MESSAGE("First header bytes");
    testCaseHeaderBytes(tapeTap, 0, 'A', 1, 0, 1, 97);

    BOOST_TEST_MESSAGE("First header delay");
    testCaseDelay(tapeTap);

    BOOST_TEST_MESSAGE("First data pilot");
    testCasePilot(tapeTap, PILOT_MAIN_DATA_MICROS);

    BOOST_TEST_MESSAGE("First data bytes");
    testCaseDataBytes(tapeTap, 'a', 158);

    BOOST_TEST_MESSAGE("First data delay");
    testCaseDelay(tapeTap);

    BOOST_TEST_MESSAGE("Second header pilot in middle");

    tapeTap.rewindToNearest(PART_1_MICROS +
            PILOT_MAIN_HEADER_MICROS / 2 +
            PILOT_MAIN_HALF_MICROS +
            50);

    BOOST_REQUIRE(tapeTap.getElapsedMicros() ==
            PART_1_MICROS + PILOT_MAIN_HEADER_MICROS / 2 + PILOT_MAIN_HALF_MICROS * 2);

    tapeTap.step(50);
    testCasePilot(tapeTap, PILOT_MAIN_HEADER_MICROS / 2 - PILOT_MAIN_HALF_MICROS * 2);

    BOOST_TEST_MESSAGE("Second header bytes");
    testCaseHeaderBytes(tapeTap, 3, 'B', 1, 32768, 32768, 96);

    BOOST_TEST_MESSAGE("Second header data in middle");

    tapeTap.rewindToNearest(PART_1_MICROS +
            PILOT_HEADER_MICROS +
            BYTE_0_MICROS +
            BYTE_3_MICROS +
            BYTE_66_MICROS +
            BYTE_32_MICROS * 4 +
            BYTE_32_MICROS / 2);

    BOOST_REQUIRE(tapeTap.getElapsedMicros() == PART_1_MICROS +
            PILOT_HEADER_MICROS +
            BYTE_0_MICROS +
            BYTE_3_MICROS +
            BYTE_66_MICROS +
            BYTE_32_MICROS * 4);

    BOOST_TEST_MESSAGE("- name spaces");

    for (int i = 5; i--;) {
        testCaseByte(tapeTap, 32);
    }

    BOOST_TEST_MESSAGE("- data size");
    testCaseByte(tapeTap, 1 & 0xFF);
    testCaseByte(tapeTap, 1 >> 8);

    BOOST_TEST_MESSAGE("- param 1");
    testCaseByte(tapeTap, 32768 & 0xFF);
    testCaseByte(tapeTap, 32768 >> 8);

    BOOST_TEST_MESSAGE("- param 2");
    testCaseByte(tapeTap, 32768 & 0xFF);
    testCaseByte(tapeTap, 32768 >> 8);

    BOOST_TEST_MESSAGE("- checksum");
    testCaseByte(tapeTap, 96);

    BOOST_TEST_MESSAGE("Behind");
    tapeTap.rewindToNearest(TOTAL_MICROS + BYTE_0_MICROS);
    BOOST_REQUIRE(tapeTap.getElapsedMicros() == TOTAL_MICROS);
}

#pragma clang diagnostic pop
