#ifndef ZEMUX_INTEGRATED__Z80_CHIP_CORE
#define ZEMUX_INTEGRATED__Z80_CHIP_CORE

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
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedGlobalDeclarationInspection"

namespace zemux {

extern Z80Chip::Opcode z80ChipOptable_00[0x100];
extern Z80Chip::Opcode z80ChipOptable_CB[0x100];
extern Z80Chip::Opcode z80ChipOptable_DD[0x100];
extern Z80Chip::Opcode z80ChipOptable_ED[0x100];
extern Z80Chip::Opcode z80ChipOptable_FD[0x100];
extern Z80Chip::Opcode z80ChipOptable_DD_CB[0x100];
extern Z80Chip::Opcode z80ChipOptable_FD_CB[0x100];

// Fake optimizations:
//
// (signedResult < -128 || signedResult > 127) <=> ((signedResult + 128) & 0xFF00)
// (signedResult < -32768 || signedResult > 32767) <=> ((signedResult + 32768) & 0xFFFF0000)
// static_cast<uint8_t>(result) <=> (result & 0xFF)
//
// For all cases, for x86-64 and for ARM Clang generates the same code, GCC generates almost the same code.
//
// ----
//
// Possible optimizations:
//
// For 8-bit operations precompute lookup-tables for flag F (max size will be 0x20000 for motionX and motionY and carry),
// but check if it will be actually faster (according to Godbolt Compiler Explorer the generated code
// for current variant has 3x more assembly instructions than code for lookup, but using lookup may have
// the same speed or be actually slower due to modern CPU architecture).

class Z80ChipCore final : private NonCopyable {
public:

    // Instructions (4 cycles)

    ZEMUX_FORCE_INLINE static void op_NOP(Z80Chip*) {
    }

    ZEMUX_FORCE_INLINE static void op_LD_R_R(Z80Chip*, uint8_t* r1, uint8_t r2) {
        *r1 = r2;
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_DO_A_R(Z80Chip* cpu, F block, uint8_t r) {
        cpu->regs.A = block(cpu, cpu->regs.A, r);
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_DO_R(Z80Chip* cpu, F block, uint8_t* r) {
        *r = block(cpu, *r);
    }

    ZEMUX_FORCE_INLINE static void op_EXX(Z80Chip* cpu) {
        uint8_t value = cpu->regs.BC;
        cpu->regs.BC = cpu->regs.BC_;
        cpu->regs.BC_ = value;

        value = cpu->regs.DE;
        cpu->regs.DE = cpu->regs.DE_;
        cpu->regs.DE_ = value;

        value = cpu->regs.HL;
        cpu->regs.HL = cpu->regs.HL_;
        cpu->regs.HL_ = value;
    }

    ZEMUX_FORCE_INLINE static void op_EX_AF_AF_(Z80Chip* cpu) {
        uint16_t value = cpu->regs.AF;
        cpu->regs.AF = cpu->regs.AF_;
        cpu->regs.AF_ = value;
    }

    ZEMUX_FORCE_INLINE static void op_EX_DE_HL(Z80Chip* cpu) {
        uint16_t value = cpu->regs.DE;
        cpu->regs.DE = cpu->regs.HL;
        cpu->regs.HL = value;
    }

    ZEMUX_FORCE_INLINE static void op_DAA(Z80Chip* cpu) {
        uint8_t value = cpu->regs.A;

        if (cpu->regs.F & Z80Chip::FLAG_N) {
            if ((cpu->regs.F & Z80Chip::FLAG_H) || (cpu->regs.A & 0x0F) > 9) {
                value -= 6;
            }

            if ((cpu->regs.F & Z80Chip::FLAG_C) || cpu->regs.A > 0x99) {
                value -= 0x60;
            }
        } else {
            if ((cpu->regs.F & Z80Chip::FLAG_H) || (cpu->regs.A & 0x0F) > 9) {
                value += 6;
            }

            if ((cpu->regs.F & Z80Chip::FLAG_C) || cpu->regs.A > 0x99) {
                value += 0x60;
            }
        }

        cpu->regs.F = (cpu->regs.F & (Z80Chip::FLAG_N | Z80Chip::FLAG_C))
                | (value & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (value ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[value]
                | ((value ^ cpu->regs.A) & Z80Chip::FLAG_H)
                | (cpu->regs.A > 0x99 ? Z80Chip::FLAG_C : 0);

        cpu->regs.A = value;
    }

    ZEMUX_FORCE_INLINE static void op_CPL(Z80Chip* cpu) {
        cpu->regs.A ^= 0xFF;

        cpu->regs.F = (cpu->regs.F & (Z80Chip::FLAG_C | Z80Chip::FLAG_PV | Z80Chip::FLAG_Z | Z80Chip::FLAG_S))
                | (cpu->regs.A & (Z80Chip::FLAG_3 | Z80Chip::FLAG_5))
                | (Z80Chip::FLAG_N | Z80Chip::FLAG_H);
    }

    ZEMUX_FORCE_INLINE static void op_CCF(Z80Chip* cpu) {
        cpu->regs.F = (cpu->regs.F & (Z80Chip::FLAG_PV | Z80Chip::FLAG_Z | Z80Chip::FLAG_S))
                | ((cpu->regs.F & Z80Chip::FLAG_C) << Z80Chip::FLAG_C_TO_H)
                | ((cpu->regs.F & Z80Chip::FLAG_C) ^ Z80Chip::FLAG_C)
                | (cpu->regs.A & (Z80Chip::FLAG_3 | Z80Chip::FLAG_5));
    }

    ZEMUX_FORCE_INLINE static void op_SCF(Z80Chip* cpu) {
        cpu->regs.F = (cpu->regs.F & (Z80Chip::FLAG_PV | Z80Chip::FLAG_Z | Z80Chip::FLAG_S))
                | (cpu->regs.A & (Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | Z80Chip::FLAG_C;
    }

    ZEMUX_FORCE_INLINE static void op_DI(Z80Chip* cpu) {
        cpu->regs.IFF1 = false;
        cpu->regs.IFF2 = false;
    }

    ZEMUX_FORCE_INLINE static void op_EI(Z80Chip* cpu) {
        cpu->regs.IFF1 = true;
        cpu->regs.IFF2 = true;

        // From "The Z80 Family Program Interrupt Structure":
        //
        // The enable interrupt instruction (EI) will set both IFF1 and IFF2 to a logic 'one'
        // allowing recognition of any maskable interrupt at the completion of the instruction
        // following the EI instruction.
        cpu->shouldSkipNextInterrupt = true;
    }

    ZEMUX_FORCE_INLINE static void op_RLA(Z80Chip* cpu) {
        uint8_t value = cpu->regs.A;
        cpu->regs.A = (cpu->regs.A << 1) | (cpu->regs.F & Z80Chip::FLAG_C);

        cpu->regs.F = (cpu->regs.F & (Z80Chip::FLAG_PV | Z80Chip::FLAG_Z | Z80Chip::FLAG_S))
                | (cpu->regs.A & (Z80Chip::FLAG_3 | Z80Chip::FLAG_5))
                | (value >> 7);
    }

    ZEMUX_FORCE_INLINE static void op_RRA(Z80Chip* cpu) {
        uint8_t value = cpu->regs.A;
        cpu->regs.A = (cpu->regs.A >> 1) | (cpu->regs.F << 7);

        cpu->regs.F = (cpu->regs.F & (Z80Chip::FLAG_PV | Z80Chip::FLAG_Z | Z80Chip::FLAG_S))
                | (cpu->regs.A & (Z80Chip::FLAG_3 | Z80Chip::FLAG_5))
                | (value & Z80Chip::FLAG_C);
    }

    ZEMUX_FORCE_INLINE static void op_RLCA(Z80Chip* cpu) {
        cpu->regs.A = (cpu->regs.A << 1) | (cpu->regs.A >> 7);

        cpu->regs.F = (cpu->regs.F & (Z80Chip::FLAG_PV | Z80Chip::FLAG_Z | Z80Chip::FLAG_S))
                | (cpu->regs.A & (Z80Chip::FLAG_C | Z80Chip::FLAG_3 | Z80Chip::FLAG_5));
    }

    ZEMUX_FORCE_INLINE static void op_RRCA(Z80Chip* cpu) {
        cpu->regs.F = (cpu->regs.F & (Z80Chip::FLAG_PV | Z80Chip::FLAG_Z | Z80Chip::FLAG_S)) |
                (cpu->regs.A & Z80Chip::FLAG_C);
        cpu->regs.A = (cpu->regs.A >> 1) | (cpu->regs.A << 7);
        cpu->regs.F |= (cpu->regs.A & (Z80Chip::FLAG_3 | Z80Chip::FLAG_5));
    }

    ZEMUX_FORCE_INLINE static void op_JP_RP(Z80Chip* cpu, uint16_t rp) {
        cpu->regs.PC = rp;
    }

    ZEMUX_FORCE_INLINE static void op_PREF_CB(Z80Chip* cpu) {
        cpu->prefix = 0xCB;
        cpu->optable = z80ChipOptable_CB;
    }

    ZEMUX_FORCE_INLINE static void op_PREF_DD(Z80Chip* cpu) {
        cpu->prefix = 0xDD;
        cpu->optable = z80ChipOptable_DD;
    }

    ZEMUX_FORCE_INLINE static void op_PREF_ED(Z80Chip* cpu) {
        cpu->prefix = 0xED;
        cpu->optable = z80ChipOptable_ED;
    }

    ZEMUX_FORCE_INLINE static void op_PREF_FD(Z80Chip* cpu) {
        cpu->prefix = 0xFD;
        cpu->optable = z80ChipOptable_FD;
    }

    ZEMUX_FORCE_INLINE static void op_HALT(Z80Chip* cpu) {
        cpu->isHalted = true;
        ++(cpu->regs.PC);
    }

    // Instructions (6 cycles)

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_DO_RP(Z80Chip* cpu, F block, uint16_t* rp) {
        *rp = block(cpu, *rp);
        cpu->putAddressOnBus(cpu->regs.IR, 2);
    }

    ZEMUX_FORCE_INLINE static void op_LD_SP_RP(Z80Chip* cpu, uint16_t rp) {
        cpu->regs.SP = rp;
        cpu->putAddressOnBus(cpu->regs.IR, 2);
    }

    // Instructions (7 cycles)

    ZEMUX_FORCE_INLINE static void op_LD_R_N(Z80Chip* cpu, uint8_t* r) {
        *r = cpu->fetchByte();
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_DO_A_N(Z80Chip* cpu, F block) {
        cpu->regs.A = block(cpu, cpu->regs.A, cpu->fetchByte());
    }

    ZEMUX_FORCE_INLINE static void op_LD_R_MRP(Z80Chip* cpu, uint8_t* r, uint16_t rp) {
        *r = cpu->memoryRead(rp);
    }

    ZEMUX_FORCE_INLINE static void op_LD_MRP_R(Z80Chip* cpu, uint16_t rp, uint8_t r) {
        cpu->memoryWrite(rp, r);
    }

    ZEMUX_FORCE_INLINE static void op_LD_A_MRP(Z80Chip* cpu, uint16_t rp) {
        cpu->regs.A = cpu->memoryRead(rp);
        cpu->regs.MP = rp + 1;
    }

    ZEMUX_FORCE_INLINE static void op_LD_MRP_A(Z80Chip* cpu, uint16_t rp) {
        cpu->regs.MPH = cpu->regs.A;
        cpu->regs.MPL = rp + 1;
        cpu->memoryWrite(rp, cpu->regs.A);
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_DO_A_MHL(Z80Chip* cpu, F block) {
        cpu->regs.A = block(cpu, cpu->regs.A, cpu->memoryRead(cpu->regs.HL));
    }

    // Instructions (8 cycles)

    ZEMUX_FORCE_INLINE static void op_BIT_R(Z80Chip* cpu, int bit, uint8_t r) {
        uint8_t value = r & (1 << bit);

        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C)
                | Z80Chip::FLAG_H
                | (value & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (value ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[value]
                | (r & (Z80Chip::FLAG_5 | Z80Chip::FLAG_3));
    }

    ZEMUX_FORCE_INLINE static void op_SET_R(Z80Chip*, int bit, uint8_t* r) {
        *r |= (1 << bit);
    }

    ZEMUX_FORCE_INLINE static void op_RES_R(Z80Chip*, int bit, uint8_t* r) {
        *r &= ~(1 << bit);
    }

    ZEMUX_FORCE_INLINE static void op_NEG_P00(Z80Chip* cpu) {
        uint8_t value = cpu->regs.A;
        cpu->regs.A = 0;
        cpu->regs.A = do_SUB_8(cpu, cpu->regs.A, value);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_IM_P00(Z80Chip* cpu, int mode) {
        cpu->regs.IM = mode;
        do_PREF_00(cpu);
    }

    // Instructions (9 cycles)

    ZEMUX_FORCE_INLINE static void op_LD_A_I_P00(Z80Chip* cpu) {
        cpu->regs.A = cpu->regs.I;

        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C)
                | (cpu->regs.A & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (cpu->regs.A ? 0 : Z80Chip::FLAG_Z)
                | (cpu->regs.IFF2 << Z80Chip::FLAG_PV_IFF_S8);

        cpu->putAddressOnBus(cpu->regs.IR, 1);

        if (cpu->shouldResetPv) {
            // doInt() was called immediately before the command (but after the prefix),
            // or doInt() was called during last call to putAddressOnBus().
            cpu->regs.F &= ~Z80Chip::FLAG_PV;
        } else {
            // For the case when doInt() will be called immediately after the command.
            cpu->shouldResetPv = (cpu->chipType == Z80Chip::TypeNmos);
        }

        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_LD_A_RR_P00(Z80Chip* cpu) {
        cpu->regs.A = cpu->regs.R;

        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C)
                | (cpu->regs.A & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (cpu->regs.A ? 0 : Z80Chip::FLAG_Z)
                | (cpu->regs.IFF2 << Z80Chip::FLAG_PV_IFF_S8);

        cpu->putAddressOnBus(cpu->regs.IR, 1);

        if (cpu->shouldResetPv) {
            // doInt() was called immediately before the command (but after the prefix),
            // or doInt() was called during last call to putAddressOnBus().
            cpu->regs.F &= ~Z80Chip::FLAG_PV;
        } else {
            // For the case when doInt() will be called immediately after the command.
            cpu->shouldResetPv = (cpu->chipType == Z80Chip::TypeNmos);
        }

        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_LD_I_A_P00(Z80Chip* cpu) {
        cpu->regs.I = cpu->regs.A;
        cpu->putAddressOnBus(cpu->regs.IR, 1);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_LD_RR_A_P00(Z80Chip* cpu) {
        cpu->regs.R = cpu->regs.A;
        cpu->putAddressOnBus(cpu->regs.IR, 1);
        do_PREF_00(cpu);
    }

    // Instructions (10 cycles)

    ZEMUX_FORCE_INLINE static void op_LD_RP_NN(Z80Chip* cpu, uint16_t* rp) {
        *rp = cpu->fetchWord();
    }

    ZEMUX_FORCE_INLINE static void op_JP(Z80Chip* cpu) {
        cpu->regs.PC = cpu->fetchWord();
        cpu->regs.MP = cpu->regs.PC;
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_JP_CC(Z80Chip* cpu, F block) {
        uint16_t address = cpu->fetchWord();
        cpu->regs.MP = address;

        if (block(cpu)) {
            cpu->regs.PC = address;
        }
    }

    ZEMUX_FORCE_INLINE static void op_LD_MHL_N(Z80Chip* cpu) {
        cpu->memoryWrite(cpu->regs.HL, cpu->fetchByte());
    }

    ZEMUX_FORCE_INLINE static void op_POP_RP(Z80Chip* cpu, uint16_t* rp) {
        *rp = do_POP(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_RET(Z80Chip* cpu) {
        do_RET(cpu);
    }

    // Instructions (11 cycles)

    ZEMUX_FORCE_INLINE static void op_ADD_RP_RP(Z80Chip* cpu, uint16_t* rp1, uint16_t rp2) {
        *rp1 = do_ADD_16(cpu, *rp1, rp2);
        cpu->putAddressOnBus(cpu->regs.IR, 7);
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_DO_MHL(Z80Chip* cpu, F block) {
        uint8_t value = block(cpu, cpu->memoryRead(cpu->regs.HL));
        cpu->putAddressOnBus(cpu->regs.HL, 1);
        cpu->memoryWrite(cpu->regs.HL, value);
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_RET_CC(Z80Chip* cpu, F block) {
        cpu->putAddressOnBus(cpu->regs.IR, 1);

        if (block(cpu)) {
            do_RET(cpu);
        }
    }

    ZEMUX_FORCE_INLINE static void op_PUSH_RP(Z80Chip* cpu, uint16_t rp) {
        cpu->putAddressOnBus(cpu->regs.IR, 1);
        do_PUSH(cpu, rp);
    }

    ZEMUX_FORCE_INLINE static void op_RST(Z80Chip* cpu, uint16_t address) {
        cpu->putAddressOnBus(cpu->regs.IR, 1);
        do_PUSH(cpu, cpu->regs.PC);
        cpu->regs.PC = address;
        cpu->regs.MP = address;
    }

    ZEMUX_FORCE_INLINE static void op_IN_A_N(Z80Chip* cpu) {
        uint16_t port = (static_cast<uint16_t>(cpu->regs.A) << 8) | cpu->fetchByte();
        cpu->regs.A = do_IN(cpu, port);
    }

    ZEMUX_FORCE_INLINE static void op_OUT_N_A(Z80Chip* cpu) {
        uint16_t port = (static_cast<uint16_t>(cpu->regs.A) << 8) | cpu->fetchByte();
        do_OUT(cpu, port, cpu->regs.A);
    }

    // Instructions (12 cycles)

    ZEMUX_FORCE_INLINE static void op_BIT_MHL(Z80Chip* cpu, int bit) {
        do_BIT(cpu, cpu->memoryRead(cpu->regs.HL), bit);
        cpu->putAddressOnBus(cpu->regs.HL, 1);
    }

    ZEMUX_FORCE_INLINE static void op_JR(Z80Chip* cpu) {
        uint16_t address = cpu->regs.PC;
        auto offset = static_cast<int8_t>(cpu->fetchByte());
        cpu->putAddressOnBus(address, 5);

        cpu->regs.PC += offset;
        cpu->regs.MP = cpu->regs.PC;
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_JR_CC(Z80Chip* cpu, F block) {
        uint16_t address = cpu->regs.PC;
        auto offset = static_cast<int8_t>(cpu->fetchByte());

        if (block(cpu)) {
            cpu->putAddressOnBus(address, 5);
            cpu->regs.PC += offset;
            cpu->regs.MP = cpu->regs.PC;
        }
    }

    ZEMUX_FORCE_INLINE static void op_IN_R_BC_P00(Z80Chip* cpu, uint8_t* r) {
        uint8_t value = cpu->ioRead(cpu->regs.BC);
        *r = value;
        cpu->regs.MP = cpu->regs.BC + 1;

        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C)
                | (value & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (value ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[value];

        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_OUT_BC_R_P00(Z80Chip* cpu, uint8_t r) {
        cpu->ioWrite(cpu->regs.BC, r);
        cpu->regs.MP = cpu->regs.BC + 1;
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_IN_F_BC_P00(Z80Chip* cpu) {
        uint8_t value = cpu->ioRead(cpu->regs.BC);
        cpu->regs.MP = cpu->regs.BC + 1;

        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C)
                | (value & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (value ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[value];

        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_OUT_BC_0_P00(Z80Chip* cpu) {
        cpu->ioWrite(cpu->regs.BC, cpu->chipType == Z80Chip::TypeCmos ? 0xFF : 0x00);
        cpu->regs.MP = cpu->regs.BC + 1;
        do_PREF_00(cpu);
    }

    // Instructions (13 cycles)

    ZEMUX_FORCE_INLINE static void op_LD_A_MNN(Z80Chip* cpu) {
        uint16_t address = cpu->fetchWord();
        cpu->regs.A = cpu->memoryRead(address);
        cpu->regs.MP = address + 1;
    }

    ZEMUX_FORCE_INLINE static void op_LD_MNN_A(Z80Chip* cpu) {
        uint16_t address = cpu->fetchWord();
        cpu->regs.MPH = cpu->regs.A;
        cpu->regs.MPL = static_cast<uint8_t>(address + 1);
        cpu->memoryWrite(address, cpu->regs.A);
    }

    ZEMUX_FORCE_INLINE static void op_DJNZ(Z80Chip* cpu) {
        cpu->putAddressOnBus(cpu->regs.IR, 1);
        uint16_t address = cpu->regs.PC;
        auto offset = static_cast<int8_t>(cpu->fetchByte());

        if (--(cpu->regs.B)) {
            cpu->putAddressOnBus(address, 5);
            cpu->regs.PC += offset;
            cpu->regs.MP = cpu->regs.PC;
        }
    }

    // Instructions (14 cycles)

    ZEMUX_FORCE_INLINE static void op_RETI_P00(Z80Chip* cpu) {
        cpu->regs.IFF1 = cpu->regs.IFF2;
        do_RET(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_RETN_P00(Z80Chip* cpu) {
        op_RETI_P00(cpu);
    }

    // Instructions (15 cycles)

    ZEMUX_FORCE_INLINE static void op_ADC_HL_RP_P00(Z80Chip* cpu, uint16_t rp) {
        cpu->regs.HL = do_ADC_16(cpu, cpu->regs.HL, rp);
        cpu->putAddressOnBus(cpu->regs.IR, 7);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_SBC_HL_RP_P00(Z80Chip* cpu, uint16_t rp) {
        cpu->regs.HL = do_SBC_16(cpu, cpu->regs.HL, rp);
        cpu->putAddressOnBus(cpu->regs.IR, 7);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_SET_MHL(Z80Chip* cpu, int bit) {
        uint8_t value = cpu->memoryRead(cpu->regs.HL);
        cpu->putAddressOnBus(cpu->regs.HL, 1);
        cpu->memoryWrite(cpu->regs.HL, value | (1 << bit));
    }

    ZEMUX_FORCE_INLINE static void op_RES_MHL(Z80Chip* cpu, int bit) {
        uint8_t value = cpu->memoryRead(cpu->regs.HL);
        cpu->putAddressOnBus(cpu->regs.HL, 1);
        cpu->memoryWrite(cpu->regs.HL, value & ~(1 << bit));
    }

    // Instructions (16 cycles)

    ZEMUX_FORCE_INLINE static void op_PREF_XX_CB(Z80Chip* cpu, Z80Chip::Opcode* withOptable) {
        cpu->cbOffset = static_cast<int8_t>(cpu->fetchByte());

        uint16_t address = cpu->regs.PC;
        uint8_t opcode = cpu->fetchByte();
        cpu->putAddressOnBus(address, 2);

        withOptable[opcode](cpu);
    }

    ZEMUX_FORCE_INLINE static void op_LD_RP_MNN(Z80Chip* cpu, uint16_t* rp) {
        uint16_t address = cpu->fetchWord();
        *rp = cpu->memoryRead(address) | (static_cast<uint16_t>(cpu->memoryRead(address + 1)) << 8);
        cpu->regs.MP = address + 1;
    }

    ZEMUX_FORCE_INLINE static void op_LD_MNN_RP(Z80Chip* cpu, uint16_t rp) {
        uint16_t address = cpu->fetchWord();
        cpu->regs.MP = address + 1;
        cpu->memoryWrite(address, static_cast<uint8_t>(rp));
        cpu->memoryWrite(address + 1, static_cast<uint8_t>(rp >> 8));
    }

    ZEMUX_FORCE_INLINE static void op_LDI_P00(Z80Chip* cpu) {
        do_REP_LD(cpu);
        do_REP_LD_INC(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_LDD_P00(Z80Chip* cpu) {
        do_REP_LD(cpu);
        do_REP_LD_DEC(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_CPI_P00(Z80Chip* cpu) {
        do_REP_CP(cpu);
        do_REP_CP_INC(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_CPD_P00(Z80Chip* cpu) {
        do_REP_CP(cpu);
        do_REP_CP_DEC(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_INI_P00(Z80Chip* cpu) {
        do_REP_INI(cpu);
        ++(cpu->regs.HL);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_IND_P00(Z80Chip* cpu) {
        do_REP_IND(cpu);
        --(cpu->regs.HL);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_OUTI_P00(Z80Chip* cpu) {
        do_REP_OUTI(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_OUTD_P00(Z80Chip* cpu) {
        do_REP_OUTD(cpu);
        do_PREF_00(cpu);
    }

    // Instructions (17 cycles)

    ZEMUX_FORCE_INLINE static void op_CALL(Z80Chip* cpu) {
        uint16_t address = cpu->fetchWord();
        cpu->putAddressOnBus(cpu->regs.PC - 1, 1);
        do_PUSH(cpu, cpu->regs.PC);

        cpu->regs.PC = address;
        cpu->regs.MP = address;
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_CALL_CC(Z80Chip* cpu, F block) {
        uint16_t address = cpu->fetchWord();
        cpu->regs.MP = address;

        if (block(cpu)) {
            cpu->putAddressOnBus(cpu->regs.PC - 1, 1);
            do_PUSH(cpu, cpu->regs.PC);
            cpu->regs.PC = address;
        }
    }

    // Instructions (18 cycles)

    ZEMUX_FORCE_INLINE static void op_RLD_P00(Z80Chip* cpu) {
        uint8_t value = cpu->memoryRead(cpu->regs.HL);
        cpu->putAddressOnBus(cpu->regs.HL, 4);
        cpu->memoryWrite(cpu->regs.HL, (value << 4) | (cpu->regs.A & 0x0F));
        cpu->regs.A = (cpu->regs.A & 0xF0) | (value >> 4);

        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C)
                | (cpu->regs.A & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (cpu->regs.A ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[cpu->regs.A];

        cpu->regs.MP = cpu->regs.HL + 1;
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_RRD_P00(Z80Chip* cpu) {
        uint8_t value = cpu->memoryRead(cpu->regs.HL);
        cpu->putAddressOnBus(cpu->regs.HL, 4);
        cpu->memoryWrite(cpu->regs.HL, (cpu->regs.A << 4) | (value >> 4));
        cpu->regs.A = (cpu->regs.A & 0xF0) | (value & 0x0F);

        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C)
                | (cpu->regs.A & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (cpu->regs.A ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[cpu->regs.A];

        cpu->regs.MP = cpu->regs.HL + 1;
        do_PREF_00(cpu);
    }

    // Instructions (19 cycles)

    ZEMUX_FORCE_INLINE static void op_LD_R_ORP_P00(Z80Chip* cpu, uint8_t* r, uint16_t rp) {
        uint16_t address = cpu->regs.PC;
        int8_t offset = cpu->fetchOffsetMp(rp);
        cpu->putAddressOnBus(address, 5);
        *r = cpu->memoryRead(rp + offset);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_LD_ORP_R_P00(Z80Chip* cpu, uint16_t rp, uint8_t r) {
        uint16_t address = cpu->regs.PC;
        int8_t offset = cpu->fetchOffsetMp(rp);
        cpu->putAddressOnBus(address, 5);
        cpu->memoryWrite(rp + offset, r);
        do_PREF_00(cpu);
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_DO_A_ORP_P00(Z80Chip* cpu, F block, uint16_t rp) {
        uint16_t address = cpu->regs.PC;
        int8_t offset = cpu->fetchOffsetMp(rp);
        cpu->putAddressOnBus(address, 5);
        cpu->regs.A = block(cpu, cpu->regs.A, cpu->memoryRead(rp + offset));
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_LD_ORP_N_P00(Z80Chip* cpu, uint16_t rp) {
        int8_t offset = cpu->fetchOffsetMp(rp);

        uint16_t address = cpu->regs.PC;
        uint8_t value = cpu->fetchByte();
        cpu->putAddressOnBus(address, 2);

        cpu->memoryWrite(rp + offset, value);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_EX_MSP_RP(Z80Chip* cpu, uint16_t* rp) {
        uint16_t value = cpu->memoryRead(cpu->regs.SP) |
                (static_cast<uint16_t>(cpu->memoryRead(cpu->regs.SP + 1)) << 8);

        cpu->putAddressOnBus(cpu->regs.SP + 1, 1);
        cpu->regs.MP = value;

        uint16_t rpv = *rp;
        cpu->memoryWrite(cpu->regs.SP, static_cast<uint8_t>(rpv));
        cpu->memoryWrite(cpu->regs.SP + 1, static_cast<uint8_t>(rpv >> 8));

        cpu->putAddressOnBus(cpu->regs.SP + 1, 2);
        *rp = value;
    }

    // Instructions (20 cycles)

    ZEMUX_FORCE_INLINE static void op_BIT_PORP_P00(Z80Chip* cpu, int bit, uint16_t rp) {
        uint16_t address = rp + cpu->cbOffset;
        cpu->regs.MP = address;

        do_BIT(cpu, cpu->memoryRead(address), bit);
        cpu->putAddressOnBus(address, 1);
        do_PREF_00(cpu);
    }

    // Instructions (21 cycle)

    ZEMUX_FORCE_INLINE static void op_LDIR_P00(Z80Chip* cpu) {
        do_REP_LD(cpu);
        do_REP_LD_INCR(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_LDDR_P00(Z80Chip* cpu) {
        do_REP_LD(cpu);
        do_REP_LD_DECR(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_CPIR_P00(Z80Chip* cpu) {
        do_REP_CP(cpu);
        do_REP_CP_INCR(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_CPDR_P00(Z80Chip* cpu) {
        do_REP_CP(cpu);
        do_REP_CP_DECR(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_INIR_P00(Z80Chip* cpu) {
        do_REP_INI(cpu);
        do_REP_IN_INCR(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_INDR_P00(Z80Chip* cpu) {
        do_REP_IND(cpu);
        do_REP_IN_DECR(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_OTIR_P00(Z80Chip* cpu) {
        do_REP_OUTI(cpu);
        do_REP_OUT_INCR_DECR(cpu);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_OTDR_P00(Z80Chip* cpu) {
        do_REP_OUTD(cpu);
        do_REP_OUT_INCR_DECR(cpu);
        do_PREF_00(cpu);
    }

    // Instructions (23 cycles)

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_DO_ORP_P00(Z80Chip* cpu, F block, uint16_t rp) {
        uint16_t offsetAddress = cpu->regs.PC;
        uint16_t valueAddress = rp + cpu->fetchOffsetMp(rp);
        cpu->putAddressOnBus(offsetAddress, 5);

        uint8_t value = block(cpu, cpu->memoryRead(valueAddress));
        cpu->putAddressOnBus(valueAddress, 1);

        cpu->memoryWrite(valueAddress, value);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_SET_PORP_P00(Z80Chip* cpu, int bit, uint16_t rp) {
        uint16_t address = rp + cpu->cbOffset;
        cpu->regs.MP = address;

        uint8_t value = cpu->memoryRead(address);
        cpu->putAddressOnBus(address, 1);

        cpu->memoryWrite(address, value | (1 << bit));
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_RES_PORP_P00(Z80Chip* cpu, int bit, uint16_t rp) {
        uint16_t address = rp + cpu->cbOffset;
        cpu->regs.MP = address;

        uint8_t value = cpu->memoryRead(address);
        cpu->putAddressOnBus(address, 1);

        cpu->memoryWrite(address, value & ~(1 << bit));
        do_PREF_00(cpu);
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_DO_PORP_P00(Z80Chip* cpu, F block, uint16_t rp) {
        uint16_t address = rp + cpu->cbOffset;
        cpu->regs.MP = address;

        uint8_t value = block(cpu, cpu->memoryRead(address));
        cpu->putAddressOnBus(address, 1);

        cpu->memoryWrite(address, value);
        do_PREF_00(cpu);
    }

    template<typename F>
    ZEMUX_FORCE_INLINE static void op_LD_DO_R_PORP_P00(Z80Chip* cpu, F block, uint8_t* r, uint16_t rp) {
        uint16_t address = rp + cpu->cbOffset;
        cpu->regs.MP = address;

        uint8_t value = block(cpu, cpu->memoryRead(address));
        cpu->putAddressOnBus(address, 1);

        *r = value;
        cpu->memoryWrite(address, value);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_LD_RES_PORP_P00(Z80Chip* cpu, int bit, uint8_t* r, uint16_t rp) {
        uint16_t address = rp + cpu->cbOffset;
        cpu->regs.MP = address;

        uint8_t value = cpu->memoryRead(address) & ~(1 << bit);
        cpu->putAddressOnBus(address, 1);

        *r = value;
        cpu->memoryWrite(address, value);
        do_PREF_00(cpu);
    }

    ZEMUX_FORCE_INLINE static void op_LD_SET_PORP_P00(Z80Chip* cpu, int bit, uint8_t* r, uint16_t rp) {
        uint16_t address = rp + cpu->cbOffset;
        cpu->regs.MP = address;

        uint8_t value = cpu->memoryRead(address) | (1 << bit);
        cpu->putAddressOnBus(address, 1);

        *r = value;
        cpu->memoryWrite(address, value);
        do_PREF_00(cpu);
    }

    // Conditions

    ZEMUX_FORCE_INLINE static bool cc_NZ(Z80Chip* cpu) {
        return !(cpu->regs.F & Z80Chip::FLAG_Z);
    }

    ZEMUX_FORCE_INLINE static bool cc_Z(Z80Chip* cpu) {
        return cpu->regs.F & Z80Chip::FLAG_Z;
    }

    ZEMUX_FORCE_INLINE static bool cc_NC(Z80Chip* cpu) {
        return !(cpu->regs.F & Z80Chip::FLAG_C);
    }

    ZEMUX_FORCE_INLINE static bool cc_C(Z80Chip* cpu) {
        return cpu->regs.F & Z80Chip::FLAG_C;
    }

    ZEMUX_FORCE_INLINE static bool cc_PO(Z80Chip* cpu) {
        return !(cpu->regs.F & Z80Chip::FLAG_PV); // parity odd
    }

    ZEMUX_FORCE_INLINE static bool cc_PE(Z80Chip* cpu) {
        return cpu->regs.F & Z80Chip::FLAG_PV; // parity even
    }

    ZEMUX_FORCE_INLINE static bool cc_P(Z80Chip* cpu) {
        return !(cpu->regs.F & Z80Chip::FLAG_S); // positive
    }

    ZEMUX_FORCE_INLINE static bool cc_M(Z80Chip* cpu) {
        return cpu->regs.F & Z80Chip::FLAG_S; // minus
    }

    // Actions (not affecting T-state)

    ZEMUX_FORCE_INLINE static void do_PREF_00(Z80Chip* cpu) {
        cpu->prefix = 0x00;
        cpu->optable = z80ChipOptable_00;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_SUB_8(Z80Chip* cpu, uint8_t x, uint8_t y) {
        uint16_t result = x - y;
        uint8_t halfResult = (x & 0x0F) - (y & 0x0F);
        int16_t signedResult = static_cast<int8_t>(x) - static_cast<int8_t>(y);

        cpu->regs.F = ((result & Z80Chip::FLAG_C_M8) >> Z80Chip::FLAG_C_S8)
                | Z80Chip::FLAG_N
                | (halfResult & Z80Chip::FLAG_H)
                | ((signedResult < -128 || signedResult > 127) ? Z80Chip::FLAG_PV : 0)
                | (result & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | ((result & 0xFF) ? 0 : Z80Chip::FLAG_Z);

        return result;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_INC_8(Z80Chip* cpu, uint8_t x) {
        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C) | (((x & 0x0F) + 1) & Z80Chip::FLAG_H);
        ++x;

        cpu->regs.F |= (x == Z80Chip::FLAG_PV_C8 ? Z80Chip::FLAG_PV : 0)
                | (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z);

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_DEC_8(Z80Chip* cpu, uint8_t x) {
        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C)
                | Z80Chip::FLAG_N
                | (((x & 0x0F) - 1) & Z80Chip::FLAG_H)
                | (x == Z80Chip::FLAG_PV_C8 ? Z80Chip::FLAG_PV : 0);

        --x;
        cpu->regs.F |= (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3)) | (x ? 0 : Z80Chip::FLAG_Z);

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_ADD_8(Z80Chip* cpu, uint8_t x, uint8_t y) {
        uint16_t result = x + y;
        uint8_t halfResult = (x & 0x0F) + (y & 0x0F);
        int16_t signedResult = static_cast<int8_t>(x) + static_cast<int8_t>(y);

        cpu->regs.F = ((result & Z80Chip::FLAG_C_M8) >> Z80Chip::FLAG_C_S8)
                | (halfResult & Z80Chip::FLAG_H)
                | ((signedResult < -128 || signedResult > 127) ? Z80Chip::FLAG_PV : 0)
                | (result & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | ((result & 0xFF) ? 0 : Z80Chip::FLAG_Z);

        return result;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_ADC_8(Z80Chip* cpu, uint8_t x, uint8_t y) {
        uint16_t result = x + y + (cpu->regs.F & Z80Chip::FLAG_C);
        uint8_t halfResult = (x & 0x0F) + (y & 0x0F) + (cpu->regs.F & Z80Chip::FLAG_C);

        int16_t signedResult = static_cast<int8_t>(x) +
                static_cast<int8_t>(y) +
                static_cast<int16_t>(cpu->regs.F & Z80Chip::FLAG_C);

        cpu->regs.F = ((result & Z80Chip::FLAG_C_M8) >> Z80Chip::FLAG_C_S8)
                | (halfResult & Z80Chip::FLAG_H)
                | ((signedResult < -128 || signedResult > 127) ? Z80Chip::FLAG_PV : 0)
                | (result & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | ((result & 0xFF) ? 0 : Z80Chip::FLAG_Z);

        return result;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_SBC_8(Z80Chip* cpu, uint8_t x, uint8_t y) {
        uint16_t result = x - y - (cpu->regs.F & Z80Chip::FLAG_C);
        uint8_t halfResult = (x & 0x0F) - (y & 0x0F) - (cpu->regs.F & Z80Chip::FLAG_C);

        int16_t signedResult = static_cast<int8_t>(x) -
                static_cast<int8_t>(y) -
                static_cast<int16_t>(cpu->regs.F & Z80Chip::FLAG_C);

        cpu->regs.F = ((result & Z80Chip::FLAG_C_M8) >> Z80Chip::FLAG_C_S8)
                | Z80Chip::FLAG_N
                | (halfResult & Z80Chip::FLAG_H)
                | ((signedResult < -128 || signedResult > 127) ? Z80Chip::FLAG_PV : 0)
                | (result & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | ((result & 0xFF) ? 0 : Z80Chip::FLAG_Z);

        return result;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_AND_8(Z80Chip* cpu, uint8_t x, uint8_t y) {
        x &= y;

        cpu->regs.F = Z80Chip::FLAG_H
                | (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_XOR_8(Z80Chip* cpu, uint8_t x, uint8_t y) {
        x ^= y;

        cpu->regs.F = (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_OR_8(Z80Chip* cpu, uint8_t x, uint8_t y) {
        x |= y;

        cpu->regs.F = (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    ZEMUX_FORCE_INLINE static uint8_t do_CP_8(Z80Chip* cpu, uint8_t x, uint8_t y) {
        uint16_t result = x - y;
        uint8_t halfResult = (x & 0x0F) - (y & 0x0F);
        int16_t signedResult = static_cast<int8_t>(x) - static_cast<int8_t>(y);

        cpu->regs.F = ((result & Z80Chip::FLAG_C_M8) >> Z80Chip::FLAG_C_S8)
                | Z80Chip::FLAG_N
                | (halfResult & Z80Chip::FLAG_H)
                | ((signedResult < -128 || signedResult > 127) ? Z80Chip::FLAG_PV : 0)
                | (y & (Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (result & Z80Chip::FLAG_S)
                | ((result & 0xFF) ? 0 : Z80Chip::FLAG_Z);

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_RLC_8(Z80Chip* cpu, uint8_t x) {
        x = (x << 1) | (x >> 7);

        cpu->regs.F = (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3 | Z80Chip::FLAG_C))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_RRC_8(Z80Chip* cpu, uint8_t x) {
        cpu->regs.F = x & Z80Chip::FLAG_C;
        x = (x >> 1) | (x << 7);

        cpu->regs.F |= (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_RL_8(Z80Chip* cpu, uint8_t x) {
        uint8_t value = x;
        x = (x << 1) | (cpu->regs.F & Z80Chip::FLAG_C);

        cpu->regs.F = (value >> 7)
                | (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_RR_8(Z80Chip* cpu, uint8_t x) {
        uint8_t value = x;
        x = (x >> 1) | (cpu->regs.F << 7);

        cpu->regs.F = (value & Z80Chip::FLAG_C)
                | (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_SLA_8(Z80Chip* cpu, uint8_t x) {
        cpu->regs.F = x >> 7;
        x <<= 1;

        cpu->regs.F |= (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_SRA_8(Z80Chip* cpu, uint8_t x) {
        cpu->regs.F = x & Z80Chip::FLAG_C;
        x = (x & 0x80) | (x >> 1);

        cpu->regs.F |= (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_SLL_8(Z80Chip* cpu, uint8_t x) {
        cpu->regs.F = x >> 7;
        x = (x << 1) | 0x01;

        cpu->regs.F |= (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_SRL_8(Z80Chip* cpu, uint8_t x) {
        cpu->regs.F = x & Z80Chip::FLAG_C;
        x >>= 1;

        cpu->regs.F |= (x & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[x];

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint16_t do_INC_16(Z80Chip*, uint16_t rp) {
        return rp + 1;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint16_t do_DEC_16(Z80Chip*, uint16_t rp) {
        return rp - 1;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint16_t do_ADD_16(Z80Chip* cpu, uint16_t x, uint16_t y) {
        uint32_t result = x + y;
        uint16_t halfResult = (x & 0x0FFF) + (y & 0x0FFF);

        cpu->regs.MP = x + 1;
        x = result;

        cpu->regs.F = (cpu->regs.F & (Z80Chip::FLAG_PV | Z80Chip::FLAG_Z | Z80Chip::FLAG_S))
                | ((result & Z80Chip::FLAG_C_M16) >> Z80Chip::FLAG_C_S16)
                | ((result >> 8) & (Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | ((halfResult & Z80Chip::FLAG_H_M16) >> Z80Chip::FLAG_H_S16);

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint16_t do_ADC_16(Z80Chip* cpu, uint16_t x, uint16_t y) {
        uint32_t result = x + y + (cpu->regs.F & Z80Chip::FLAG_C);
        uint16_t halfResult = (x & 0x0FFF) + (y & 0x0FFF) + (cpu->regs.F & Z80Chip::FLAG_C);

        int32_t signedResult = static_cast<int16_t>(x) +
                static_cast<int16_t>(y) +
                static_cast<int32_t>(cpu->regs.F & Z80Chip::FLAG_C);

        cpu->regs.MP = x + 1;
        x = result;

        cpu->regs.F = ((result & Z80Chip::FLAG_C_M16) >> Z80Chip::FLAG_C_S16)
                | ((halfResult & Z80Chip::FLAG_H_M16) >> Z80Chip::FLAG_H_S16)
                | ((signedResult < -32768 || signedResult > 32767) ? Z80Chip::FLAG_PV : 0)
                | ((result >> 8) & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (x ? 0 : Z80Chip::FLAG_Z);

        return x;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint16_t do_SBC_16(Z80Chip* cpu, uint16_t x, uint16_t y) {
        uint32_t result = x - y - (cpu->regs.F & Z80Chip::FLAG_C);
        uint16_t halfResult = (x & 0x0FFF) - (y & 0x0FFF) - (cpu->regs.F & Z80Chip::FLAG_C);

        int32_t signedResult = static_cast<int16_t>(x) -
                static_cast<int16_t>(y) -
                static_cast<int32_t>(cpu->regs.F & Z80Chip::FLAG_C);

        cpu->regs.F = ((result & Z80Chip::FLAG_C_M16) >> Z80Chip::FLAG_C_S16)
                | Z80Chip::FLAG_N
                | ((halfResult & Z80Chip::FLAG_H_M16) >> Z80Chip::FLAG_H_S16)
                | ((signedResult < -32768 || signedResult > 32767) ? Z80Chip::FLAG_PV : 0)
                | ((result >> 8) & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (static_cast<uint16_t>(result) ? 0 : Z80Chip::FLAG_Z);

        cpu->regs.MP = x + 1;
        return result;
    }

    ZEMUX_FORCE_INLINE static void do_BIT(Z80Chip* cpu, uint8_t value, int bit) {
        uint8_t result = value & (1 << bit);

        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C)
                | Z80Chip::FLAG_H
                | (result & Z80Chip::FLAG_S)
                | (result ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[result]
                | (cpu->regs.MPH & (Z80Chip::FLAG_5 | Z80Chip::FLAG_3));
    }

    ZEMUX_FORCE_INLINE static void do_REP_LD_INC(Z80Chip* cpu) {
        ++(cpu->regs.HL);
        ++(cpu->regs.DE);
    }

    ZEMUX_FORCE_INLINE static void do_REP_LD_DEC(Z80Chip* cpu) {
        --(cpu->regs.HL);
        --(cpu->regs.DE);
    }

    ZEMUX_FORCE_INLINE static void do_REP_CP_INC(Z80Chip* cpu) {
        ++(cpu->regs.HL);
        ++(cpu->regs.MP);
    }

    ZEMUX_FORCE_INLINE static void do_REP_CP_DEC(Z80Chip* cpu) {
        --(cpu->regs.HL);
        --(cpu->regs.MP);
    }

    // Actions (affecting T-state)

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint16_t do_POP(Z80Chip* cpu) {
        uint8_t value = cpu->memoryRead((cpu->regs.SP)++);
        return value | (static_cast<uint16_t>(cpu->memoryRead((cpu->regs.SP)++)) << 8);
    }

    ZEMUX_FORCE_INLINE static void do_RET(Z80Chip* cpu) {
        cpu->regs.PC = do_POP(cpu);
        cpu->regs.MP = cpu->regs.PC;
    }

    ZEMUX_FORCE_INLINE static void do_PUSH(Z80Chip* cpu, uint16_t rp) {
        cpu->memoryWrite(--(cpu->regs.SP), static_cast<uint8_t>(rp >> 8));
        cpu->memoryWrite(--(cpu->regs.SP), static_cast<uint8_t>(rp));
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE static uint8_t do_IN(Z80Chip* cpu, uint16_t port) {
        uint8_t result = cpu->ioRead(port);
        cpu->regs.MP = port + 1;
        return result;
    }

    ZEMUX_FORCE_INLINE static void do_OUT(Z80Chip* cpu, uint16_t port, uint8_t value) {
        cpu->ioWrite(port, value);
        cpu->regs.MPL = static_cast<uint8_t>(port + 1);
        cpu->regs.MPH = cpu->regs.A;
    }

    ZEMUX_FORCE_INLINE static void do_REP_LD(Z80Chip* cpu) {
        uint8_t value = cpu->memoryRead(cpu->regs.HL);
        cpu->memoryWrite(cpu->regs.DE, value);
        cpu->putAddressOnBus(cpu->regs.DE, 2);

        --(cpu->regs.BC);
        value += cpu->regs.A;

        cpu->regs.F = (cpu->regs.F & (Z80Chip::FLAG_S | Z80Chip::FLAG_Z | Z80Chip::FLAG_C))
                | (cpu->regs.BC ? Z80Chip::FLAG_PV : 0)
                | (value & Z80Chip::FLAG_3)
                | ((value << Z80Chip::FLAG_N_TO_5) & Z80Chip::FLAG_5);
    }

    ZEMUX_FORCE_INLINE static void do_REP_LD_INCR(Z80Chip* cpu) {
        if (cpu->regs.BC) {
            cpu->putAddressOnBus(cpu->regs.DE, 5);
            cpu->regs.PC -= 2;
            cpu->regs.MP = cpu->regs.PC + 1;
        }

        ++(cpu->regs.HL);
        ++(cpu->regs.DE);
    }

    ZEMUX_FORCE_INLINE static void do_REP_LD_DECR(Z80Chip* cpu) {
        if (cpu->regs.BC) {
            cpu->putAddressOnBus(cpu->regs.DE, 5);
            cpu->regs.PC -= 2;
            cpu->regs.MP = cpu->regs.PC + 1;
        }

        --(cpu->regs.HL);
        --(cpu->regs.DE);
    }

    ZEMUX_FORCE_INLINE static void do_REP_CP(Z80Chip* cpu) {
        uint8_t value = cpu->memoryRead(cpu->regs.HL);
        cpu->putAddressOnBus(cpu->regs.HL, 5);

        uint8_t halfResult = (cpu->regs.A & 0x0F) - (value & 0x0F);
        value = cpu->regs.A - value;
        --(cpu->regs.BC);

        cpu->regs.F = (cpu->regs.F & Z80Chip::FLAG_C)
                | Z80Chip::FLAG_N
                | (cpu->regs.BC ? Z80Chip::FLAG_PV : 0)
                | (halfResult & Z80Chip::FLAG_H)
                | (value ? 0 : Z80Chip::FLAG_Z)
                | (value & Z80Chip::FLAG_S);

        value -= ((cpu->regs.F & Z80Chip::FLAG_H) >> Z80Chip::FLAG_H_TO_C);
        cpu->regs.F |= (value & Z80Chip::FLAG_3) | ((value << Z80Chip::FLAG_N_TO_5) & Z80Chip::FLAG_5);
    }

    ZEMUX_FORCE_INLINE static void do_REP_CP_INCR(Z80Chip* cpu) {
        if ((cpu->regs.F & (Z80Chip::FLAG_Z | Z80Chip::FLAG_PV)) == Z80Chip::FLAG_PV) {
            cpu->putAddressOnBus(cpu->regs.HL, 5);
            cpu->regs.PC -= 2;
            cpu->regs.MP = cpu->regs.PC + 1;
        } else {
            ++(cpu->regs.MP);
        }

        ++(cpu->regs.HL);
    }

    ZEMUX_FORCE_INLINE static void do_REP_CP_DECR(Z80Chip* cpu) {
        if ((cpu->regs.F & (Z80Chip::FLAG_Z | Z80Chip::FLAG_PV)) == Z80Chip::FLAG_PV) {
            cpu->putAddressOnBus(cpu->regs.HL, 5);
            cpu->regs.PC -= 2;
            cpu->regs.MP = cpu->regs.PC + 1;
        } else {
            --(cpu->regs.MP);
        }

        --(cpu->regs.HL);
    }

    ZEMUX_FORCE_INLINE static void do_REP_INI(Z80Chip* cpu) {
        cpu->putAddressOnBus(cpu->regs.IR, 1);
        uint8_t byteValue = cpu->ioRead(cpu->regs.BC);
        cpu->memoryWrite(cpu->regs.HL, byteValue);

        cpu->regs.MP = cpu->regs.BC + 1;
        --(cpu->regs.B);
        uint16_t wordValue = byteValue + static_cast<uint8_t>(cpu->regs.C + 1);

        cpu->regs.F = ((byteValue >> Z80Chip::FLAG_S_TO_N) & Z80Chip::FLAG_N)
                | (cpu->regs.B & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (cpu->regs.B ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[(wordValue & 0x07) ^ cpu->regs.B]
                | ((wordValue > 255) ? (Z80Chip::FLAG_C | Z80Chip::FLAG_H) : 0);
    }

    ZEMUX_FORCE_INLINE static void do_REP_IND(Z80Chip* cpu) {
        cpu->putAddressOnBus(cpu->regs.IR, 1);
        uint8_t byteValue = cpu->ioRead(cpu->regs.BC);
        cpu->memoryWrite(cpu->regs.HL, byteValue);

        cpu->regs.MP = cpu->regs.BC - 1;
        --(cpu->regs.B);
        uint16_t wordValue = byteValue + static_cast<uint8_t>(cpu->regs.C - 1);

        cpu->regs.F = ((byteValue >> Z80Chip::FLAG_S_TO_N) & Z80Chip::FLAG_N)
                | (cpu->regs.B & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (cpu->regs.B ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[(wordValue & 0x07) ^ cpu->regs.B]
                | ((wordValue > 255) ? (Z80Chip::FLAG_C | Z80Chip::FLAG_H) : 0);
    }

    ZEMUX_FORCE_INLINE static void do_REP_IN_INCR(Z80Chip* cpu) {
        if (cpu->regs.B) {
            cpu->putAddressOnBus(cpu->regs.HL, 5);
            cpu->regs.PC -= 2;
        }

        ++(cpu->regs.HL);
    }

    ZEMUX_FORCE_INLINE static void do_REP_IN_DECR(Z80Chip* cpu) {
        if (cpu->regs.B) {
            cpu->putAddressOnBus(cpu->regs.HL, 5);
            cpu->regs.PC -= 2;
        }

        --(cpu->regs.HL);
    }

    ZEMUX_FORCE_INLINE static void do_REP_OUTI(Z80Chip* cpu) {
        cpu->putAddressOnBus(cpu->regs.IR, 1);
        uint8_t byteValue = cpu->memoryRead(cpu->regs.HL);

        --(cpu->regs.B);
        cpu->regs.MP = cpu->regs.BC + 1;
        cpu->ioWrite(cpu->regs.BC, byteValue);

        ++(cpu->regs.HL);
        uint16_t wordValue = byteValue + cpu->regs.L;

        cpu->regs.F = ((byteValue >> Z80Chip::FLAG_S_TO_N) & Z80Chip::FLAG_N)
                | (cpu->regs.B & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (cpu->regs.B ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[(wordValue & 0x07) ^ cpu->regs.B]
                | ((wordValue > 255) ? (Z80Chip::FLAG_C | Z80Chip::FLAG_H) : 0);
    }

    ZEMUX_FORCE_INLINE static void do_REP_OUTD(Z80Chip* cpu) {
        cpu->putAddressOnBus(cpu->regs.IR, 1);
        uint8_t byteValue = cpu->memoryRead(cpu->regs.HL);

        --(cpu->regs.B);
        cpu->regs.MP = cpu->regs.BC - 1;
        cpu->ioWrite(cpu->regs.BC, byteValue);

        --(cpu->regs.HL);
        uint16_t wordValue = byteValue + cpu->regs.L;

        cpu->regs.F = ((byteValue >> Z80Chip::FLAG_S_TO_N) & Z80Chip::FLAG_N)
                | (cpu->regs.B & (Z80Chip::FLAG_S | Z80Chip::FLAG_5 | Z80Chip::FLAG_3))
                | (cpu->regs.B ? 0 : Z80Chip::FLAG_Z)
                | Z80Chip::parityLookup[(wordValue & 0x07) ^ cpu->regs.B]
                | ((wordValue > 255) ? (Z80Chip::FLAG_C | Z80Chip::FLAG_H) : 0);
    }

    ZEMUX_FORCE_INLINE static void do_REP_OUT_INCR_DECR(Z80Chip* cpu) {
        if (cpu->regs.B) {
            cpu->putAddressOnBus(cpu->regs.BC, 5);
            cpu->regs.PC -= 2;
        }
    }
};

}

#pragma clang diagnostic pop
#endif
