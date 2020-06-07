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

#include "z80cpu.h"
#include "z80cpu_core.h"

namespace zemux {

//
// 0x0n
//

// *NOP
static void op_ED_00(Z80Cpu* cpu) {
    Z80CpuCore::op_NOP(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

#define op_ED_01 op_ED_00
#define op_ED_02 op_ED_00
#define op_ED_03 op_ED_00
#define op_ED_04 op_ED_00
#define op_ED_05 op_ED_00
#define op_ED_06 op_ED_00
#define op_ED_07 op_ED_00
#define op_ED_08 op_ED_00
#define op_ED_09 op_ED_00
#define op_ED_0A op_ED_00
#define op_ED_0B op_ED_00
#define op_ED_0C op_ED_00
#define op_ED_0D op_ED_00
#define op_ED_0E op_ED_00
#define op_ED_0F op_ED_00

//
// 0x1n
//

#define op_ED_10 op_ED_00
#define op_ED_11 op_ED_00
#define op_ED_12 op_ED_00
#define op_ED_13 op_ED_00
#define op_ED_14 op_ED_00
#define op_ED_15 op_ED_00
#define op_ED_16 op_ED_00
#define op_ED_17 op_ED_00
#define op_ED_18 op_ED_00
#define op_ED_19 op_ED_00
#define op_ED_1A op_ED_00
#define op_ED_1B op_ED_00
#define op_ED_1C op_ED_00
#define op_ED_1D op_ED_00
#define op_ED_1E op_ED_00
#define op_ED_1F op_ED_00

//
// 0x2n
//

#define op_ED_20 op_ED_00
#define op_ED_21 op_ED_00
#define op_ED_22 op_ED_00
#define op_ED_23 op_ED_00
#define op_ED_24 op_ED_00
#define op_ED_25 op_ED_00
#define op_ED_26 op_ED_00
#define op_ED_27 op_ED_00
#define op_ED_28 op_ED_00
#define op_ED_29 op_ED_00
#define op_ED_2A op_ED_00
#define op_ED_2B op_ED_00
#define op_ED_2C op_ED_00
#define op_ED_2D op_ED_00
#define op_ED_2E op_ED_00
#define op_ED_2F op_ED_00

//
// 0x3n
//

#define op_ED_30 op_ED_00
#define op_ED_31 op_ED_00
#define op_ED_32 op_ED_00
#define op_ED_33 op_ED_00
#define op_ED_34 op_ED_00
#define op_ED_35 op_ED_00
#define op_ED_36 op_ED_00
#define op_ED_37 op_ED_00
#define op_ED_38 op_ED_00
#define op_ED_39 op_ED_00
#define op_ED_3A op_ED_00
#define op_ED_3B op_ED_00
#define op_ED_3C op_ED_00
#define op_ED_3D op_ED_00
#define op_ED_3E op_ED_00
#define op_ED_3F op_ED_00

//
// 0x4n
//

// IN B,(C)
static void op_ED_40(Z80Cpu* cpu) {
    Z80CpuCore::op_IN_R_BC_P00(cpu, &(cpu->regs.B));
}

// OUT (C),B
static void op_ED_41(Z80Cpu* cpu) {
    Z80CpuCore::op_OUT_BC_R_P00(cpu, cpu->regs.B);
}

// SBC HL,BC
static void op_ED_42(Z80Cpu* cpu) {
    Z80CpuCore::op_SBC_HL_RP_P00(cpu, cpu->regs.BC);
}

// LD (NN),BC
static void op_ED_43(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_MNN_RP(cpu, cpu->regs.BC);
    Z80CpuCore::do_PREF_00(cpu);
}

// NEG
static void op_ED_44(Z80Cpu* cpu) {
    Z80CpuCore::op_NEG_P00(cpu);
}

// RETN
static void op_ED_45(Z80Cpu* cpu) {
    Z80CpuCore::op_RETN_P00(cpu);
}

// IM 0
static void op_ED_46(Z80Cpu* cpu) {
    Z80CpuCore::op_IM_P00(cpu, 0);
}

// LD I,A
static void op_ED_47(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_I_A_P00(cpu);
}

// IN C,(C)
static void op_ED_48(Z80Cpu* cpu) {
    Z80CpuCore::op_IN_R_BC_P00(cpu, &(cpu->regs.C));
}

// OUT (C),C
static void op_ED_49(Z80Cpu* cpu) {
    Z80CpuCore::op_OUT_BC_R_P00(cpu, cpu->regs.C);
}

// ADC HL,BC
static void op_ED_4A(Z80Cpu* cpu) {
    Z80CpuCore::op_ADC_HL_RP_P00(cpu, cpu->regs.BC);
}

// LD BC,(NN)
static void op_ED_4B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RP_MNN(cpu, &(cpu->regs.BC));
    Z80CpuCore::do_PREF_00(cpu);
}

// NEG
static void op_ED_4C(Z80Cpu* cpu) {
    Z80CpuCore::op_NEG_P00(cpu);
}

// RETI
static void op_ED_4D(Z80Cpu* cpu) {
    Z80CpuCore::op_RETI_P00(cpu);
}

// IM 0
static void op_ED_4E(Z80Cpu* cpu) {
    Z80CpuCore::op_IM_P00(cpu, 0);
}

// LD R,A
static void op_ED_4F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RR_A_P00(cpu);
}

//
// 0x5n
//

// IN D,(C)
static void op_ED_50(Z80Cpu* cpu) {
    Z80CpuCore::op_IN_R_BC_P00(cpu, &(cpu->regs.D));
}

// OUT (C),D
static void op_ED_51(Z80Cpu* cpu) {
    Z80CpuCore::op_OUT_BC_R_P00(cpu, cpu->regs.D);
}

// SBC HL,DE
static void op_ED_52(Z80Cpu* cpu) {
    Z80CpuCore::op_SBC_HL_RP_P00(cpu, cpu->regs.DE);
}

// LD (NN),DE
static void op_ED_53(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_MNN_RP(cpu, cpu->regs.DE);
    Z80CpuCore::do_PREF_00(cpu);
}

// NEG
static void op_ED_54(Z80Cpu* cpu) {
    Z80CpuCore::op_NEG_P00(cpu);
}

// RETN
static void op_ED_55(Z80Cpu* cpu) {
    Z80CpuCore::op_RETN_P00(cpu);
}

// IM 1
static void op_ED_56(Z80Cpu* cpu) {
    Z80CpuCore::op_IM_P00(cpu, 1);
}

// LD A,I
static void op_ED_57(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_A_I_P00(cpu);
}

// IN E,(C)
static void op_ED_58(Z80Cpu* cpu) {
    Z80CpuCore::op_IN_R_BC_P00(cpu, &(cpu->regs.E));
}

// OUT (C),E
static void op_ED_59(Z80Cpu* cpu) {
    Z80CpuCore::op_OUT_BC_R_P00(cpu, cpu->regs.E);
}

// ADC HL,DE
static void op_ED_5A(Z80Cpu* cpu) {
    Z80CpuCore::op_ADC_HL_RP_P00(cpu, cpu->regs.DE);
}

// LD DE,(NN)
static void op_ED_5B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RP_MNN(cpu, &(cpu->regs.DE));
    Z80CpuCore::do_PREF_00(cpu);
}

// NEG
static void op_ED_5C(Z80Cpu* cpu) {
    Z80CpuCore::op_NEG_P00(cpu);
}

// RETN
static void op_ED_5D(Z80Cpu* cpu) {
    Z80CpuCore::op_RETN_P00(cpu);
}

// IM 2
static void op_ED_5E(Z80Cpu* cpu) {
    Z80CpuCore::op_IM_P00(cpu, 2);
}

// LD A,R
static void op_ED_5F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_A_RR_P00(cpu);
}

//
// 0x6n
//

// IN H,(C)
static void op_ED_60(Z80Cpu* cpu) {
    Z80CpuCore::op_IN_R_BC_P00(cpu, &(cpu->regs.H));
}

// OUT (H),D
static void op_ED_61(Z80Cpu* cpu) {
    Z80CpuCore::op_OUT_BC_R_P00(cpu, cpu->regs.H);
}

// SBC HL,HL
static void op_ED_62(Z80Cpu* cpu) {
    Z80CpuCore::op_SBC_HL_RP_P00(cpu, cpu->regs.HL);
}

// LD (NN),HL
static void op_ED_63(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_MNN_RP(cpu, cpu->regs.HL);
    Z80CpuCore::do_PREF_00(cpu);
}

// NEG
static void op_ED_64(Z80Cpu* cpu) {
    Z80CpuCore::op_NEG_P00(cpu);
}

// RETN
static void op_ED_65(Z80Cpu* cpu) {
    Z80CpuCore::op_RETN_P00(cpu);
}

// IM 0
static void op_ED_66(Z80Cpu* cpu) {
    Z80CpuCore::op_IM_P00(cpu, 0);
}

// RRD
static void op_ED_67(Z80Cpu* cpu) {
    Z80CpuCore::op_RRD_P00(cpu);
}

// IN L,(C)
static void op_ED_68(Z80Cpu* cpu) {
    Z80CpuCore::op_IN_R_BC_P00(cpu, &(cpu->regs.L));
}

// OUT (C),L
static void op_ED_69(Z80Cpu* cpu) {
    Z80CpuCore::op_OUT_BC_R_P00(cpu, cpu->regs.L);
}

// ADC HL,HL
static void op_ED_6A(Z80Cpu* cpu) {
    Z80CpuCore::op_ADC_HL_RP_P00(cpu, cpu->regs.HL);
}

// LD HL,(NN)
static void op_ED_6B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RP_MNN(cpu, &(cpu->regs.HL));
    Z80CpuCore::do_PREF_00(cpu);
}

// NEG
static void op_ED_6C(Z80Cpu* cpu) {
    Z80CpuCore::op_NEG_P00(cpu);
}

// RETN
static void op_ED_6D(Z80Cpu* cpu) {
    Z80CpuCore::op_RETN_P00(cpu);
}

// IM 0
static void op_ED_6E(Z80Cpu* cpu) {
    Z80CpuCore::op_IM_P00(cpu, 0);
}

// RLD
static void op_ED_6F(Z80Cpu* cpu) {
    Z80CpuCore::op_RLD_P00(cpu);
}

// IN F,(C)
static void op_ED_70(Z80Cpu* cpu) {
    Z80CpuCore::op_IN_F_BC_P00(cpu);
}

// OUT (C),0
static void op_ED_71(Z80Cpu* cpu) {
    Z80CpuCore::op_OUT_BC_0_P00(cpu);
}

// SBC HL,SP
static void op_ED_72(Z80Cpu* cpu) {
    Z80CpuCore::op_SBC_HL_RP_P00(cpu, cpu->regs.SP);
}

// LD (NN),SP
static void op_ED_73(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_MNN_RP(cpu, cpu->regs.SP);
    Z80CpuCore::do_PREF_00(cpu);
}

// NEG
static void op_ED_74(Z80Cpu* cpu) {
    Z80CpuCore::op_NEG_P00(cpu);
}

// RETN
static void op_ED_75(Z80Cpu* cpu) {
    Z80CpuCore::op_RETN_P00(cpu);
}

// IM 1
static void op_ED_76(Z80Cpu* cpu) {
    Z80CpuCore::op_IM_P00(cpu, 1);
}

// LD (HL),A
static void op_ED_77(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_MRP_R(cpu, cpu->regs.HL, cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

// IN A,(C)
static void op_ED_78(Z80Cpu* cpu) {
    Z80CpuCore::op_IN_R_BC_P00(cpu, &(cpu->regs.A));
}

// OUT (C),A
static void op_ED_79(Z80Cpu* cpu) {
    Z80CpuCore::op_OUT_BC_R_P00(cpu, cpu->regs.A);
}

// ADC HL,SP
static void op_ED_7A(Z80Cpu* cpu) {
    Z80CpuCore::op_ADC_HL_RP_P00(cpu, cpu->regs.SP);
}

// LD SP,(NN)
static void op_ED_7B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RP_MNN(cpu, &(cpu->regs.SP));
    Z80CpuCore::do_PREF_00(cpu);
}

// NEG
static void op_ED_7C(Z80Cpu* cpu) {
    Z80CpuCore::op_NEG_P00(cpu);
}

// RETN
static void op_ED_7D(Z80Cpu* cpu) {
    Z80CpuCore::op_RETN_P00(cpu);
}

// IM 2
static void op_ED_7E(Z80Cpu* cpu) {
    Z80CpuCore::op_IM_P00(cpu, 2);
}

#define op_ED_7F op_ED_00

//
// 0x8n
//

#define op_ED_80 op_ED_00
#define op_ED_81 op_ED_00
#define op_ED_82 op_ED_00
#define op_ED_83 op_ED_00
#define op_ED_84 op_ED_00
#define op_ED_85 op_ED_00
#define op_ED_86 op_ED_00
#define op_ED_87 op_ED_00
#define op_ED_88 op_ED_00
#define op_ED_89 op_ED_00
#define op_ED_8A op_ED_00
#define op_ED_8B op_ED_00
#define op_ED_8C op_ED_00
#define op_ED_8D op_ED_00
#define op_ED_8E op_ED_00
#define op_ED_8F op_ED_00

//
// 0x9n
//

#define op_ED_90 op_ED_00
#define op_ED_91 op_ED_00
#define op_ED_92 op_ED_00
#define op_ED_93 op_ED_00
#define op_ED_94 op_ED_00
#define op_ED_95 op_ED_00
#define op_ED_96 op_ED_00
#define op_ED_97 op_ED_00
#define op_ED_98 op_ED_00
#define op_ED_99 op_ED_00
#define op_ED_9A op_ED_00
#define op_ED_9B op_ED_00
#define op_ED_9C op_ED_00
#define op_ED_9D op_ED_00
#define op_ED_9E op_ED_00
#define op_ED_9F op_ED_00

// LDI
static void op_ED_A0(Z80Cpu* cpu) {
    Z80CpuCore::op_LDI_P00(cpu);
}

// CPI
static void op_ED_A1(Z80Cpu* cpu) {
    Z80CpuCore::op_CPI_P00(cpu);
}

// INI
static void op_ED_A2(Z80Cpu* cpu) {
    Z80CpuCore::op_INI_P00(cpu);
}

// OUTI
static void op_ED_A3(Z80Cpu* cpu) {
    Z80CpuCore::op_OUTI_P00(cpu);
}

#define op_ED_A4 op_ED_00
#define op_ED_A5 op_ED_00
#define op_ED_A6 op_ED_00
#define op_ED_A7 op_ED_00

// LDD
static void op_ED_A8(Z80Cpu* cpu) {
    Z80CpuCore::op_LDD_P00(cpu);
}

// CPD
static void op_ED_A9(Z80Cpu* cpu) {
    Z80CpuCore::op_CPD_P00(cpu);
}

// IND
static void op_ED_AA(Z80Cpu* cpu) {
    Z80CpuCore::op_IND_P00(cpu);
}

// OUTD
static void op_ED_AB(Z80Cpu* cpu) {
    Z80CpuCore::op_OUTD_P00(cpu);
}

#define op_ED_AC op_ED_00
#define op_ED_AD op_ED_00
#define op_ED_AE op_ED_00
#define op_ED_AF op_ED_00

// LDIR
static void op_ED_B0(Z80Cpu* cpu) {
    Z80CpuCore::op_LDIR_P00(cpu);
}

// CPIR
static void op_ED_B1(Z80Cpu* cpu) {
    Z80CpuCore::op_CPIR_P00(cpu);
}

// INIR
static void op_ED_B2(Z80Cpu* cpu) {
    Z80CpuCore::op_INIR_P00(cpu);
}

// OTIR
static void op_ED_B3(Z80Cpu* cpu) {
    Z80CpuCore::op_OTIR_P00(cpu);
}

#define op_ED_B4 op_ED_00
#define op_ED_B5 op_ED_00
#define op_ED_B6 op_ED_00
#define op_ED_B7 op_ED_00

// LDDR
static void op_ED_B8(Z80Cpu* cpu) {
    Z80CpuCore::op_LDDR_P00(cpu);
}

// CPDR
static void op_ED_B9(Z80Cpu* cpu) {
    Z80CpuCore::op_CPDR_P00(cpu);
}

// INDR
static void op_ED_BA(Z80Cpu* cpu) {
    Z80CpuCore::op_INDR_P00(cpu);
}

// OTDR
static void op_ED_BB(Z80Cpu* cpu) {
    Z80CpuCore::op_OTDR_P00(cpu);
}

#define op_ED_BC op_ED_00
#define op_ED_BD op_ED_00
#define op_ED_BE op_ED_00
#define op_ED_BF op_ED_00

//
// 0xCn
//

#define op_ED_C0 op_ED_00
#define op_ED_C1 op_ED_00
#define op_ED_C2 op_ED_00
#define op_ED_C3 op_ED_00
#define op_ED_C4 op_ED_00
#define op_ED_C5 op_ED_00
#define op_ED_C6 op_ED_00
#define op_ED_C7 op_ED_00
#define op_ED_C8 op_ED_00
#define op_ED_C9 op_ED_00
#define op_ED_CA op_ED_00
#define op_ED_CB op_ED_00
#define op_ED_CC op_ED_00
#define op_ED_CD op_ED_00
#define op_ED_CE op_ED_00
#define op_ED_CF op_ED_00

//
// 0xDn
//

#define op_ED_D0 op_ED_00
#define op_ED_D1 op_ED_00
#define op_ED_D2 op_ED_00
#define op_ED_D3 op_ED_00
#define op_ED_D4 op_ED_00
#define op_ED_D5 op_ED_00
#define op_ED_D6 op_ED_00
#define op_ED_D7 op_ED_00
#define op_ED_D8 op_ED_00
#define op_ED_D9 op_ED_00
#define op_ED_DA op_ED_00
#define op_ED_DB op_ED_00
#define op_ED_DC op_ED_00
#define op_ED_DD op_ED_00
#define op_ED_DE op_ED_00
#define op_ED_DF op_ED_00

//
// 0xEn
//

#define op_ED_E0 op_ED_00
#define op_ED_E1 op_ED_00
#define op_ED_E2 op_ED_00
#define op_ED_E3 op_ED_00
#define op_ED_E4 op_ED_00
#define op_ED_E5 op_ED_00
#define op_ED_E6 op_ED_00
#define op_ED_E7 op_ED_00
#define op_ED_E8 op_ED_00
#define op_ED_E9 op_ED_00
#define op_ED_EA op_ED_00
#define op_ED_EB op_ED_00
#define op_ED_EC op_ED_00
#define op_ED_ED op_ED_00
#define op_ED_EE op_ED_00
#define op_ED_EF op_ED_00

//
// 0xFn
//

#define op_ED_F0 op_ED_00
#define op_ED_F1 op_ED_00
#define op_ED_F2 op_ED_00
#define op_ED_F3 op_ED_00
#define op_ED_F4 op_ED_00
#define op_ED_F5 op_ED_00
#define op_ED_F6 op_ED_00
#define op_ED_F7 op_ED_00
#define op_ED_F8 op_ED_00
#define op_ED_F9 op_ED_00
#define op_ED_FA op_ED_00
#define op_ED_FB op_ED_00
#define op_ED_FC op_ED_00
#define op_ED_FD op_ED_00
#define op_ED_FE op_ED_00
#define op_ED_FF op_ED_00

Z80CpuOpcode z80CpuOptable_ED[0x100] = {
        op_ED_00, op_ED_01, op_ED_02, op_ED_03, op_ED_04, op_ED_05, op_ED_06, op_ED_07,
        op_ED_08, op_ED_09, op_ED_0A, op_ED_0B, op_ED_0C, op_ED_0D, op_ED_0E, op_ED_0F,
        op_ED_10, op_ED_11, op_ED_12, op_ED_13, op_ED_14, op_ED_15, op_ED_16, op_ED_17,
        op_ED_18, op_ED_19, op_ED_1A, op_ED_1B, op_ED_1C, op_ED_1D, op_ED_1E, op_ED_1F,
        op_ED_20, op_ED_21, op_ED_22, op_ED_23, op_ED_24, op_ED_25, op_ED_26, op_ED_27,
        op_ED_28, op_ED_29, op_ED_2A, op_ED_2B, op_ED_2C, op_ED_2D, op_ED_2E, op_ED_2F,
        op_ED_30, op_ED_31, op_ED_32, op_ED_33, op_ED_34, op_ED_35, op_ED_36, op_ED_37,
        op_ED_38, op_ED_39, op_ED_3A, op_ED_3B, op_ED_3C, op_ED_3D, op_ED_3E, op_ED_3F,
        op_ED_40, op_ED_41, op_ED_42, op_ED_43, op_ED_44, op_ED_45, op_ED_46, op_ED_47,
        op_ED_48, op_ED_49, op_ED_4A, op_ED_4B, op_ED_4C, op_ED_4D, op_ED_4E, op_ED_4F,
        op_ED_50, op_ED_51, op_ED_52, op_ED_53, op_ED_54, op_ED_55, op_ED_56, op_ED_57,
        op_ED_58, op_ED_59, op_ED_5A, op_ED_5B, op_ED_5C, op_ED_5D, op_ED_5E, op_ED_5F,
        op_ED_60, op_ED_61, op_ED_62, op_ED_63, op_ED_64, op_ED_65, op_ED_66, op_ED_67,
        op_ED_68, op_ED_69, op_ED_6A, op_ED_6B, op_ED_6C, op_ED_6D, op_ED_6E, op_ED_6F,
        op_ED_70, op_ED_71, op_ED_72, op_ED_73, op_ED_74, op_ED_75, op_ED_76, op_ED_77,
        op_ED_78, op_ED_79, op_ED_7A, op_ED_7B, op_ED_7C, op_ED_7D, op_ED_7E, op_ED_7F,
        op_ED_80, op_ED_81, op_ED_82, op_ED_83, op_ED_84, op_ED_85, op_ED_86, op_ED_87,
        op_ED_88, op_ED_89, op_ED_8A, op_ED_8B, op_ED_8C, op_ED_8D, op_ED_8E, op_ED_8F,
        op_ED_90, op_ED_91, op_ED_92, op_ED_93, op_ED_94, op_ED_95, op_ED_96, op_ED_97,
        op_ED_98, op_ED_99, op_ED_9A, op_ED_9B, op_ED_9C, op_ED_9D, op_ED_9E, op_ED_9F,
        op_ED_A0, op_ED_A1, op_ED_A2, op_ED_A3, op_ED_A4, op_ED_A5, op_ED_A6, op_ED_A7,
        op_ED_A8, op_ED_A9, op_ED_AA, op_ED_AB, op_ED_AC, op_ED_AD, op_ED_AE, op_ED_AF,
        op_ED_B0, op_ED_B1, op_ED_B2, op_ED_B3, op_ED_B4, op_ED_B5, op_ED_B6, op_ED_B7,
        op_ED_B8, op_ED_B9, op_ED_BA, op_ED_BB, op_ED_BC, op_ED_BD, op_ED_BE, op_ED_BF,
        op_ED_C0, op_ED_C1, op_ED_C2, op_ED_C3, op_ED_C4, op_ED_C5, op_ED_C6, op_ED_C7,
        op_ED_C8, op_ED_C9, op_ED_CA, op_ED_CB, op_ED_CC, op_ED_CD, op_ED_CE, op_ED_CF,
        op_ED_D0, op_ED_D1, op_ED_D2, op_ED_D3, op_ED_D4, op_ED_D5, op_ED_D6, op_ED_D7,
        op_ED_D8, op_ED_D9, op_ED_DA, op_ED_DB, op_ED_DC, op_ED_DD, op_ED_DE, op_ED_DF,
        op_ED_E0, op_ED_E1, op_ED_E2, op_ED_E3, op_ED_E4, op_ED_E5, op_ED_E6, op_ED_E7,
        op_ED_E8, op_ED_E9, op_ED_EA, op_ED_EB, op_ED_EC, op_ED_ED, op_ED_EE, op_ED_EF,
        op_ED_F0, op_ED_F1, op_ED_F2, op_ED_F3, op_ED_F4, op_ED_F5, op_ED_F6, op_ED_F7,
        op_ED_F8, op_ED_F9, op_ED_FA, op_ED_FB, op_ED_FC, op_ED_FD, op_ED_FE, op_ED_FF };

}
