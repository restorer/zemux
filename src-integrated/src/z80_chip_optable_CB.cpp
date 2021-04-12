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

namespace zemux {

//
// 0x0n
//

// RLC B
static void op_CB_00(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RLC C
static void op_CB_01(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RLC D
static void op_CB_02(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RLC E
static void op_CB_03(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RLC H
static void op_CB_04(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RLC L
static void op_CB_05(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RLC (HL)
static void op_CB_06(Z80Chip* cpu) {
    Z80ChipCore::op_DO_MHL(cpu, Z80ChipCore::do_RLC_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// RLC A
static void op_CB_07(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// RRC B
static void op_CB_08(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RRC C
static void op_CB_09(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RRC D
static void op_CB_0A(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RRC E
static void op_CB_0B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RRC H
static void op_CB_0C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RRC L
static void op_CB_0D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RRC (HL)
static void op_CB_0E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_MHL(cpu, Z80ChipCore::do_RRC_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// RRC A
static void op_CB_0F(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x1n
//

// RL B
static void op_CB_10(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RL C
static void op_CB_11(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RL D
static void op_CB_12(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RL E
static void op_CB_13(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RL H
static void op_CB_14(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RL L
static void op_CB_15(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RL (HL)
static void op_CB_16(Z80Chip* cpu) {
    Z80ChipCore::op_DO_MHL(cpu, Z80ChipCore::do_RL_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// RL A
static void op_CB_17(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// RR B
static void op_CB_18(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RR C
static void op_CB_19(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RR D
static void op_CB_1A(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RR E
static void op_CB_1B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RR H
static void op_CB_1C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RR L
static void op_CB_1D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RR (HL)
static void op_CB_1E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_MHL(cpu, Z80ChipCore::do_RR_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// RR A
static void op_CB_1F(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x2n
//

// SLA B
static void op_CB_20(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLA C
static void op_CB_21(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLA D
static void op_CB_22(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLA E
static void op_CB_23(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLA H
static void op_CB_24(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLA L
static void op_CB_25(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLA (HL)
static void op_CB_26(Z80Chip* cpu) {
    Z80ChipCore::op_DO_MHL(cpu, Z80ChipCore::do_SLA_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// SLA A
static void op_CB_27(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRA B
static void op_CB_28(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRA C
static void op_CB_29(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRA D
static void op_CB_2A(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRA E
static void op_CB_2B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRA H
static void op_CB_2C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRA L
static void op_CB_2D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRA (HL)
static void op_CB_2E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_MHL(cpu, Z80ChipCore::do_SRA_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// SRA A
static void op_CB_2F(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x3n
//

// SLL B
static void op_CB_30(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLL C
static void op_CB_31(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLL D
static void op_CB_32(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLL E
static void op_CB_33(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLL H
static void op_CB_34(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLL L
static void op_CB_35(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SLL (HL)
static void op_CB_36(Z80Chip* cpu) {
    Z80ChipCore::op_DO_MHL(cpu, Z80ChipCore::do_SLL_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// SLL A
static void op_CB_37(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRL B
static void op_CB_38(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRL C
static void op_CB_39(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRL D
static void op_CB_3A(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRL E
static void op_CB_3B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRL H
static void op_CB_3C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRL L
static void op_CB_3D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SRL (HL)
static void op_CB_3E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_MHL(cpu, Z80ChipCore::do_SRL_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// SRL A
static void op_CB_3F(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x4n
//

// BIT 0,B
static void op_CB_40(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 0, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 0,C
static void op_CB_41(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 0, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 0,D
static void op_CB_42(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 0, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 0,E
static void op_CB_43(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 0, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 0,H
static void op_CB_44(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 0, cpu->regs.H);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 0,L
static void op_CB_45(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 0, cpu->regs.L);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 0,(HL)
static void op_CB_46(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_MHL(cpu, 0);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 0,A
static void op_CB_47(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 0, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 1,B
static void op_CB_48(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 1, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 1,C
static void op_CB_49(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 1, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 1,D
static void op_CB_4A(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 1, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 1,E
static void op_CB_4B(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 1, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 1,H
static void op_CB_4C(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 1, cpu->regs.H);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 1,L
static void op_CB_4D(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 1, cpu->regs.L);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 1,(HL)
static void op_CB_4E(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_MHL(cpu, 1);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 1,A
static void op_CB_4F(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 1, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x5n
//

// BIT 2,B
static void op_CB_50(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 2, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 2,C
static void op_CB_51(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 2, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 2,D
static void op_CB_52(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 2, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 2,E
static void op_CB_53(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 2, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 2,H
static void op_CB_54(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 2, cpu->regs.H);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 2,L
static void op_CB_55(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 2, cpu->regs.L);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 2,(HL)
static void op_CB_56(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_MHL(cpu, 2);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 2,A
static void op_CB_57(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 2, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 3,B
static void op_CB_58(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 3, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 3,C
static void op_CB_59(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 3, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 3,D
static void op_CB_5A(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 3, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 3,E
static void op_CB_5B(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 3, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 3,H
static void op_CB_5C(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 3, cpu->regs.H);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 3,L
static void op_CB_5D(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 3, cpu->regs.L);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 3,(HL)
static void op_CB_5E(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_MHL(cpu, 3);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 3,A
static void op_CB_5F(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 3, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x6n
//

// BIT 4,B
static void op_CB_60(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 4, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 4,C
static void op_CB_61(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 4, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 4,D
static void op_CB_62(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 4, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 4,E
static void op_CB_63(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 4, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 4,H
static void op_CB_64(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 4, cpu->regs.H);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 4,L
static void op_CB_65(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 4, cpu->regs.L);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 4,(HL)
static void op_CB_66(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_MHL(cpu, 4);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 4,A
static void op_CB_67(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 4, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 5,B
static void op_CB_68(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 5, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 5,C
static void op_CB_69(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 5, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 5,D
static void op_CB_6A(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 5, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 5,E
static void op_CB_6B(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 5, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 5,H
static void op_CB_6C(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 5, cpu->regs.H);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 5,L
static void op_CB_6D(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 5, cpu->regs.L);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 5,(HL)
static void op_CB_6E(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_MHL(cpu, 5);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 5,A
static void op_CB_6F(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 5, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x7n
//

// BIT 6,B
static void op_CB_70(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 6, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 6,C
static void op_CB_71(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 6, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 6,D
static void op_CB_72(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 6, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 6,E
static void op_CB_73(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 6, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 6,H
static void op_CB_74(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 6, cpu->regs.H);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 6,L
static void op_CB_75(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 6, cpu->regs.L);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 6,(HL)
static void op_CB_76(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_MHL(cpu, 6);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 6,A
static void op_CB_77(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 6, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 7,B
static void op_CB_78(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 7, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 7,C
static void op_CB_79(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 7, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 7,D
static void op_CB_7A(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 7, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 7,E
static void op_CB_7B(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 7, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 7,H
static void op_CB_7C(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 7, cpu->regs.H);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 7,L
static void op_CB_7D(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 7, cpu->regs.L);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 7,(HL)
static void op_CB_7E(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_MHL(cpu, 7);
    Z80ChipCore::do_PREF_00(cpu);
}

// BIT 7,A
static void op_CB_7F(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_R(cpu, 7, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x8n
//

// RES 0,B
static void op_CB_80(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 0, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 0,C
static void op_CB_81(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 0, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 0,D
static void op_CB_82(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 0, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 0,E
static void op_CB_83(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 0, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 0,H
static void op_CB_84(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 0, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 0,L
static void op_CB_85(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 0, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 0,(HL)
static void op_CB_86(Z80Chip* cpu) {
    Z80ChipCore::op_RES_MHL(cpu, 0);
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 0,A
static void op_CB_87(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 0, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 1,B
static void op_CB_88(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 1, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 1,C
static void op_CB_89(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 1, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 1,D
static void op_CB_8A(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 1, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 1,E
static void op_CB_8B(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 1, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 1,H
static void op_CB_8C(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 1, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 1,L
static void op_CB_8D(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 1, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 1,(HL)
static void op_CB_8E(Z80Chip* cpu) {
    Z80ChipCore::op_RES_MHL(cpu, 1);
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 1,A
static void op_CB_8F(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 1, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x9n
//

// RES 2,B
static void op_CB_90(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 2, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 2,C
static void op_CB_91(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 2, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 2,D
static void op_CB_92(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 2, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 2,E
static void op_CB_93(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 2, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 2,H
static void op_CB_94(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 2, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 2,L
static void op_CB_95(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 2, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 2,(HL)
static void op_CB_96(Z80Chip* cpu) {
    Z80ChipCore::op_RES_MHL(cpu, 2);
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 2,A
static void op_CB_97(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 2, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 3,B
static void op_CB_98(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 3, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 3,C
static void op_CB_99(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 3, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 3,D
static void op_CB_9A(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 3, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 3,E
static void op_CB_9B(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 3, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 3,H
static void op_CB_9C(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 3, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 3,L
static void op_CB_9D(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 3, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 3,(HL)
static void op_CB_9E(Z80Chip* cpu) {
    Z80ChipCore::op_RES_MHL(cpu, 3);
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 3,A
static void op_CB_9F(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 3, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xAn
//

// RES 4,B
static void op_CB_A0(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 4, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 4,C
static void op_CB_A1(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 4, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 4,D
static void op_CB_A2(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 4, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 4,E
static void op_CB_A3(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 4, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 4,H
static void op_CB_A4(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 4, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 4,L
static void op_CB_A5(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 4, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 4,(HL)
static void op_CB_A6(Z80Chip* cpu) {
    Z80ChipCore::op_RES_MHL(cpu, 4);
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 4,A
static void op_CB_A7(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 4, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 5,B
static void op_CB_A8(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 5, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 5,C
static void op_CB_A9(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 5, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 5,D
static void op_CB_AA(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 5, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 5,E
static void op_CB_AB(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 5, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 5,H
static void op_CB_AC(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 5, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 5,L
static void op_CB_AD(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 5, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 5,(HL)
static void op_CB_AE(Z80Chip* cpu) {
    Z80ChipCore::op_RES_MHL(cpu, 5);
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 5,A
static void op_CB_AF(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 5, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xBn
//

// RES 6,B
static void op_CB_B0(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 6, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 6,C
static void op_CB_B1(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 6, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 6,D
static void op_CB_B2(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 6, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 6,E
static void op_CB_B3(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 6, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 6,H
static void op_CB_B4(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 6, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 6,L
static void op_CB_B5(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 6, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 6,(HL)
static void op_CB_B6(Z80Chip* cpu) {
    Z80ChipCore::op_RES_MHL(cpu, 6);
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 6,A
static void op_CB_B7(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 6, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 7,B
static void op_CB_B8(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 7, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 7,C
static void op_CB_B9(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 7, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 7,D
static void op_CB_BA(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 7, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 7,E
static void op_CB_BB(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 7, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 7,H
static void op_CB_BC(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 7, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 7,L
static void op_CB_BD(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 7, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 7,(HL)
static void op_CB_BE(Z80Chip* cpu) {
    Z80ChipCore::op_RES_MHL(cpu, 7);
    Z80ChipCore::do_PREF_00(cpu);
}

// RES 7,A
static void op_CB_BF(Z80Chip* cpu) {
    Z80ChipCore::op_RES_R(cpu, 7, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xCn
//

// SET 0,B
static void op_CB_C0(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 0, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 0,C
static void op_CB_C1(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 0, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 0,D
static void op_CB_C2(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 0, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 0,E
static void op_CB_C3(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 0, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 0,H
static void op_CB_C4(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 0, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 0,L
static void op_CB_C5(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 0, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 0,(HL)
static void op_CB_C6(Z80Chip* cpu) {
    Z80ChipCore::op_SET_MHL(cpu, 0);
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 0,A
static void op_CB_C7(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 0, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 1,B
static void op_CB_C8(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 1, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 1,C
static void op_CB_C9(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 1, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 1,D
static void op_CB_CA(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 1, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 1,E
static void op_CB_CB(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 1, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 1,H
static void op_CB_CC(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 1, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 1,L
static void op_CB_CD(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 1, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 1,(HL)
static void op_CB_CE(Z80Chip* cpu) {
    Z80ChipCore::op_SET_MHL(cpu, 1);
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 1,A
static void op_CB_CF(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 1, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xDn
//

// SET 2,B
static void op_CB_D0(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 2, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 2,C
static void op_CB_D1(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 2, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 2,D
static void op_CB_D2(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 2, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 2,E
static void op_CB_D3(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 2, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 2,H
static void op_CB_D4(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 2, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 2,L
static void op_CB_D5(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 2, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 2,(HL)
static void op_CB_D6(Z80Chip* cpu) {
    Z80ChipCore::op_SET_MHL(cpu, 2);
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 2,A
static void op_CB_D7(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 2, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 3,B
static void op_CB_D8(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 3, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 3,C
static void op_CB_D9(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 3, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 3,D
static void op_CB_DA(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 3, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 3,E
static void op_CB_DB(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 3, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 3,H
static void op_CB_DC(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 3, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 3,L
static void op_CB_DD(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 3, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 3,(HL)
static void op_CB_DE(Z80Chip* cpu) {
    Z80ChipCore::op_SET_MHL(cpu, 3);
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 3,A
static void op_CB_DF(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 3, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xEn
//

// SET 4,B
static void op_CB_E0(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 4, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 4,C
static void op_CB_E1(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 4, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 4,D
static void op_CB_E2(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 4, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 4,E
static void op_CB_E3(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 4, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 4,H
static void op_CB_E4(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 4, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 4,L
static void op_CB_E5(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 4, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 4,(HL)
static void op_CB_E6(Z80Chip* cpu) {
    Z80ChipCore::op_SET_MHL(cpu, 4);
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 4,A
static void op_CB_E7(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 4, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 5,B
static void op_CB_E8(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 5, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 5,C
static void op_CB_E9(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 5, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 5,D
static void op_CB_EA(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 5, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 5,E
static void op_CB_EB(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 5, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 5,H
static void op_CB_EC(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 5, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 5,L
static void op_CB_ED(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 5, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 5,(HL)
static void op_CB_EE(Z80Chip* cpu) {
    Z80ChipCore::op_SET_MHL(cpu, 5);
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 5,A
static void op_CB_EF(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 5, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xFn
//

// SET 6,B
static void op_CB_F0(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 6, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 6,C
static void op_CB_F1(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 6, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 6,D
static void op_CB_F2(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 6, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 6,E
static void op_CB_F3(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 6, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 6,H
static void op_CB_F4(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 6, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 6,L
static void op_CB_F5(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 6, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 6,(HL)
static void op_CB_F6(Z80Chip* cpu) {
    Z80ChipCore::op_SET_MHL(cpu, 6);
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 6,A
static void op_CB_F7(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 6, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 7,B
static void op_CB_F8(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 7, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 7,C
static void op_CB_F9(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 7, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 7,D
static void op_CB_FA(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 7, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 7,E
static void op_CB_FB(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 7, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 7,H
static void op_CB_FC(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 7, &(cpu->regs.H));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 7,L
static void op_CB_FD(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 7, &(cpu->regs.L));
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 7,(HL)
static void op_CB_FE(Z80Chip* cpu) {
    Z80ChipCore::op_SET_MHL(cpu, 7);
    Z80ChipCore::do_PREF_00(cpu);
}

// SET 7,A
static void op_CB_FF(Z80Chip* cpu) {
    Z80ChipCore::op_SET_R(cpu, 7, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

Z80Chip::Opcode z80ChipOptable_CB[0x100] = {
        op_CB_00, op_CB_01, op_CB_02, op_CB_03, op_CB_04, op_CB_05, op_CB_06, op_CB_07,
        op_CB_08, op_CB_09, op_CB_0A, op_CB_0B, op_CB_0C, op_CB_0D, op_CB_0E, op_CB_0F,
        op_CB_10, op_CB_11, op_CB_12, op_CB_13, op_CB_14, op_CB_15, op_CB_16, op_CB_17,
        op_CB_18, op_CB_19, op_CB_1A, op_CB_1B, op_CB_1C, op_CB_1D, op_CB_1E, op_CB_1F,
        op_CB_20, op_CB_21, op_CB_22, op_CB_23, op_CB_24, op_CB_25, op_CB_26, op_CB_27,
        op_CB_28, op_CB_29, op_CB_2A, op_CB_2B, op_CB_2C, op_CB_2D, op_CB_2E, op_CB_2F,
        op_CB_30, op_CB_31, op_CB_32, op_CB_33, op_CB_34, op_CB_35, op_CB_36, op_CB_37,
        op_CB_38, op_CB_39, op_CB_3A, op_CB_3B, op_CB_3C, op_CB_3D, op_CB_3E, op_CB_3F,
        op_CB_40, op_CB_41, op_CB_42, op_CB_43, op_CB_44, op_CB_45, op_CB_46, op_CB_47,
        op_CB_48, op_CB_49, op_CB_4A, op_CB_4B, op_CB_4C, op_CB_4D, op_CB_4E, op_CB_4F,
        op_CB_50, op_CB_51, op_CB_52, op_CB_53, op_CB_54, op_CB_55, op_CB_56, op_CB_57,
        op_CB_58, op_CB_59, op_CB_5A, op_CB_5B, op_CB_5C, op_CB_5D, op_CB_5E, op_CB_5F,
        op_CB_60, op_CB_61, op_CB_62, op_CB_63, op_CB_64, op_CB_65, op_CB_66, op_CB_67,
        op_CB_68, op_CB_69, op_CB_6A, op_CB_6B, op_CB_6C, op_CB_6D, op_CB_6E, op_CB_6F,
        op_CB_70, op_CB_71, op_CB_72, op_CB_73, op_CB_74, op_CB_75, op_CB_76, op_CB_77,
        op_CB_78, op_CB_79, op_CB_7A, op_CB_7B, op_CB_7C, op_CB_7D, op_CB_7E, op_CB_7F,
        op_CB_80, op_CB_81, op_CB_82, op_CB_83, op_CB_84, op_CB_85, op_CB_86, op_CB_87,
        op_CB_88, op_CB_89, op_CB_8A, op_CB_8B, op_CB_8C, op_CB_8D, op_CB_8E, op_CB_8F,
        op_CB_90, op_CB_91, op_CB_92, op_CB_93, op_CB_94, op_CB_95, op_CB_96, op_CB_97,
        op_CB_98, op_CB_99, op_CB_9A, op_CB_9B, op_CB_9C, op_CB_9D, op_CB_9E, op_CB_9F,
        op_CB_A0, op_CB_A1, op_CB_A2, op_CB_A3, op_CB_A4, op_CB_A5, op_CB_A6, op_CB_A7,
        op_CB_A8, op_CB_A9, op_CB_AA, op_CB_AB, op_CB_AC, op_CB_AD, op_CB_AE, op_CB_AF,
        op_CB_B0, op_CB_B1, op_CB_B2, op_CB_B3, op_CB_B4, op_CB_B5, op_CB_B6, op_CB_B7,
        op_CB_B8, op_CB_B9, op_CB_BA, op_CB_BB, op_CB_BC, op_CB_BD, op_CB_BE, op_CB_BF,
        op_CB_C0, op_CB_C1, op_CB_C2, op_CB_C3, op_CB_C4, op_CB_C5, op_CB_C6, op_CB_C7,
        op_CB_C8, op_CB_C9, op_CB_CA, op_CB_CB, op_CB_CC, op_CB_CD, op_CB_CE, op_CB_CF,
        op_CB_D0, op_CB_D1, op_CB_D2, op_CB_D3, op_CB_D4, op_CB_D5, op_CB_D6, op_CB_D7,
        op_CB_D8, op_CB_D9, op_CB_DA, op_CB_DB, op_CB_DC, op_CB_DD, op_CB_DE, op_CB_DF,
        op_CB_E0, op_CB_E1, op_CB_E2, op_CB_E3, op_CB_E4, op_CB_E5, op_CB_E6, op_CB_E7,
        op_CB_E8, op_CB_E9, op_CB_EA, op_CB_EB, op_CB_EC, op_CB_ED, op_CB_EE, op_CB_EF,
        op_CB_F0, op_CB_F1, op_CB_F2, op_CB_F3, op_CB_F4, op_CB_F5, op_CB_F6, op_CB_F7,
        op_CB_F8, op_CB_F9, op_CB_FA, op_CB_FB, op_CB_FC, op_CB_FD, op_CB_FE, op_CB_FF };

}
