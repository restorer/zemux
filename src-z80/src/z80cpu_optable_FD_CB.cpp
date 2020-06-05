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

// LD B,RLC (IY+N)
static void op_FD_CB_00(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RLC_8, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RLC (IY+N)
static void op_FD_CB_01(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RLC_8, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RLC (IY+N)
static void op_FD_CB_02(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RLC_8, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RLC (IY+N)
static void op_FD_CB_03(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RLC_8, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RLC (IY+N)
static void op_FD_CB_04(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RLC_8, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RLC (IY+N)
static void op_FD_CB_05(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RLC_8, &(cpu->regs.L), cpu->regs.IY);
}

// RLC (IY+N)
static void op_FD_CB_06(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_PORP_P00(cpu, Z80CpuCore::do_RLC_8, cpu->regs.IY);
}

// LD A,RLC (IY+N)
static void op_FD_CB_07(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RLC_8, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,RRC (IY+N)
static void op_FD_CB_08(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RRC_8, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RRC (IY+N)
static void op_FD_CB_09(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RRC_8, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RRC (IY+N)
static void op_FD_CB_0A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RRC_8, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RRC (IY+N)
static void op_FD_CB_0B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RRC_8, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RRC (IY+N)
static void op_FD_CB_0C(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RRC_8, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RRC (IY+N)
static void op_FD_CB_0D(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RRC_8, &(cpu->regs.L), cpu->regs.IY);
}

// RRC (IY+N)
static void op_FD_CB_0E(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_PORP_P00(cpu, Z80CpuCore::do_RRC_8, cpu->regs.IY);
}

// LD A,RRC (IY+N)
static void op_FD_CB_0F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RRC_8, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0x1n
//

// LD B,RL (IY+N)
static void op_FD_CB_10(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RL_8, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RL (IY+N)
static void op_FD_CB_11(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RL_8, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RL (IY+N)
static void op_FD_CB_12(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RL_8, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RL (IY+N)
static void op_FD_CB_13(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RL_8, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RL (IY+N)
static void op_FD_CB_14(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RL_8, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RL (IY+N)
static void op_FD_CB_15(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RL_8, &(cpu->regs.L), cpu->regs.IY);
}

// RL (IY+N)
static void op_FD_CB_16(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_PORP_P00(cpu, Z80CpuCore::do_RL_8, cpu->regs.IY);
}

// LD A,RL (IY+N)
static void op_FD_CB_17(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RL_8, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,RR (IY+N)
static void op_FD_CB_18(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RR_8, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RR (IY+N)
static void op_FD_CB_19(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RR_8, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RR (IY+N)
static void op_FD_CB_1A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RR_8, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RR (IY+N)
static void op_FD_CB_1B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RR_8, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RR (IY+N)
static void op_FD_CB_1C(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RR_8, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RR (IY+N)
static void op_FD_CB_1D(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RR_8, &(cpu->regs.L), cpu->regs.IY);
}

// RR (IY+N)
static void op_FD_CB_1E(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_PORP_P00(cpu, Z80CpuCore::do_RR_8, cpu->regs.IY);
}

// LD A,RR (IY+N)
static void op_FD_CB_1F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_RR_8, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0x2n
//

// LD B,SLA (IY+N)
static void op_FD_CB_20(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLA_8, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SLA (IY+N)
static void op_FD_CB_21(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLA_8, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SLA (IY+N)
static void op_FD_CB_22(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLA_8, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SLA (IY+N)
static void op_FD_CB_23(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLA_8, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SLA (IY+N)
static void op_FD_CB_24(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLA_8, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SLA (IY+N)
static void op_FD_CB_25(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLA_8, &(cpu->regs.L), cpu->regs.IY);
}

// SLA (IY+N)
static void op_FD_CB_26(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_PORP_P00(cpu, Z80CpuCore::do_SLA_8, cpu->regs.IY);
}

// LD A,SLA (IY+N)
static void op_FD_CB_27(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLA_8, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,SRA (IY+N)
static void op_FD_CB_28(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRA_8, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SRA (IY+N)
static void op_FD_CB_29(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRA_8, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SRA (IY+N)
static void op_FD_CB_2A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRA_8, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SRA (IY+N)
static void op_FD_CB_2B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRA_8, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SRA (IY+N)
static void op_FD_CB_2C(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRA_8, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SRA (IY+N)
static void op_FD_CB_2D(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRA_8, &(cpu->regs.L), cpu->regs.IY);
}

// SRA (IY+N)
static void op_FD_CB_2E(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_PORP_P00(cpu, Z80CpuCore::do_SRA_8, cpu->regs.IY);
}

// LD A,SRA (IY+N)
static void op_FD_CB_2F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRA_8, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0x3n
//

// LD B,SLL (IY+N)
static void op_FD_CB_30(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLL_8, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SLL (IY+N)
static void op_FD_CB_31(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLL_8, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SLL (IY+N)
static void op_FD_CB_32(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLL_8, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SLL (IY+N)
static void op_FD_CB_33(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLL_8, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SLL (IY+N)
static void op_FD_CB_34(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLL_8, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SLL (IY+N)
static void op_FD_CB_35(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLL_8, &(cpu->regs.L), cpu->regs.IY);
}

// SLL (IY+N)
static void op_FD_CB_36(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_PORP_P00(cpu, Z80CpuCore::do_SLL_8, cpu->regs.IY);
}

// LD A,SLL (IY+N)
static void op_FD_CB_37(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SLL_8, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,SRL (IY+N)
static void op_FD_CB_38(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRL_8, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SRL (IY+N)
static void op_FD_CB_39(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRL_8, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SRL (IY+N)
static void op_FD_CB_3A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRL_8, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SRL (IY+N)
static void op_FD_CB_3B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRL_8, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SRL (IY+N)
static void op_FD_CB_3C(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRL_8, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SRL (IY+N)
static void op_FD_CB_3D(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRL_8, &(cpu->regs.L), cpu->regs.IY);
}

// SRL (IY+N)
static void op_FD_CB_3E(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_PORP_P00(cpu, Z80CpuCore::do_SRL_8, cpu->regs.IY);
}

// LD A,SRL (IY+N)
static void op_FD_CB_3F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_DO_R_PORP_P00(cpu, Z80CpuCore::do_SRL_8, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0x4n
//

#define op_FD_CB_40 op_FD_CB_46
#define op_FD_CB_41 op_FD_CB_46
#define op_FD_CB_42 op_FD_CB_46
#define op_FD_CB_43 op_FD_CB_46
#define op_FD_CB_44 op_FD_CB_46
#define op_FD_CB_45 op_FD_CB_46

// BIT 0,(IY+N)
static void op_FD_CB_46(Z80Cpu* cpu) {
    Z80CpuCore::op_BIT_PORP_P00(cpu, 0, cpu->regs.IY);
}

#define op_FD_CB_47 op_FD_CB_46
#define op_FD_CB_48 op_FD_CB_4E
#define op_FD_CB_49 op_FD_CB_4E
#define op_FD_CB_4A op_FD_CB_4E
#define op_FD_CB_4B op_FD_CB_4E
#define op_FD_CB_4C op_FD_CB_4E
#define op_FD_CB_4D op_FD_CB_4E

// BIT 1,(IY+N)
static void op_FD_CB_4E(Z80Cpu* cpu) {
    Z80CpuCore::op_BIT_PORP_P00(cpu, 1, cpu->regs.IY);
}

#define op_FD_CB_4F op_FD_CB_4E

//
// 0x5n
//

#define op_FD_CB_50 op_FD_CB_56
#define op_FD_CB_51 op_FD_CB_56
#define op_FD_CB_52 op_FD_CB_56
#define op_FD_CB_53 op_FD_CB_56
#define op_FD_CB_54 op_FD_CB_56
#define op_FD_CB_55 op_FD_CB_56

// BIT 2,(IY+N)
static void op_FD_CB_56(Z80Cpu* cpu) {
    Z80CpuCore::op_BIT_PORP_P00(cpu, 2, cpu->regs.IY);
}

#define op_FD_CB_57 op_FD_CB_56
#define op_FD_CB_58 op_FD_CB_5E
#define op_FD_CB_59 op_FD_CB_5E
#define op_FD_CB_5A op_FD_CB_5E
#define op_FD_CB_5B op_FD_CB_5E
#define op_FD_CB_5C op_FD_CB_5E
#define op_FD_CB_5D op_FD_CB_5E

// BIT 3,(IY+N)
static void op_FD_CB_5E(Z80Cpu* cpu) {
    Z80CpuCore::op_BIT_PORP_P00(cpu, 3, cpu->regs.IY);
}

#define op_FD_CB_5F op_FD_CB_5E

//
// 0x6n
//

#define op_FD_CB_60 op_FD_CB_66
#define op_FD_CB_61 op_FD_CB_66
#define op_FD_CB_62 op_FD_CB_66
#define op_FD_CB_63 op_FD_CB_66
#define op_FD_CB_64 op_FD_CB_66
#define op_FD_CB_65 op_FD_CB_66

// BIT 4,(IY+N)
static void op_FD_CB_66(Z80Cpu* cpu) {
    Z80CpuCore::op_BIT_PORP_P00(cpu, 4, cpu->regs.IY);
}

#define op_FD_CB_67 op_FD_CB_66
#define op_FD_CB_68 op_FD_CB_6E
#define op_FD_CB_69 op_FD_CB_6E
#define op_FD_CB_6A op_FD_CB_6E
#define op_FD_CB_6B op_FD_CB_6E
#define op_FD_CB_6C op_FD_CB_6E
#define op_FD_CB_6D op_FD_CB_6E

// BIT 5,(IY+N)
static void op_FD_CB_6E(Z80Cpu* cpu) {
    Z80CpuCore::op_BIT_PORP_P00(cpu, 5, cpu->regs.IY);
}

#define op_FD_CB_6F op_FD_CB_6E

//
// 0x7n
//

#define op_FD_CB_70 op_FD_CB_76
#define op_FD_CB_71 op_FD_CB_76
#define op_FD_CB_72 op_FD_CB_76
#define op_FD_CB_73 op_FD_CB_76
#define op_FD_CB_74 op_FD_CB_76
#define op_FD_CB_75 op_FD_CB_76

// BIT 6,(IY+N)
static void op_FD_CB_76(Z80Cpu* cpu) {
    Z80CpuCore::op_BIT_PORP_P00(cpu, 6, cpu->regs.IY);
}

#define op_FD_CB_77 op_FD_CB_76
#define op_FD_CB_78 op_FD_CB_7E
#define op_FD_CB_79 op_FD_CB_7E
#define op_FD_CB_7A op_FD_CB_7E
#define op_FD_CB_7B op_FD_CB_7E
#define op_FD_CB_7C op_FD_CB_7E
#define op_FD_CB_7D op_FD_CB_7E

// BIT 7,(IY+N)
static void op_FD_CB_7E(Z80Cpu* cpu) {
    Z80CpuCore::op_BIT_PORP_P00(cpu, 7, cpu->regs.IY);
}

#define op_FD_CB_7F op_FD_CB_7E

//
// 0x8n
//

// LD B,RES 0,(IY+N)
static void op_FD_CB_80(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RES 0,(IY+N)
static void op_FD_CB_81(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RES 0,(IY+N)
static void op_FD_CB_82(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RES 0,(IY+N)
static void op_FD_CB_83(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RES 0,(IY+N)
static void op_FD_CB_84(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RES 0,(IY+N)
static void op_FD_CB_85(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.L), cpu->regs.IY);
}

// RES 0,(IY+N)
static void op_FD_CB_86(Z80Cpu* cpu) {
    Z80CpuCore::op_RES_PORP_P00(cpu, 0, cpu->regs.IY);
}

// LD A,RES 0,(IY+N)
static void op_FD_CB_87(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 0, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,RES 1,(IY+N)
static void op_FD_CB_88(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RES 1,(IY+N)
static void op_FD_CB_89(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RES 1,(IY+N)
static void op_FD_CB_8A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RES 1,(IY+N)
static void op_FD_CB_8B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RES 1,(IY+N)
static void op_FD_CB_8C(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RES 1,(IY+N)
static void op_FD_CB_8D(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.L), cpu->regs.IY);
}

// RES 1,(IY+N)
static void op_FD_CB_8E(Z80Cpu* cpu) {
    Z80CpuCore::op_RES_PORP_P00(cpu, 1, cpu->regs.IY);
}

// LD A,RES 1,(IY+N)
static void op_FD_CB_8F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 1, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0x9n
//

// LD B,RES 2,(IY+N)
static void op_FD_CB_90(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RES 2,(IY+N)
static void op_FD_CB_91(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RES 2,(IY+N)
static void op_FD_CB_92(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RES 2,(IY+N)
static void op_FD_CB_93(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RES 2,(IY+N)
static void op_FD_CB_94(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RES 2,(IY+N)
static void op_FD_CB_95(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.L), cpu->regs.IY);
}

// RES 2,(IY+N)
static void op_FD_CB_96(Z80Cpu* cpu) {
    Z80CpuCore::op_RES_PORP_P00(cpu, 2, cpu->regs.IY);
}

// LD A,RES 2,(IY+N)
static void op_FD_CB_97(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 2, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,RES 3,(IY+N)
static void op_FD_CB_98(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RES 3,(IY+N)
static void op_FD_CB_99(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RES 3,(IY+N)
static void op_FD_CB_9A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RES 3,(IY+N)
static void op_FD_CB_9B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RES 3,(IY+N)
static void op_FD_CB_9C(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RES 3,(IY+N)
static void op_FD_CB_9D(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.L), cpu->regs.IY);
}

// RES 3,(IY+N)
static void op_FD_CB_9E(Z80Cpu* cpu) {
    Z80CpuCore::op_RES_PORP_P00(cpu, 3, cpu->regs.IY);
}

// LD A,RES 3,(IY+N)
static void op_FD_CB_9F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 3, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0xAn
//

// LD B,RES 4,(IY+N)
static void op_FD_CB_A0(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RES 4,(IY+N)
static void op_FD_CB_A1(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RES 4,(IY+N)
static void op_FD_CB_A2(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RES 4,(IY+N)
static void op_FD_CB_A3(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RES 4,(IY+N)
static void op_FD_CB_A4(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RES 4,(IY+N)
static void op_FD_CB_A5(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.L), cpu->regs.IY);
}

// RES 4,(IY+N)
static void op_FD_CB_A6(Z80Cpu* cpu) {
    Z80CpuCore::op_RES_PORP_P00(cpu, 4, cpu->regs.IY);
}

// LD A,RES 4,(IY+N)
static void op_FD_CB_A7(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 4, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,RES 5,(IY+N)
static void op_FD_CB_A8(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RES 5,(IY+N)
static void op_FD_CB_A9(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RES 5,(IY+N)
static void op_FD_CB_AA(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RES 5,(IY+N)
static void op_FD_CB_AB(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RES 5,(IY+N)
static void op_FD_CB_AC(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RES 5,(IY+N)
static void op_FD_CB_AD(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.L), cpu->regs.IY);
}

// RES 5,(IY+N)
static void op_FD_CB_AE(Z80Cpu* cpu) {
    Z80CpuCore::op_RES_PORP_P00(cpu, 5, cpu->regs.IY);
}

// LD A,RES 5,(IY+N)
static void op_FD_CB_AF(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 5, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0xBn
//

// LD B,RES 6,(IY+N)
static void op_FD_CB_B0(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RES 6,(IY+N)
static void op_FD_CB_B1(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RES 6,(IY+N)
static void op_FD_CB_B2(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RES 6,(IY+N)
static void op_FD_CB_B3(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RES 6,(IY+N)
static void op_FD_CB_B4(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RES 6,(IY+N)
static void op_FD_CB_B5(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.L), cpu->regs.IY);
}

// RES 6,(IY+N)
static void op_FD_CB_B6(Z80Cpu* cpu) {
    Z80CpuCore::op_RES_PORP_P00(cpu, 6, cpu->regs.IY);
}

// LD A,RES 6,(IY+N)
static void op_FD_CB_B7(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 6, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,RES 7,(IY+N)
static void op_FD_CB_B8(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,RES 7,(IY+N)
static void op_FD_CB_B9(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,RES 7,(IY+N)
static void op_FD_CB_BA(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,RES 7,(IY+N)
static void op_FD_CB_BB(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,RES 7,(IY+N)
static void op_FD_CB_BC(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,RES 7,(IY+N)
static void op_FD_CB_BD(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.L), cpu->regs.IY);
}

// RES 7,(IY+N)
static void op_FD_CB_BE(Z80Cpu* cpu) {
    Z80CpuCore::op_RES_PORP_P00(cpu, 7, cpu->regs.IY);
}

// LD A,RES 7,(IY+N)
static void op_FD_CB_BF(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RES_PORP_P00(cpu, 7, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0xCn
//

// LD B,SET 0,(IY+N)
static void op_FD_CB_C0(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SET 0,(IY+N)
static void op_FD_CB_C1(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SET 0,(IY+N)
static void op_FD_CB_C2(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SET 0,(IY+N)
static void op_FD_CB_C3(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SET 0,(IY+N)
static void op_FD_CB_C4(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SET 0,(IY+N)
static void op_FD_CB_C5(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.L), cpu->regs.IY);
}

// SET 0,(IY+N)
static void op_FD_CB_C6(Z80Cpu* cpu) {
    Z80CpuCore::op_SET_PORP_P00(cpu, 0, cpu->regs.IY);
}

// LD A,SET 0,(IY+N)
static void op_FD_CB_C7(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 0, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,SET 1,(IY+N)
static void op_FD_CB_C8(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SET 1,(IY+N)
static void op_FD_CB_C9(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SET 1,(IY+N)
static void op_FD_CB_CA(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SET 1,(IY+N)
static void op_FD_CB_CB(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SET 1,(IY+N)
static void op_FD_CB_CC(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SET 1,(IY+N)
static void op_FD_CB_CD(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.L), cpu->regs.IY);
}

// SET 1,(IY+N)
static void op_FD_CB_CE(Z80Cpu* cpu) {
    Z80CpuCore::op_SET_PORP_P00(cpu, 1, cpu->regs.IY);
}

// LD A,SET 1,(IY+N)
static void op_FD_CB_CF(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 1, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0xDn
//

// LD B,SET 2,(IY+N)
static void op_FD_CB_D0(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SET 2,(IY+N)
static void op_FD_CB_D1(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SET 2,(IY+N)
static void op_FD_CB_D2(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SET 2,(IY+N)
static void op_FD_CB_D3(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SET 2,(IY+N)
static void op_FD_CB_D4(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SET 2,(IY+N)
static void op_FD_CB_D5(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.L), cpu->regs.IY);
}

// SET 2,(IY+N)
static void op_FD_CB_D6(Z80Cpu* cpu) {
    Z80CpuCore::op_SET_PORP_P00(cpu, 2, cpu->regs.IY);
}

// LD A,SET 2,(IY+N)
static void op_FD_CB_D7(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 2, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,SET 3,(IY+N)
static void op_FD_CB_D8(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SET 3,(IY+N)
static void op_FD_CB_D9(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SET 3,(IY+N)
static void op_FD_CB_DA(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SET 3,(IY+N)
static void op_FD_CB_DB(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SET 3,(IY+N)
static void op_FD_CB_DC(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SET 3,(IY+N)
static void op_FD_CB_DD(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.L), cpu->regs.IY);
}

// SET 3,(IY+N)
static void op_FD_CB_DE(Z80Cpu* cpu) {
    Z80CpuCore::op_SET_PORP_P00(cpu, 3, cpu->regs.IY);
}

// LD A,SET 3,(IY+N)
static void op_FD_CB_DF(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 3, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0xEn
//

// LD B,SET 4,(IY+N)
static void op_FD_CB_E0(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SET 4,(IY+N)
static void op_FD_CB_E1(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SET 4,(IY+N)
static void op_FD_CB_E2(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SET 4,(IY+N)
static void op_FD_CB_E3(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SET 4,(IY+N)
static void op_FD_CB_E4(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SET 4,(IY+N)
static void op_FD_CB_E5(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.L), cpu->regs.IY);
}

// SET 4,(IY+N)
static void op_FD_CB_E6(Z80Cpu* cpu) {
    Z80CpuCore::op_SET_PORP_P00(cpu, 4, cpu->regs.IY);
}

// LD A,SET 4,(IY+N)
static void op_FD_CB_E7(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 4, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,SET 5,(IY+N)
static void op_FD_CB_E8(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SET 5,(IY+N)
static void op_FD_CB_E9(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SET 5,(IY+N)
static void op_FD_CB_EA(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SET 5,(IY+N)
static void op_FD_CB_EB(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SET 5,(IY+N)
static void op_FD_CB_EC(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SET 5,(IY+N)
static void op_FD_CB_ED(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.L), cpu->regs.IY);
}

// SET 5,(IY+N
static void op_FD_CB_EE(Z80Cpu* cpu) {
    Z80CpuCore::op_SET_PORP_P00(cpu, 5, cpu->regs.IY);
}

// LD A,SET 5,(IY+N)
static void op_FD_CB_EF(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 5, &(cpu->regs.A), cpu->regs.IY);
}

//
// 0xFn
//

// LD B,SET 6,(IY+N)
static void op_FD_CB_F0(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SET 6,(IY+N)
static void op_FD_CB_F1(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SET 6,(IY+N)
static void op_FD_CB_F2(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SET 6,(IY+N)
static void op_FD_CB_F3(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SET 6,(IY+N)
static void op_FD_CB_F4(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SET 6,(IY+N)
static void op_FD_CB_F5(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.L), cpu->regs.IY);
}

// SET 6,(IY+N)
static void op_FD_CB_F6(Z80Cpu* cpu) {
    Z80CpuCore::op_SET_PORP_P00(cpu, 6, cpu->regs.IY);
}

// LD A,SET 6,(IY+N)
static void op_FD_CB_F7(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 6, &(cpu->regs.A), cpu->regs.IY);
}

// LD B,SET 7,(IY+N)
static void op_FD_CB_F8(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.B), cpu->regs.IY);
}

// LD C,SET 7,(IY+N)
static void op_FD_CB_F9(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.C), cpu->regs.IY);
}

// LD D,SET 7,(IY+N)
static void op_FD_CB_FA(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.D), cpu->regs.IY);
}

// LD E,SET 7,(IY+N)
static void op_FD_CB_FB(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.E), cpu->regs.IY);
}

// LD H,SET 7,(IY+N)
static void op_FD_CB_FC(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.H), cpu->regs.IY);
}

// LD L,SET 7,(IY+N)
static void op_FD_CB_FD(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.L), cpu->regs.IY);
}

// SET 7,(IY+N)
static void op_FD_CB_FE(Z80Cpu* cpu) {
    Z80CpuCore::op_SET_PORP_P00(cpu, 7, cpu->regs.IY);
}

// LD A,SET 7,(IY+N)
static void op_FD_CB_FF(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SET_PORP_P00(cpu, 7, &(cpu->regs.A), cpu->regs.IY);
}

Z80CpuOpcode z80CpuOptable_FD_CB[0x100] = {
    op_FD_CB_00, op_FD_CB_01, op_FD_CB_02, op_FD_CB_03, op_FD_CB_04, op_FD_CB_05, op_FD_CB_06, op_FD_CB_07,
    op_FD_CB_08, op_FD_CB_09, op_FD_CB_0A, op_FD_CB_0B, op_FD_CB_0C, op_FD_CB_0D, op_FD_CB_0E, op_FD_CB_0F,
    op_FD_CB_10, op_FD_CB_11, op_FD_CB_12, op_FD_CB_13, op_FD_CB_14, op_FD_CB_15, op_FD_CB_16, op_FD_CB_17,
    op_FD_CB_18, op_FD_CB_19, op_FD_CB_1A, op_FD_CB_1B, op_FD_CB_1C, op_FD_CB_1D, op_FD_CB_1E, op_FD_CB_1F,
    op_FD_CB_20, op_FD_CB_21, op_FD_CB_22, op_FD_CB_23, op_FD_CB_24, op_FD_CB_25, op_FD_CB_26, op_FD_CB_27,
    op_FD_CB_28, op_FD_CB_29, op_FD_CB_2A, op_FD_CB_2B, op_FD_CB_2C, op_FD_CB_2D, op_FD_CB_2E, op_FD_CB_2F,
    op_FD_CB_30, op_FD_CB_31, op_FD_CB_32, op_FD_CB_33, op_FD_CB_34, op_FD_CB_35, op_FD_CB_36, op_FD_CB_37,
    op_FD_CB_38, op_FD_CB_39, op_FD_CB_3A, op_FD_CB_3B, op_FD_CB_3C, op_FD_CB_3D, op_FD_CB_3E, op_FD_CB_3F,
    op_FD_CB_40, op_FD_CB_41, op_FD_CB_42, op_FD_CB_43, op_FD_CB_44, op_FD_CB_45, op_FD_CB_46, op_FD_CB_47,
    op_FD_CB_48, op_FD_CB_49, op_FD_CB_4A, op_FD_CB_4B, op_FD_CB_4C, op_FD_CB_4D, op_FD_CB_4E, op_FD_CB_4F,
    op_FD_CB_50, op_FD_CB_51, op_FD_CB_52, op_FD_CB_53, op_FD_CB_54, op_FD_CB_55, op_FD_CB_56, op_FD_CB_57,
    op_FD_CB_58, op_FD_CB_59, op_FD_CB_5A, op_FD_CB_5B, op_FD_CB_5C, op_FD_CB_5D, op_FD_CB_5E, op_FD_CB_5F,
    op_FD_CB_60, op_FD_CB_61, op_FD_CB_62, op_FD_CB_63, op_FD_CB_64, op_FD_CB_65, op_FD_CB_66, op_FD_CB_67,
    op_FD_CB_68, op_FD_CB_69, op_FD_CB_6A, op_FD_CB_6B, op_FD_CB_6C, op_FD_CB_6D, op_FD_CB_6E, op_FD_CB_6F,
    op_FD_CB_70, op_FD_CB_71, op_FD_CB_72, op_FD_CB_73, op_FD_CB_74, op_FD_CB_75, op_FD_CB_76, op_FD_CB_77,
    op_FD_CB_78, op_FD_CB_79, op_FD_CB_7A, op_FD_CB_7B, op_FD_CB_7C, op_FD_CB_7D, op_FD_CB_7E, op_FD_CB_7F,
    op_FD_CB_80, op_FD_CB_81, op_FD_CB_82, op_FD_CB_83, op_FD_CB_84, op_FD_CB_85, op_FD_CB_86, op_FD_CB_87,
    op_FD_CB_88, op_FD_CB_89, op_FD_CB_8A, op_FD_CB_8B, op_FD_CB_8C, op_FD_CB_8D, op_FD_CB_8E, op_FD_CB_8F,
    op_FD_CB_90, op_FD_CB_91, op_FD_CB_92, op_FD_CB_93, op_FD_CB_94, op_FD_CB_95, op_FD_CB_96, op_FD_CB_97,
    op_FD_CB_98, op_FD_CB_99, op_FD_CB_9A, op_FD_CB_9B, op_FD_CB_9C, op_FD_CB_9D, op_FD_CB_9E, op_FD_CB_9F,
    op_FD_CB_A0, op_FD_CB_A1, op_FD_CB_A2, op_FD_CB_A3, op_FD_CB_A4, op_FD_CB_A5, op_FD_CB_A6, op_FD_CB_A7,
    op_FD_CB_A8, op_FD_CB_A9, op_FD_CB_AA, op_FD_CB_AB, op_FD_CB_AC, op_FD_CB_AD, op_FD_CB_AE, op_FD_CB_AF,
    op_FD_CB_B0, op_FD_CB_B1, op_FD_CB_B2, op_FD_CB_B3, op_FD_CB_B4, op_FD_CB_B5, op_FD_CB_B6, op_FD_CB_B7,
    op_FD_CB_B8, op_FD_CB_B9, op_FD_CB_BA, op_FD_CB_BB, op_FD_CB_BC, op_FD_CB_BD, op_FD_CB_BE, op_FD_CB_BF,
    op_FD_CB_C0, op_FD_CB_C1, op_FD_CB_C2, op_FD_CB_C3, op_FD_CB_C4, op_FD_CB_C5, op_FD_CB_C6, op_FD_CB_C7,
    op_FD_CB_C8, op_FD_CB_C9, op_FD_CB_CA, op_FD_CB_CB, op_FD_CB_CC, op_FD_CB_CD, op_FD_CB_CE, op_FD_CB_CF,
    op_FD_CB_D0, op_FD_CB_D1, op_FD_CB_D2, op_FD_CB_D3, op_FD_CB_D4, op_FD_CB_D5, op_FD_CB_D6, op_FD_CB_D7,
    op_FD_CB_D8, op_FD_CB_D9, op_FD_CB_DA, op_FD_CB_DB, op_FD_CB_DC, op_FD_CB_DD, op_FD_CB_DE, op_FD_CB_DF,
    op_FD_CB_E0, op_FD_CB_E1, op_FD_CB_E2, op_FD_CB_E3, op_FD_CB_E4, op_FD_CB_E5, op_FD_CB_E6, op_FD_CB_E7,
    op_FD_CB_E8, op_FD_CB_E9, op_FD_CB_EA, op_FD_CB_EB, op_FD_CB_EC, op_FD_CB_ED, op_FD_CB_EE, op_FD_CB_EF,
    op_FD_CB_F0, op_FD_CB_F1, op_FD_CB_F2, op_FD_CB_F3, op_FD_CB_F4, op_FD_CB_F5, op_FD_CB_F6, op_FD_CB_F7,
    op_FD_CB_F8, op_FD_CB_F9, op_FD_CB_FA, op_FD_CB_FB, op_FD_CB_FC, op_FD_CB_FD, op_FD_CB_FE, op_FD_CB_FF
};

}
