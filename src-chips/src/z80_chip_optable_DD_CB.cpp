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

#include "z80_chip.h"
#include "z80_chip_core.h"

namespace zemux {

//
// 0x0n
//

// LD B,RLC (IX+N)
static void op_DD_CB_00(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RLC (IX+N)
static void op_DD_CB_01(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RLC (IX+N)
static void op_DD_CB_02(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RLC (IX+N)
static void op_DD_CB_03(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RLC (IX+N)
static void op_DD_CB_04(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RLC (IX+N)
static void op_DD_CB_05(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.L), cpu->regs.IX);
}

// RLC (IX+N)
static void op_DD_CB_06(Z80Chip* cpu) {
    Z80ChipCore::op_DO_PORP_P00(cpu, Z80ChipCore::do_RLC_8, cpu->regs.IX);
}

// LD A,RLC (IX+N)
static void op_DD_CB_07(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RLC_8, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,RRC (IX+N)
static void op_DD_CB_08(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RRC (IX+N)
static void op_DD_CB_09(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RRC (IX+N)
static void op_DD_CB_0A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RRC (IX+N)
static void op_DD_CB_0B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RRC (IX+N)
static void op_DD_CB_0C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RRC (IX+N)
static void op_DD_CB_0D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.L), cpu->regs.IX);
}

// RRC (IX+N)
static void op_DD_CB_0E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_PORP_P00(cpu, Z80ChipCore::do_RRC_8, cpu->regs.IX);
}

// LD A,RRC (IX+N)
static void op_DD_CB_0F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RRC_8, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0x1n
//

// LD B,RL (IX+N)
static void op_DD_CB_10(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RL (IX+N)
static void op_DD_CB_11(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RL (IX+N)
static void op_DD_CB_12(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RL (IX+N)
static void op_DD_CB_13(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RL (IX+N)
static void op_DD_CB_14(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RL (IX+N)
static void op_DD_CB_15(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.L), cpu->regs.IX);
}

// RL (IX+N)
static void op_DD_CB_16(Z80Chip* cpu) {
    Z80ChipCore::op_DO_PORP_P00(cpu, Z80ChipCore::do_RL_8, cpu->regs.IX);
}

// LD A,RL (IX+N)
static void op_DD_CB_17(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RL_8, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,RR (IX+N)
static void op_DD_CB_18(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RR (IX+N)
static void op_DD_CB_19(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RR (IX+N)
static void op_DD_CB_1A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RR (IX+N)
static void op_DD_CB_1B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RR (IX+N)
static void op_DD_CB_1C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RR (IX+N)
static void op_DD_CB_1D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.L), cpu->regs.IX);
}

// RR (IX+N)
static void op_DD_CB_1E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_PORP_P00(cpu, Z80ChipCore::do_RR_8, cpu->regs.IX);
}

// LD A,RR (IX+N)
static void op_DD_CB_1F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_RR_8, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0x2n
//

// LD B,SLA (IX+N)
static void op_DD_CB_20(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SLA (IX+N)
static void op_DD_CB_21(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SLA (IX+N)
static void op_DD_CB_22(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SLA (IX+N)
static void op_DD_CB_23(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SLA (IX+N)
static void op_DD_CB_24(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SLA (IX+N)
static void op_DD_CB_25(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.L), cpu->regs.IX);
}

// SLA (IX+N)
static void op_DD_CB_26(Z80Chip* cpu) {
    Z80ChipCore::op_DO_PORP_P00(cpu, Z80ChipCore::do_SLA_8, cpu->regs.IX);
}

// LD A,SLA (IX+N)
static void op_DD_CB_27(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLA_8, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,SRA (IX+N)
static void op_DD_CB_28(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SRA (IX+N)
static void op_DD_CB_29(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SRA (IX+N)
static void op_DD_CB_2A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SRA (IX+N)
static void op_DD_CB_2B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SRA (IX+N)
static void op_DD_CB_2C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SRA (IX+N)
static void op_DD_CB_2D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.L), cpu->regs.IX);
}

// SRA (IX+N)
static void op_DD_CB_2E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_PORP_P00(cpu, Z80ChipCore::do_SRA_8, cpu->regs.IX);
}

// LD A,SRA (IX+N)
static void op_DD_CB_2F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRA_8, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0x3n
//

// LD B,SLL (IX+N)
static void op_DD_CB_30(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SLL (IX+N)
static void op_DD_CB_31(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SLL (IX+N)
static void op_DD_CB_32(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SLL (IX+N)
static void op_DD_CB_33(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SLL (IX+N)
static void op_DD_CB_34(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SLL (IX+N)
static void op_DD_CB_35(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.L), cpu->regs.IX);
}

// SLL (IX+N)
static void op_DD_CB_36(Z80Chip* cpu) {
    Z80ChipCore::op_DO_PORP_P00(cpu, Z80ChipCore::do_SLL_8, cpu->regs.IX);
}

// LD A,SLL (IX+N)
static void op_DD_CB_37(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SLL_8, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,SRL (IX+N)
static void op_DD_CB_38(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SRL (IX+N)
static void op_DD_CB_39(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SRL (IX+N)
static void op_DD_CB_3A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SRL (IX+N)
static void op_DD_CB_3B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SRL (IX+N)
static void op_DD_CB_3C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SRL (IX+N)
static void op_DD_CB_3D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.L), cpu->regs.IX);
}

// SRL (IX+N)
static void op_DD_CB_3E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_PORP_P00(cpu, Z80ChipCore::do_SRL_8, cpu->regs.IX);
}

// LD A,SRL (IX+N)
static void op_DD_CB_3F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_DO_R_PORP_P00(cpu, Z80ChipCore::do_SRL_8, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0x4n
//

#define op_DD_CB_40 op_DD_CB_46
#define op_DD_CB_41 op_DD_CB_46
#define op_DD_CB_42 op_DD_CB_46
#define op_DD_CB_43 op_DD_CB_46
#define op_DD_CB_44 op_DD_CB_46
#define op_DD_CB_45 op_DD_CB_46

// BIT 0,(IX+N)
static void op_DD_CB_46(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_PORP_P00(cpu, 0, cpu->regs.IX);
}

#define op_DD_CB_47 op_DD_CB_46
#define op_DD_CB_48 op_DD_CB_4E
#define op_DD_CB_49 op_DD_CB_4E
#define op_DD_CB_4A op_DD_CB_4E
#define op_DD_CB_4B op_DD_CB_4E
#define op_DD_CB_4C op_DD_CB_4E
#define op_DD_CB_4D op_DD_CB_4E

// BIT 1,(IX+N)
static void op_DD_CB_4E(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_PORP_P00(cpu, 1, cpu->regs.IX);
}

#define op_DD_CB_4F op_DD_CB_4E

//
// 0x5n
//

#define op_DD_CB_50 op_DD_CB_56
#define op_DD_CB_51 op_DD_CB_56
#define op_DD_CB_52 op_DD_CB_56
#define op_DD_CB_53 op_DD_CB_56
#define op_DD_CB_54 op_DD_CB_56
#define op_DD_CB_55 op_DD_CB_56

// BIT 2,(IX+N)
static void op_DD_CB_56(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_PORP_P00(cpu, 2, cpu->regs.IX);
}

#define op_DD_CB_57 op_DD_CB_56
#define op_DD_CB_58 op_DD_CB_5E
#define op_DD_CB_59 op_DD_CB_5E
#define op_DD_CB_5A op_DD_CB_5E
#define op_DD_CB_5B op_DD_CB_5E
#define op_DD_CB_5C op_DD_CB_5E
#define op_DD_CB_5D op_DD_CB_5E

// BIT 3,(IX+N)
static void op_DD_CB_5E(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_PORP_P00(cpu, 3, cpu->regs.IX);
}

#define op_DD_CB_5F op_DD_CB_5E

//
// 0x6n
//

#define op_DD_CB_60 op_DD_CB_66
#define op_DD_CB_61 op_DD_CB_66
#define op_DD_CB_62 op_DD_CB_66
#define op_DD_CB_63 op_DD_CB_66
#define op_DD_CB_64 op_DD_CB_66
#define op_DD_CB_65 op_DD_CB_66

// BIT 4,(IX+N)
static void op_DD_CB_66(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_PORP_P00(cpu, 4, cpu->regs.IX);
}

#define op_DD_CB_67 op_DD_CB_66
#define op_DD_CB_68 op_DD_CB_6E
#define op_DD_CB_69 op_DD_CB_6E
#define op_DD_CB_6A op_DD_CB_6E
#define op_DD_CB_6B op_DD_CB_6E
#define op_DD_CB_6C op_DD_CB_6E
#define op_DD_CB_6D op_DD_CB_6E

// BIT 5,(IX+N)
static void op_DD_CB_6E(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_PORP_P00(cpu, 5, cpu->regs.IX);
}

#define op_DD_CB_6F op_DD_CB_6E

//
// 0x7n
//

#define op_DD_CB_70 op_DD_CB_76
#define op_DD_CB_71 op_DD_CB_76
#define op_DD_CB_72 op_DD_CB_76
#define op_DD_CB_73 op_DD_CB_76
#define op_DD_CB_74 op_DD_CB_76
#define op_DD_CB_75 op_DD_CB_76

// BIT 6,(IX+N)
static void op_DD_CB_76(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_PORP_P00(cpu, 6, cpu->regs.IX);
}

#define op_DD_CB_77 op_DD_CB_76
#define op_DD_CB_78 op_DD_CB_7E
#define op_DD_CB_79 op_DD_CB_7E
#define op_DD_CB_7A op_DD_CB_7E
#define op_DD_CB_7B op_DD_CB_7E
#define op_DD_CB_7C op_DD_CB_7E
#define op_DD_CB_7D op_DD_CB_7E

// BIT 7,(IX+N)
static void op_DD_CB_7E(Z80Chip* cpu) {
    Z80ChipCore::op_BIT_PORP_P00(cpu, 7, cpu->regs.IX);
}

#define op_DD_CB_7F op_DD_CB_7E

//
// 0x8n
//

// LD B,RES 0,(IX+N)
static void op_DD_CB_80(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RES 0,(IX+N)
static void op_DD_CB_81(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RES 0,(IX+N)
static void op_DD_CB_82(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RES 0,(IX+N)
static void op_DD_CB_83(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RES 0,(IX+N)
static void op_DD_CB_84(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RES 0,(IX+N)
static void op_DD_CB_85(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.L), cpu->regs.IX);
}

// RES 0,(IX+N)
static void op_DD_CB_86(Z80Chip* cpu) {
    Z80ChipCore::op_RES_PORP_P00(cpu, 0, cpu->regs.IX);
}

// LD A,RES 0,(IX+N)
static void op_DD_CB_87(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,RES 1,(IX+N)
static void op_DD_CB_88(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RES 1,(IX+N)
static void op_DD_CB_89(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RES 1,(IX+N)
static void op_DD_CB_8A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RES 1,(IX+N)
static void op_DD_CB_8B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RES 1,(IX+N)
static void op_DD_CB_8C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RES 1,(IX+N)
static void op_DD_CB_8D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.L), cpu->regs.IX);
}

// RES 1,(IX+N)
static void op_DD_CB_8E(Z80Chip* cpu) {
    Z80ChipCore::op_RES_PORP_P00(cpu, 1, cpu->regs.IX);
}

// LD A,RES 1,(IX+N)
static void op_DD_CB_8F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0x9n
//

// LD B,RES 2,(IX+N)
static void op_DD_CB_90(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RES 2,(IX+N)
static void op_DD_CB_91(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RES 2,(IX+N)
static void op_DD_CB_92(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RES 2,(IX+N)
static void op_DD_CB_93(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RES 2,(IX+N)
static void op_DD_CB_94(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RES 2,(IX+N)
static void op_DD_CB_95(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.L), cpu->regs.IX);
}

// RES 2,(IX+N)
static void op_DD_CB_96(Z80Chip* cpu) {
    Z80ChipCore::op_RES_PORP_P00(cpu, 2, cpu->regs.IX);
}

// LD A,RES 2,(IX+N)
static void op_DD_CB_97(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,RES 3,(IX+N)
static void op_DD_CB_98(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RES 3,(IX+N)
static void op_DD_CB_99(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RES 3,(IX+N)
static void op_DD_CB_9A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RES 3,(IX+N)
static void op_DD_CB_9B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RES 3,(IX+N)
static void op_DD_CB_9C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RES 3,(IX+N)
static void op_DD_CB_9D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.L), cpu->regs.IX);
}

// RES 3,(IX+N)
static void op_DD_CB_9E(Z80Chip* cpu) {
    Z80ChipCore::op_RES_PORP_P00(cpu, 3, cpu->regs.IX);
}

// LD A,RES 3,(IX+N)
static void op_DD_CB_9F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0xAn
//

// LD B,RES 4,(IX+N)
static void op_DD_CB_A0(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RES 4,(IX+N)
static void op_DD_CB_A1(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RES 4,(IX+N)
static void op_DD_CB_A2(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RES 4,(IX+N)
static void op_DD_CB_A3(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RES 4,(IX+N)
static void op_DD_CB_A4(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RES 4,(IX+N)
static void op_DD_CB_A5(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.L), cpu->regs.IX);
}

// RES 4,(IX+N)
static void op_DD_CB_A6(Z80Chip* cpu) {
    Z80ChipCore::op_RES_PORP_P00(cpu, 4, cpu->regs.IX);
}

// LD A,RES 4,(IX+N)
static void op_DD_CB_A7(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,RES 5,(IX+N)
static void op_DD_CB_A8(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RES 5,(IX+N)
static void op_DD_CB_A9(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RES 5,(IX+N)
static void op_DD_CB_AA(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RES 5,(IX+N)
static void op_DD_CB_AB(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RES 5,(IX+N)
static void op_DD_CB_AC(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RES 5,(IX+N)
static void op_DD_CB_AD(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.L), cpu->regs.IX);
}

// RES 5,(IX+N)
static void op_DD_CB_AE(Z80Chip* cpu) {
    Z80ChipCore::op_RES_PORP_P00(cpu, 5, cpu->regs.IX);
}

// LD A,RES 5,(IX+N)
static void op_DD_CB_AF(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0xBn
//

// LD B,RES 6,(IX+N)
static void op_DD_CB_B0(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RES 6,(IX+N)
static void op_DD_CB_B1(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RES 6,(IX+N)
static void op_DD_CB_B2(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RES 6,(IX+N)
static void op_DD_CB_B3(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RES 6,(IX+N)
static void op_DD_CB_B4(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RES 6,(IX+N)
static void op_DD_CB_B5(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.L), cpu->regs.IX);
}

// RES 6,(IX+N)
static void op_DD_CB_B6(Z80Chip* cpu) {
    Z80ChipCore::op_RES_PORP_P00(cpu, 6, cpu->regs.IX);
}

// LD A,RES 6,(IX+N)
static void op_DD_CB_B7(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,RES 7,(IX+N)
static void op_DD_CB_B8(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,RES 7,(IX+N)
static void op_DD_CB_B9(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,RES 7,(IX+N)
static void op_DD_CB_BA(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,RES 7,(IX+N)
static void op_DD_CB_BB(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,RES 7,(IX+N)
static void op_DD_CB_BC(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,RES 7,(IX+N)
static void op_DD_CB_BD(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.L), cpu->regs.IX);
}

// RES 7,(IX+N)
static void op_DD_CB_BE(Z80Chip* cpu) {
    Z80ChipCore::op_RES_PORP_P00(cpu, 7, cpu->regs.IX);
}

// LD A,RES 7,(IX+N)
static void op_DD_CB_BF(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0xCn
//

// LD B,SET 0,(IX+N)
static void op_DD_CB_C0(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SET 0,(IX+N)
static void op_DD_CB_C1(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SET 0,(IX+N)
static void op_DD_CB_C2(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SET 0,(IX+N)
static void op_DD_CB_C3(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SET 0,(IX+N)
static void op_DD_CB_C4(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SET 0,(IX+N)
static void op_DD_CB_C5(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.L), cpu->regs.IX);
}

// SET 0,(IX+N)
static void op_DD_CB_C6(Z80Chip* cpu) {
    Z80ChipCore::op_SET_PORP_P00(cpu, 0, cpu->regs.IX);
}

// LD A,SET 0,(IX+N)
static void op_DD_CB_C7(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,SET 1,(IX+N)
static void op_DD_CB_C8(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SET 1,(IX+N)
static void op_DD_CB_C9(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SET 1,(IX+N)
static void op_DD_CB_CA(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SET 1,(IX+N)
static void op_DD_CB_CB(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SET 1,(IX+N)
static void op_DD_CB_CC(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SET 1,(IX+N)
static void op_DD_CB_CD(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.L), cpu->regs.IX);
}

// SET 1,(IX+N)
static void op_DD_CB_CE(Z80Chip* cpu) {
    Z80ChipCore::op_SET_PORP_P00(cpu, 1, cpu->regs.IX);
}

// LD A,SET 1,(IX+N)
static void op_DD_CB_CF(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0xDn
//

// LD B,SET 2,(IX+N)
static void op_DD_CB_D0(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SET 2,(IX+N)
static void op_DD_CB_D1(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SET 2,(IX+N)
static void op_DD_CB_D2(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SET 2,(IX+N)
static void op_DD_CB_D3(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SET 2,(IX+N)
static void op_DD_CB_D4(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SET 2,(IX+N)
static void op_DD_CB_D5(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.L), cpu->regs.IX);
}

// SET 2,(IX+N)
static void op_DD_CB_D6(Z80Chip* cpu) {
    Z80ChipCore::op_SET_PORP_P00(cpu, 2, cpu->regs.IX);
}

// LD A,SET 2,(IX+N)
static void op_DD_CB_D7(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,SET 3,(IX+N)
static void op_DD_CB_D8(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SET 3,(IX+N)
static void op_DD_CB_D9(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SET 3,(IX+N)
static void op_DD_CB_DA(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SET 3,(IX+N)
static void op_DD_CB_DB(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SET 3,(IX+N)
static void op_DD_CB_DC(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SET 3,(IX+N)
static void op_DD_CB_DD(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.L), cpu->regs.IX);
}

// SET 3,(IX+N)
static void op_DD_CB_DE(Z80Chip* cpu) {
    Z80ChipCore::op_SET_PORP_P00(cpu, 3, cpu->regs.IX);
}

// LD A,SET 3,(IX+N)
static void op_DD_CB_DF(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0xEn
//

// LD B,SET 4,(IX+N)
static void op_DD_CB_E0(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SET 4,(IX+N)
static void op_DD_CB_E1(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SET 4,(IX+N)
static void op_DD_CB_E2(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SET 4,(IX+N)
static void op_DD_CB_E3(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SET 4,(IX+N)
static void op_DD_CB_E4(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SET 4,(IX+N)
static void op_DD_CB_E5(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.L), cpu->regs.IX);
}

// SET 4,(IX+N)
static void op_DD_CB_E6(Z80Chip* cpu) {
    Z80ChipCore::op_SET_PORP_P00(cpu, 4, cpu->regs.IX);
}

// LD A,SET 4,(IX+N)
static void op_DD_CB_E7(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,SET 5,(IX+N)
static void op_DD_CB_E8(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SET 5,(IX+N)
static void op_DD_CB_E9(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SET 5,(IX+N)
static void op_DD_CB_EA(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SET 5,(IX+N)
static void op_DD_CB_EB(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SET 5,(IX+N)
static void op_DD_CB_EC(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SET 5,(IX+N)
static void op_DD_CB_ED(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.L), cpu->regs.IX);
}

// SET 5,(IX+N
static void op_DD_CB_EE(Z80Chip* cpu) {
    Z80ChipCore::op_SET_PORP_P00(cpu, 5, cpu->regs.IX);
}

// LD A,SET 5,(IX+N)
static void op_DD_CB_EF(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.A), cpu->regs.IX);
}

//
// 0xFn
//

// LD B,SET 6,(IX+N)
static void op_DD_CB_F0(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SET 6,(IX+N)
static void op_DD_CB_F1(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SET 6,(IX+N)
static void op_DD_CB_F2(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SET 6,(IX+N)
static void op_DD_CB_F3(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SET 6,(IX+N)
static void op_DD_CB_F4(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SET 6,(IX+N)
static void op_DD_CB_F5(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.L), cpu->regs.IX);
}

// SET 6,(IX+N)
static void op_DD_CB_F6(Z80Chip* cpu) {
    Z80ChipCore::op_SET_PORP_P00(cpu, 6, cpu->regs.IX);
}

// LD A,SET 6,(IX+N)
static void op_DD_CB_F7(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.A), cpu->regs.IX);
}

// LD B,SET 7,(IX+N)
static void op_DD_CB_F8(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.B), cpu->regs.IX);
}

// LD C,SET 7,(IX+N)
static void op_DD_CB_F9(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.C), cpu->regs.IX);
}

// LD D,SET 7,(IX+N)
static void op_DD_CB_FA(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.D), cpu->regs.IX);
}

// LD E,SET 7,(IX+N)
static void op_DD_CB_FB(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.E), cpu->regs.IX);
}

// LD H,SET 7,(IX+N)
static void op_DD_CB_FC(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.H), cpu->regs.IX);
}

// LD L,SET 7,(IX+N)
static void op_DD_CB_FD(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.L), cpu->regs.IX);
}

// SET 7,(IX+N)
static void op_DD_CB_FE(Z80Chip* cpu) {
    Z80ChipCore::op_SET_PORP_P00(cpu, 7, cpu->regs.IX);
}

// LD A,SET 7,(IX+N)
static void op_DD_CB_FF(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.A), cpu->regs.IX);
}

Z80Chip::Opcode z80ChipOptable_DD_CB[0x100] = {
        op_DD_CB_00, op_DD_CB_01, op_DD_CB_02, op_DD_CB_03, op_DD_CB_04, op_DD_CB_05, op_DD_CB_06, op_DD_CB_07,
        op_DD_CB_08, op_DD_CB_09, op_DD_CB_0A, op_DD_CB_0B, op_DD_CB_0C, op_DD_CB_0D, op_DD_CB_0E, op_DD_CB_0F,
        op_DD_CB_10, op_DD_CB_11, op_DD_CB_12, op_DD_CB_13, op_DD_CB_14, op_DD_CB_15, op_DD_CB_16, op_DD_CB_17,
        op_DD_CB_18, op_DD_CB_19, op_DD_CB_1A, op_DD_CB_1B, op_DD_CB_1C, op_DD_CB_1D, op_DD_CB_1E, op_DD_CB_1F,
        op_DD_CB_20, op_DD_CB_21, op_DD_CB_22, op_DD_CB_23, op_DD_CB_24, op_DD_CB_25, op_DD_CB_26, op_DD_CB_27,
        op_DD_CB_28, op_DD_CB_29, op_DD_CB_2A, op_DD_CB_2B, op_DD_CB_2C, op_DD_CB_2D, op_DD_CB_2E, op_DD_CB_2F,
        op_DD_CB_30, op_DD_CB_31, op_DD_CB_32, op_DD_CB_33, op_DD_CB_34, op_DD_CB_35, op_DD_CB_36, op_DD_CB_37,
        op_DD_CB_38, op_DD_CB_39, op_DD_CB_3A, op_DD_CB_3B, op_DD_CB_3C, op_DD_CB_3D, op_DD_CB_3E, op_DD_CB_3F,
        op_DD_CB_40, op_DD_CB_41, op_DD_CB_42, op_DD_CB_43, op_DD_CB_44, op_DD_CB_45, op_DD_CB_46, op_DD_CB_47,
        op_DD_CB_48, op_DD_CB_49, op_DD_CB_4A, op_DD_CB_4B, op_DD_CB_4C, op_DD_CB_4D, op_DD_CB_4E, op_DD_CB_4F,
        op_DD_CB_50, op_DD_CB_51, op_DD_CB_52, op_DD_CB_53, op_DD_CB_54, op_DD_CB_55, op_DD_CB_56, op_DD_CB_57,
        op_DD_CB_58, op_DD_CB_59, op_DD_CB_5A, op_DD_CB_5B, op_DD_CB_5C, op_DD_CB_5D, op_DD_CB_5E, op_DD_CB_5F,
        op_DD_CB_60, op_DD_CB_61, op_DD_CB_62, op_DD_CB_63, op_DD_CB_64, op_DD_CB_65, op_DD_CB_66, op_DD_CB_67,
        op_DD_CB_68, op_DD_CB_69, op_DD_CB_6A, op_DD_CB_6B, op_DD_CB_6C, op_DD_CB_6D, op_DD_CB_6E, op_DD_CB_6F,
        op_DD_CB_70, op_DD_CB_71, op_DD_CB_72, op_DD_CB_73, op_DD_CB_74, op_DD_CB_75, op_DD_CB_76, op_DD_CB_77,
        op_DD_CB_78, op_DD_CB_79, op_DD_CB_7A, op_DD_CB_7B, op_DD_CB_7C, op_DD_CB_7D, op_DD_CB_7E, op_DD_CB_7F,
        op_DD_CB_80, op_DD_CB_81, op_DD_CB_82, op_DD_CB_83, op_DD_CB_84, op_DD_CB_85, op_DD_CB_86, op_DD_CB_87,
        op_DD_CB_88, op_DD_CB_89, op_DD_CB_8A, op_DD_CB_8B, op_DD_CB_8C, op_DD_CB_8D, op_DD_CB_8E, op_DD_CB_8F,
        op_DD_CB_90, op_DD_CB_91, op_DD_CB_92, op_DD_CB_93, op_DD_CB_94, op_DD_CB_95, op_DD_CB_96, op_DD_CB_97,
        op_DD_CB_98, op_DD_CB_99, op_DD_CB_9A, op_DD_CB_9B, op_DD_CB_9C, op_DD_CB_9D, op_DD_CB_9E, op_DD_CB_9F,
        op_DD_CB_A0, op_DD_CB_A1, op_DD_CB_A2, op_DD_CB_A3, op_DD_CB_A4, op_DD_CB_A5, op_DD_CB_A6, op_DD_CB_A7,
        op_DD_CB_A8, op_DD_CB_A9, op_DD_CB_AA, op_DD_CB_AB, op_DD_CB_AC, op_DD_CB_AD, op_DD_CB_AE, op_DD_CB_AF,
        op_DD_CB_B0, op_DD_CB_B1, op_DD_CB_B2, op_DD_CB_B3, op_DD_CB_B4, op_DD_CB_B5, op_DD_CB_B6, op_DD_CB_B7,
        op_DD_CB_B8, op_DD_CB_B9, op_DD_CB_BA, op_DD_CB_BB, op_DD_CB_BC, op_DD_CB_BD, op_DD_CB_BE, op_DD_CB_BF,
        op_DD_CB_C0, op_DD_CB_C1, op_DD_CB_C2, op_DD_CB_C3, op_DD_CB_C4, op_DD_CB_C5, op_DD_CB_C6, op_DD_CB_C7,
        op_DD_CB_C8, op_DD_CB_C9, op_DD_CB_CA, op_DD_CB_CB, op_DD_CB_CC, op_DD_CB_CD, op_DD_CB_CE, op_DD_CB_CF,
        op_DD_CB_D0, op_DD_CB_D1, op_DD_CB_D2, op_DD_CB_D3, op_DD_CB_D4, op_DD_CB_D5, op_DD_CB_D6, op_DD_CB_D7,
        op_DD_CB_D8, op_DD_CB_D9, op_DD_CB_DA, op_DD_CB_DB, op_DD_CB_DC, op_DD_CB_DD, op_DD_CB_DE, op_DD_CB_DF,
        op_DD_CB_E0, op_DD_CB_E1, op_DD_CB_E2, op_DD_CB_E3, op_DD_CB_E4, op_DD_CB_E5, op_DD_CB_E6, op_DD_CB_E7,
        op_DD_CB_E8, op_DD_CB_E9, op_DD_CB_EA, op_DD_CB_EB, op_DD_CB_EC, op_DD_CB_ED, op_DD_CB_EE, op_DD_CB_EF,
        op_DD_CB_F0, op_DD_CB_F1, op_DD_CB_F2, op_DD_CB_F3, op_DD_CB_F4, op_DD_CB_F5, op_DD_CB_F6, op_DD_CB_F7,
        op_DD_CB_F8, op_DD_CB_F9, op_DD_CB_FA, op_DD_CB_FB, op_DD_CB_FC, op_DD_CB_FD, op_DD_CB_FE, op_DD_CB_FF };

}
