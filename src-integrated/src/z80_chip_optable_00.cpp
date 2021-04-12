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

// NOP
static void op_00_00(Z80Chip* cpu) {
    Z80ChipCore::op_NOP(cpu);
}

// LD BC,NN
static void op_00_01(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RP_NN(cpu, &(cpu->regs.BC));
}

// LD (BC),A
static void op_00_02(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_A(cpu, cpu->regs.BC);
}

// INC BC
static void op_00_03(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_INC_16, &(cpu->regs.BC));
}

// INC B
static void op_00_04(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.B));
}

// DEC B
static void op_00_05(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.B));
}

// LD B,N
static void op_00_06(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.B));
}

// RLCA
static void op_00_07(Z80Chip* cpu) {
    Z80ChipCore::op_RLCA(cpu);
}

// EX AF,AF'
static void op_00_08(Z80Chip* cpu) {
    Z80ChipCore::op_EX_AF_AF_(cpu);
}

// ADD HL,BC
static void op_00_09(Z80Chip* cpu) {
    Z80ChipCore::op_ADD_RP_RP(cpu, &(cpu->regs.HL), cpu->regs.BC);
}

// LD A,(BC)
static void op_00_0A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_A_MRP(cpu, cpu->regs.BC);
}

// DEC BC
static void op_00_0B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_DEC_16, &(cpu->regs.BC));
}

// INC C
static void op_00_0C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.C));
}

// DEC C
static void op_00_0D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.C));
}

// LD C,N
static void op_00_0E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.C));
}

// RRCA
static void op_00_0F(Z80Chip* cpu) {
    Z80ChipCore::op_RRCA(cpu);
}

//
// 0x1n
//

// DJNZ O
static void op_00_10(Z80Chip* cpu) {
    Z80ChipCore::op_DJNZ(cpu);
}

// LD DE,NN
static void op_00_11(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RP_NN(cpu, &(cpu->regs.DE));
}

// LD (DE),A
static void op_00_12(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_A(cpu, cpu->regs.DE);
}

// INC DE
static void op_00_13(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_INC_16, &(cpu->regs.DE));
}

// INC D
static void op_00_14(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.D));
}

// DEC D
static void op_00_15(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.D));
}

// LD D,N
static void op_00_16(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.D));
}

// RLA
static void op_00_17(Z80Chip* cpu) {
    Z80ChipCore::op_RLA(cpu);
}

// JR O
static void op_00_18(Z80Chip* cpu) {
    Z80ChipCore::op_JR(cpu);
}

// ADD HL,DE
static void op_00_19(Z80Chip* cpu) {
    Z80ChipCore::op_ADD_RP_RP(cpu, &(cpu->regs.HL), cpu->regs.DE);
}

// LD A,(DE)
static void op_00_1A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_A_MRP(cpu, cpu->regs.DE);
}

// DEC DE
static void op_00_1B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_DEC_16, &(cpu->regs.DE));
}

// INC E
static void op_00_1C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.E));
}

// DEC E
static void op_00_1D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.E));
}

// LD E,N
static void op_00_1E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.E));
}

// RRA
static void op_00_1F(Z80Chip* cpu) {
    Z80ChipCore::op_RRA(cpu);
}

//
// 0x2n
//

// JR NZ,O
static void op_00_20(Z80Chip* cpu) {
    Z80ChipCore::op_JR_CC(cpu, Z80ChipCore::cc_NZ);
}

// LD HL,NN
static void op_00_21(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RP_NN(cpu, &(cpu->regs.HL));
}

// LD (NN),HL
static void op_00_22(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MNN_RP(cpu, cpu->regs.HL);
}

// INC HL
static void op_00_23(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_INC_16, &(cpu->regs.HL));
}

// INC H
static void op_00_24(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.H));
}

// DEC H
static void op_00_25(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.H));
}

// LD H,N
static void op_00_26(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.H));
}

// DAA
static void op_00_27(Z80Chip* cpu) {
    Z80ChipCore::op_DAA(cpu);
}

// JR Z,O
static void op_00_28(Z80Chip* cpu) {
    Z80ChipCore::op_JR_CC(cpu, Z80ChipCore::cc_Z);
}

// ADD HL,HL
static void op_00_29(Z80Chip* cpu) {
    Z80ChipCore::op_ADD_RP_RP(cpu, &(cpu->regs.HL), cpu->regs.HL);
}

// LD HL,(NN)
static void op_00_2A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RP_MNN(cpu, &(cpu->regs.HL));
}

// DEC HL
static void op_00_2B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_DEC_16, &(cpu->regs.HL));
}

// INC L
static void op_00_2C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.L));
}

// DEC L
static void op_00_2D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.L));
}

// LD L,N
static void op_00_2E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.L));
}

// CPL
static void op_00_2F(Z80Chip* cpu) {
    Z80ChipCore::op_CPL(cpu);
}

//
// 0x3n
//

// JR NC,O
static void op_00_30(Z80Chip* cpu) {
    Z80ChipCore::op_JR_CC(cpu, Z80ChipCore::cc_NC);
}

// LD SP,NN
static void op_00_31(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RP_NN(cpu, &(cpu->regs.SP));
}

// LD (NN),A
static void op_00_32(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MNN_A(cpu);
}

// INC SP
static void op_00_33(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_INC_16, &(cpu->regs.SP));
}

// INC (HL)
static void op_00_34(Z80Chip* cpu) {
    Z80ChipCore::op_DO_MHL(cpu, Z80ChipCore::do_INC_8);
}

// DEC (HL)
static void op_00_35(Z80Chip* cpu) {
    Z80ChipCore::op_DO_MHL(cpu, Z80ChipCore::do_DEC_8);
}

// LD (HL),N
static void op_00_36(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MHL_N(cpu);
}

// SCF
static void op_00_37(Z80Chip* cpu) {
    Z80ChipCore::op_SCF(cpu);
}

// JR C,O
static void op_00_38(Z80Chip* cpu) {
    Z80ChipCore::op_JR_CC(cpu, Z80ChipCore::cc_C);
}

// ADD HL,SP
static void op_00_39(Z80Chip* cpu) {
    Z80ChipCore::op_ADD_RP_RP(cpu, &(cpu->regs.HL), cpu->regs.SP);
}

// LD A,(NN)
static void op_00_3A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_A_MNN(cpu);
}

// DEC SP
static void op_00_3B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_DEC_16, &(cpu->regs.SP));
}

// INC A
static void op_00_3C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.A));
}

// DEC A
static void op_00_3D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.A));
}

// LD A,N
static void op_00_3E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.A));
}

// CCF
static void op_00_3F(Z80Chip* cpu) {
    Z80ChipCore::op_CCF(cpu);
}

//
// 0x4n
//

// LD B,B
static void op_00_40(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.B);
}

// LD B,C
static void op_00_41(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.C);
}

// LD B,D
static void op_00_42(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.D);
}

// LD B,E
static void op_00_43(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.E);
}

// LD B,H
static void op_00_44(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.H);
}

// LD B,L
static void op_00_45(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.L);
}

// LD B,(HL)
static void op_00_46(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_MRP(cpu, &(cpu->regs.B), cpu->regs.HL);
}

// LD B,A
static void op_00_47(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.A);
}

// LD C,B
static void op_00_48(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.B);
}

// LD C,C
static void op_00_49(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.C);
}

// LD C,D
static void op_00_4A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.D);
}

// LD C,E
static void op_00_4B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.E);
}

// LD C,H
static void op_00_4C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.H);
}

// LD C,L
static void op_00_4D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.L);
}

// LD C,(HL)
static void op_00_4E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_MRP(cpu, &(cpu->regs.C), cpu->regs.HL);
}

// LD C,A
static void op_00_4F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.A);
}

//
// 0x5n
//

// LD D,B
static void op_00_50(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.B);
}

// LD D,C
static void op_00_51(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.C);
}

// LD D,D
static void op_00_52(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.D);
}

// LD D,E
static void op_00_53(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.E);
}

// LD D,H
static void op_00_54(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.H);
}

// LD D,L
static void op_00_55(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.L);
}

// LD D,(HL)
static void op_00_56(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_MRP(cpu, &(cpu->regs.D), cpu->regs.HL);
}

// LD D,A
static void op_00_57(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.A);
}

// LD E,B
static void op_00_58(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.B);
}

// LD E,C
static void op_00_59(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.C);
}

// LD E,D
static void op_00_5A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.D);
}

// LD E,E
static void op_00_5B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.E);
}

// LD E,H
static void op_00_5C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.H);
}

// LD E,L
static void op_00_5D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.L);
}

// LD E,(HL)
static void op_00_5E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_MRP(cpu, &(cpu->regs.E), cpu->regs.HL);
}

// LD E,A
static void op_00_5F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.A);
}

//
// 0x6n
//

// LD H,B
static void op_00_60(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.H), cpu->regs.B);
}

// LD H,C
static void op_00_61(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.H), cpu->regs.C);
}

// LD H,D
static void op_00_62(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.H), cpu->regs.D);
}

// LD H,E
static void op_00_63(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.H), cpu->regs.E);
}

// LD H,H
static void op_00_64(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.H), cpu->regs.H);
}

// LD H,L
static void op_00_65(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.H), cpu->regs.L);
}

// LD H,(HL)
static void op_00_66(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_MRP(cpu, &(cpu->regs.H), cpu->regs.HL);
}

// LD H,A
static void op_00_67(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.H), cpu->regs.A);
}

// LD L,B
static void op_00_68(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.L), cpu->regs.B);
}

// LD L,C
static void op_00_69(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.L), cpu->regs.C);
}

// LD L,D
static void op_00_6A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.L), cpu->regs.D);
}

// LD L,E
static void op_00_6B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.L), cpu->regs.E);
}

// LD L,H
static void op_00_6C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.L), cpu->regs.H);
}

// LD L,L
static void op_00_6D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.L), cpu->regs.L);
}

// LD L,(HL)
static void op_00_6E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_MRP(cpu, &(cpu->regs.L), cpu->regs.HL);
}

// LD L,A
static void op_00_6F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.L), cpu->regs.A);
}

//
// 0x7n
//

// LD (HL),B
static void op_00_70(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_R(cpu, cpu->regs.HL, cpu->regs.B);
}

// LD (HL),C
static void op_00_71(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_R(cpu, cpu->regs.HL, cpu->regs.C);
}

// LD (HL),D
static void op_00_72(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_R(cpu, cpu->regs.HL, cpu->regs.D);
}

// LD (HL),E
static void op_00_73(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_R(cpu, cpu->regs.HL, cpu->regs.E);
}

// LD (HL),H
static void op_00_74(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_R(cpu, cpu->regs.HL, cpu->regs.H);
}

// LD (HL),L
static void op_00_75(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_R(cpu, cpu->regs.HL, cpu->regs.L);
}

// HALT
static void op_00_76(Z80Chip* cpu) {
    Z80ChipCore::op_HALT(cpu);
}

// LD (HL),A
static void op_00_77(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_R(cpu, cpu->regs.HL, cpu->regs.A);
}

// LD A,B
static void op_00_78(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.B);
}

// LD A,C
static void op_00_79(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.C);
}

// LD A,D
static void op_00_7A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.D);
}

// LD A,E
static void op_00_7B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.E);
}

// LD A,H
static void op_00_7C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.H);
}

// LD A,L
static void op_00_7D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.L);
}

// LD A,(HL)
static void op_00_7E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_MRP(cpu, &(cpu->regs.A), cpu->regs.HL);
}

// LD A,A
static void op_00_7F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.A);
}

//
// 0x8n
//

// ADD A,B
static void op_00_80(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.B);
}

// ADD A,C
static void op_00_81(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.C);
}

// ADD A,D
static void op_00_82(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.D);
}

// ADD A,E
static void op_00_83(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.E);
}

// ADD A,H
static void op_00_84(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.H);
}

// ADD A,L
static void op_00_85(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.L);
}

// ADD A,(HL)
static void op_00_86(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_MHL(cpu, Z80ChipCore::do_ADD_8);
}

// ADD A,A
static void op_00_87(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.A);
}

// ADC A,B
static void op_00_88(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.B);
}

// ADC A,C
static void op_00_89(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.C);
}

// ADC A,D
static void op_00_8A(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.D);
}

// ADC A,E
static void op_00_8B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.E);
}

// ADC A,H
static void op_00_8C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.H);
}

// ADC A,L
static void op_00_8D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.L);
}

// ADC A,(HL)
static void op_00_8E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_MHL(cpu, Z80ChipCore::do_ADC_8);
}

// ADC A,A
static void op_00_8F(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.A);
}

//
// 0x9n
//

// SUB B
static void op_00_90(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.B);
}

// SUB C
static void op_00_91(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.C);
}

// SUB D
static void op_00_92(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.D);
}

// SUB E
static void op_00_93(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.E);
}

// SUB H
static void op_00_94(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.H);
}

// SUB L
static void op_00_95(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.L);
}

// SUB (HL)
static void op_00_96(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_MHL(cpu, Z80ChipCore::do_SUB_8);
}

// SUB A
static void op_00_97(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.A);
}

// SBC A,B
static void op_00_98(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.B);
}

// SBC A,C
static void op_00_99(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.C);
}

// SBC A,D
static void op_00_9A(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.D);
}

// SBC A,E
static void op_00_9B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.E);
}

// SBC A,H
static void op_00_9C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.H);
}

// SBC A,L
static void op_00_9D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.L);
}

// SBC A,(HL)
static void op_00_9E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_MHL(cpu, Z80ChipCore::do_SBC_8);
}

// SBC A,A
static void op_00_9F(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.A);
}

//
// 0xAn
//

// AND B
static void op_00_A0(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.B);
}

// AND C
static void op_00_A1(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.C);
}

// AND D
static void op_00_A2(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.D);
}

// AND E
static void op_00_A3(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.E);
}

// AND H
static void op_00_A4(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.H);
}

// AND L
static void op_00_A5(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.L);
}

// AND (HL)
static void op_00_A6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_MHL(cpu, Z80ChipCore::do_AND_8);
}

// AND A
static void op_00_A7(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.A);
}

// XOR B
static void op_00_A8(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.B);
}

// XOR C
static void op_00_A9(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.C);
}

// XOR D
static void op_00_AA(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.D);
}

// XOR E
static void op_00_AB(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.E);
}

// XOR H
static void op_00_AC(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.H);
}

// XOR L
static void op_00_AD(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.L);
}

// XOR (HL)
static void op_00_AE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_MHL(cpu, Z80ChipCore::do_XOR_8);
}

// XOR A
static void op_00_AF(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.A);
}

//
// 0xBn
//

// OR B
static void op_00_B0(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.B);
}

// OR C
static void op_00_B1(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.C);
}

// OR D
static void op_00_B2(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.D);
}

// OR E
static void op_00_B3(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.E);
}

// OR H
static void op_00_B4(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.H);
}

// OR L
static void op_00_B5(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.L);
}

// OR (HL)
static void op_00_B6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_MHL(cpu, Z80ChipCore::do_OR_8);
}

// OR A
static void op_00_B7(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.A);
}

// CP B
static void op_00_B8(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.B);
}

// CP C
static void op_00_B9(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.C);
}

// CP D
static void op_00_BA(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.D);
}

// CP E
static void op_00_BB(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.E);
}

// CP H
static void op_00_BC(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.H);
}

// CP L
static void op_00_BD(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.L);
}

// CP (HL)
static void op_00_BE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_MHL(cpu, Z80ChipCore::do_CP_8);
}

// CP A
static void op_00_BF(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.A);
}

//
// 0xCn
//

// RET NZ
static void op_00_C0(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_NZ);
}

// POP BC
static void op_00_C1(Z80Chip* cpu) {
    Z80ChipCore::op_POP_RP(cpu, &(cpu->regs.BC));
}

// JP NZ,NN
static void op_00_C2(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_NZ);
}

// JP NN
static void op_00_C3(Z80Chip* cpu) {
    Z80ChipCore::op_JP(cpu);
}

// CALL NZ,NN
static void op_00_C4(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_NZ);
}

// PUSH BC
static void op_00_C5(Z80Chip* cpu) {
    Z80ChipCore::op_PUSH_RP(cpu, cpu->regs.BC);
}

// ADD A,N
static void op_00_C6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_ADD_8);
}

// RST #00
static void op_00_C7(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x00);
}

// RET Z
static void op_00_C8(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_Z);
}

// RET
static void op_00_C9(Z80Chip* cpu) {
    Z80ChipCore::op_RET(cpu);
}

// JP Z,NN
static void op_00_CA(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_Z);
}

// prefix #CB
static void op_00_CB(Z80Chip* cpu) {
    Z80ChipCore::op_PREF_CB(cpu);
}

// CALL Z,NN
static void op_00_CC(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_Z);
}

// CALL NN
static void op_00_CD(Z80Chip* cpu) {
    Z80ChipCore::op_CALL(cpu);
}

// ADC A,N
static void op_00_CE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_ADC_8);
}

// RST #08
static void op_00_CF(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x08);
}

//
// 0xDn
//

// RET NC
static void op_00_D0(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_NC);
}

// POP DE
static void op_00_D1(Z80Chip* cpu) {
    Z80ChipCore::op_POP_RP(cpu, &(cpu->regs.DE));
}

// JP NC,NN
static void op_00_D2(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_NC);
}

// OUT (N),A
static void op_00_D3(Z80Chip* cpu) {
    Z80ChipCore::op_OUT_N_A(cpu);
}

// CALL NC,NN
static void op_00_D4(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_NC);
}

// PUSH DE
static void op_00_D5(Z80Chip* cpu) {
    Z80ChipCore::op_PUSH_RP(cpu, cpu->regs.DE);
}

// SUB N
static void op_00_D6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_SUB_8);
}

// RST #10
static void op_00_D7(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x10);
}

// RET C
static void op_00_D8(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_C);
}

// EXX
static void op_00_D9(Z80Chip* cpu) {
    Z80ChipCore::op_EXX(cpu);
}

// JP C,NN
static void op_00_DA(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_C);
}

// IN A,(N)
static void op_00_DB(Z80Chip* cpu) {
    Z80ChipCore::op_IN_A_N(cpu);
}

// CALL C,NN
static void op_00_DC(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_C);
}

// prefix #DD
static void op_00_DD(Z80Chip* cpu) {
    Z80ChipCore::op_PREF_DD(cpu);
}

// SBC A,N
static void op_00_DE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_SBC_8);
}

// RST #18
static void op_00_DF(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x18);
}

//
// 0xEn
//

// RET PO
static void op_00_E0(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_PO);
}

// POP HL
static void op_00_E1(Z80Chip* cpu) {
    Z80ChipCore::op_POP_RP(cpu, &(cpu->regs.HL));
}

// JP PO,NN
static void op_00_E2(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_PO);
}

// EX (SP),HL
static void op_00_E3(Z80Chip* cpu) {
    Z80ChipCore::op_EX_MSP_RP(cpu, &(cpu->regs.HL));
}

// CALL PO,NN
static void op_00_E4(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_PO);
}

// PUSH HL
static void op_00_E5(Z80Chip* cpu) {
    Z80ChipCore::op_PUSH_RP(cpu, cpu->regs.HL);
}

// AND N
static void op_00_E6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_AND_8);
}

// RST #20
static void op_00_E7(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x20);
}

// RET PE
static void op_00_E8(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_PE);
}

// JP HL
static void op_00_E9(Z80Chip* cpu) {
    Z80ChipCore::op_JP_RP(cpu, cpu->regs.HL);
}

// JP PE,NN
static void op_00_EA(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_PE);
}

// EX DE,HL
static void op_00_EB(Z80Chip* cpu) {
    Z80ChipCore::op_EX_DE_HL(cpu);
}

// CALL PE,NN
static void op_00_EC(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_PE);
}

// prefix #ED
static void op_00_ED(Z80Chip* cpu) {
    Z80ChipCore::op_PREF_ED(cpu);
}

// XOR N
static void op_00_EE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_XOR_8);
}

// RST #28
static void op_00_EF(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x28);
}

//
// 0xFn
//

// RET P
static void op_00_F0(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_P);
}

// POP AF
static void op_00_F1(Z80Chip* cpu) {
    Z80ChipCore::op_POP_RP(cpu, &(cpu->regs.AF));
}

// JP P,NN
static void op_00_F2(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_P);
}

// DI
static void op_00_F3(Z80Chip* cpu) {
    Z80ChipCore::op_DI(cpu);
}

// CALL P,NN
static void op_00_F4(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_P);
}

// PUSH AF
static void op_00_F5(Z80Chip* cpu) {
    Z80ChipCore::op_PUSH_RP(cpu, cpu->regs.AF);
}

// OR N
static void op_00_F6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_OR_8);
}

// RST #30
static void op_00_F7(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x30);
}

// RET M
static void op_00_F8(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_M);
}

// LD SP,HL
static void op_00_F9(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SP_RP(cpu, cpu->regs.HL);
}

// JP M,NN
static void op_00_FA(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_M);
}

// EI
static void op_00_FB(Z80Chip* cpu) {
    Z80ChipCore::op_EI(cpu);
}

// CALL M,NN
static void op_00_FC(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_M);
}

// prefix #FD
static void op_00_FD(Z80Chip* cpu) {
    Z80ChipCore::op_PREF_FD(cpu);
}

// CP N
static void op_00_FE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_CP_8);
}

// RST #38
static void op_00_FF(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x38);
}

Z80Chip::Opcode z80ChipOptable_00[0x100] = {
        op_00_00, op_00_01, op_00_02, op_00_03, op_00_04, op_00_05, op_00_06, op_00_07,
        op_00_08, op_00_09, op_00_0A, op_00_0B, op_00_0C, op_00_0D, op_00_0E, op_00_0F,
        op_00_10, op_00_11, op_00_12, op_00_13, op_00_14, op_00_15, op_00_16, op_00_17,
        op_00_18, op_00_19, op_00_1A, op_00_1B, op_00_1C, op_00_1D, op_00_1E, op_00_1F,
        op_00_20, op_00_21, op_00_22, op_00_23, op_00_24, op_00_25, op_00_26, op_00_27,
        op_00_28, op_00_29, op_00_2A, op_00_2B, op_00_2C, op_00_2D, op_00_2E, op_00_2F,
        op_00_30, op_00_31, op_00_32, op_00_33, op_00_34, op_00_35, op_00_36, op_00_37,
        op_00_38, op_00_39, op_00_3A, op_00_3B, op_00_3C, op_00_3D, op_00_3E, op_00_3F,
        op_00_40, op_00_41, op_00_42, op_00_43, op_00_44, op_00_45, op_00_46, op_00_47,
        op_00_48, op_00_49, op_00_4A, op_00_4B, op_00_4C, op_00_4D, op_00_4E, op_00_4F,
        op_00_50, op_00_51, op_00_52, op_00_53, op_00_54, op_00_55, op_00_56, op_00_57,
        op_00_58, op_00_59, op_00_5A, op_00_5B, op_00_5C, op_00_5D, op_00_5E, op_00_5F,
        op_00_60, op_00_61, op_00_62, op_00_63, op_00_64, op_00_65, op_00_66, op_00_67,
        op_00_68, op_00_69, op_00_6A, op_00_6B, op_00_6C, op_00_6D, op_00_6E, op_00_6F,
        op_00_70, op_00_71, op_00_72, op_00_73, op_00_74, op_00_75, op_00_76, op_00_77,
        op_00_78, op_00_79, op_00_7A, op_00_7B, op_00_7C, op_00_7D, op_00_7E, op_00_7F,
        op_00_80, op_00_81, op_00_82, op_00_83, op_00_84, op_00_85, op_00_86, op_00_87,
        op_00_88, op_00_89, op_00_8A, op_00_8B, op_00_8C, op_00_8D, op_00_8E, op_00_8F,
        op_00_90, op_00_91, op_00_92, op_00_93, op_00_94, op_00_95, op_00_96, op_00_97,
        op_00_98, op_00_99, op_00_9A, op_00_9B, op_00_9C, op_00_9D, op_00_9E, op_00_9F,
        op_00_A0, op_00_A1, op_00_A2, op_00_A3, op_00_A4, op_00_A5, op_00_A6, op_00_A7,
        op_00_A8, op_00_A9, op_00_AA, op_00_AB, op_00_AC, op_00_AD, op_00_AE, op_00_AF,
        op_00_B0, op_00_B1, op_00_B2, op_00_B3, op_00_B4, op_00_B5, op_00_B6, op_00_B7,
        op_00_B8, op_00_B9, op_00_BA, op_00_BB, op_00_BC, op_00_BD, op_00_BE, op_00_BF,
        op_00_C0, op_00_C1, op_00_C2, op_00_C3, op_00_C4, op_00_C5, op_00_C6, op_00_C7,
        op_00_C8, op_00_C9, op_00_CA, op_00_CB, op_00_CC, op_00_CD, op_00_CE, op_00_CF,
        op_00_D0, op_00_D1, op_00_D2, op_00_D3, op_00_D4, op_00_D5, op_00_D6, op_00_D7,
        op_00_D8, op_00_D9, op_00_DA, op_00_DB, op_00_DC, op_00_DD, op_00_DE, op_00_DF,
        op_00_E0, op_00_E1, op_00_E2, op_00_E3, op_00_E4, op_00_E5, op_00_E6, op_00_E7,
        op_00_E8, op_00_E9, op_00_EA, op_00_EB, op_00_EC, op_00_ED, op_00_EE, op_00_EF,
        op_00_F0, op_00_F1, op_00_F2, op_00_F3, op_00_F4, op_00_F5, op_00_F6, op_00_F7,
        op_00_F8, op_00_F9, op_00_FA, op_00_FB, op_00_FC, op_00_FD, op_00_FE, op_00_FF };

}
