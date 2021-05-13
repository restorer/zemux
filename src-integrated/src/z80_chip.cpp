// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

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

#include "z80_chip.h"
#include "z80_chip_core.h"
#include <mutex>

namespace zemux {

static bool isSharedDataInitialized = false;
static std::mutex initSharedDataMutex;

uint8_t Z80Chip::parityLookup[0x100];

void Z80Chip::initSharedData() {
    std::lock_guard _ { initSharedDataMutex };

    if (isSharedDataInitialized) {
        return;
    }

    for (int i = 0; i < 0x100; ++i) {
        int p = ((i & 0x80) >> 7) ^
                ((i & 0x40) >> 6) ^
                ((i & 0x20) >> 5) ^
                ((i & 0x10) >> 4) ^
                ((i & 0x08) >> 3) ^
                ((i & 0x04) >> 2) ^
                ((i & 0x02) >> 1) ^
                (i & 0x01);

        Z80Chip::parityLookup[i] = (p ? 0 : Z80Chip::FLAG_PV);
    }

    isSharedDataInitialized = true;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

Z80Chip::Z80Chip(void* callbackData,
        MreqRdCallback onMreqRd,
        MreqWrCallback onMreqWr,
        IorqRdCallback onIorqRd,
        IorqWrCallback onIorqWr,
        IorqM1Callback onIorqM1,
        PutAddressCallback onPutAddress,
        ChipType chipType
) : callbackData { callbackData },
        onMreqRd { onMreqRd },
        onMreqWr { onMreqWr },
        onIorqRd { onIorqRd },
        onIorqWr { onIorqWr },
        onIorqM1 { onIorqM1 },
        onPutAddress { onPutAddress },
        chipType { chipType } {

    initSharedData();

    regs.BC = 0xFFFF;
    regs.DE = 0xFFFF;
    regs.HL = 0xFFFF;
    regs.AF = 0xFFFF;
    regs.IX = 0xFFFF;
    regs.IY = 0xFFFF;
    regs.SP = 0xFFFF;
    regs.BC_ = 0xFFFF;
    regs.DE_ = 0xFFFF;
    regs.HL_ = 0xFFFF;
    regs.AF_ = 0xFFFF;

    reset();
}

#pragma clang diagnostic pop

void Z80Chip::setChipType(ChipType type) {
    chipType = type;
}

void Z80Chip::reset() {
    // From "Z80 CPU User Manual":
    //
    // Reset (input, active Low). RESET initializes the CPU as follows: it resets the
    // interrupt enable flip-flop, clears the Program Counter and registers I and R, and sets the
    // interrupt status to Mode 0.
    //
    // So all other registers may have any value.

    regs.PC = 0x0000;
    regs.MP = 0x0000;
    regs.IR = 0x0000;
    regs.IFF1 = false;
    regs.IFF2 = false;
    regs.IM = 0;

    optable = z80ChipOptable_00;
    isHalted = false;
    shouldResetPv = false;
    isProcessingInstruction = false;
    shouldSkipNextInterrupt = false;
    pcIncrement = 1;
    prefix = 0;
    tstate = 0;
}

uint_fast32_t Z80Chip::step() {
    shouldResetPv = false;
    shouldSkipNextInterrupt = false;
    tstate = 0;

    isProcessingInstruction = true;
    optable[fetchOpcode()](this);
    isProcessingInstruction = false;

    return tstate;
}

uint_fast32_t Z80Chip::doInt() {
    tstate = 0;

    if (!regs.IFF1 || shouldSkipNextInterrupt) {
        return tstate;
    }

    if (isProcessingInstruction || prefix) {
        if (chipType == TypeNmos) {
            shouldResetPv = true;
        }

        return tstate;
    }

    if (isHalted) {
        ++regs.PC;
        isHalted = false;
    }

    regs.IFF1 = false;
    regs.IFF2 = false;

    if (shouldResetPv) {
        regs.F &= ~FLAG_PV;
        shouldResetPv = false;
    }

    isProcessingInstruction = true;
    uint8_t intVec = fetchIntVec();

    switch (regs.IM) {
        case 0: {
            // From "The Z80 Family Program Interrupt Structure":
            //
            // The first byte of a multi-byte instruction is read during the interrupt acknowledge cycle.
            // Subsequent bytes are read in by a normal memory read sequence (the PC, however, remains
            // at its pre-interrupt state and the user must insure that memory will not respond
            // to these read sequences).

            pcIncrement = 0;
            optable[intVec](this);
            pcIncrement = 1;

            break;
        }

        case 1: {
            // From "The Z80 Family Program Interrupt Structure":
            //
            // Note that when doing programmed I/O the CPU will ignore any data put onto
            // the data bus during the interrupt acknowledge cycle.

            z80ChipOptable_00[0xFF](this); // RST #38
            break;
        }

            // IM 2
        default: {
            uint16_t address = intVec | (static_cast<uint16_t>(regs.I) << 8);

            memoryWrite(--regs.SP, regs.PCH);
            memoryWrite(--regs.SP, regs.PCL);

            regs.PCL = memoryRead(address);
            regs.PCH = memoryRead(address + 1);

            regs.MP = regs.PC;
            ++tstate;

            break;
        }
    }

    isProcessingInstruction = false;
    return tstate;
}

uint_fast32_t Z80Chip::doNmi() {
    tstate = 0;

    if (isProcessingInstruction || prefix || shouldSkipNextInterrupt) {
        return tstate;
    }

    if (isHalted) {
        ++regs.PC;
        isHalted = false;
    }

    isProcessingInstruction = true;
    ++tstate; // Additional T-state

    // From "Z80 CPU User Manual":
    //
    // The CPU response to a nonmaskable interrupt is similar to a normal memory read operation.
    // The only difference is that the contents of the data bus are ignored while the processor
    // automatically stores the Program Counter in the external stack and jumps to address 0066h.
    [[maybe_unused]] auto unused = fetchOpcode();

    // NOTICE: if you thinking about submitting bug-report about IFF1 not copied to IFF2, you are not right :)
    // actually IFF1 *not* copied to IFF2. this is tested on real hardware.
    regs.IFF1 = false;

    memoryWrite(--regs.SP, regs.PCH);
    memoryWrite(--regs.SP, regs.PCL);

    regs.PC = 0x0066;
    regs.MP = regs.PC;

    isProcessingInstruction = false;
    return tstate;
}

}
