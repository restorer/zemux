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

// *NOP
static void op_DD_00(Z80Chip* cpu) {
    Z80ChipCore::op_NOP(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD BC,NN
static void op_DD_01(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RP_NN(cpu, &(cpu->regs.BC));
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD (BC),A
static void op_DD_02(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_A(cpu, cpu->regs.BC);
    Z80ChipCore::do_PREF_00(cpu);
}

// *INC BC
static void op_DD_03(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_INC_16, &(cpu->regs.BC));
    Z80ChipCore::do_PREF_00(cpu);
}

// *INC B
static void op_DD_04(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// *DEC B
static void op_DD_05(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD B,N
static void op_DD_06(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.B));
    Z80ChipCore::do_PREF_00(cpu);
}

// *RLCA
static void op_DD_07(Z80Chip* cpu) {
    Z80ChipCore::op_RLCA(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *EX AF,AF'
static void op_DD_08(Z80Chip* cpu) {
    Z80ChipCore::op_EX_AF_AF_(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// ADD IX,BC
static void op_DD_09(Z80Chip* cpu) {
    Z80ChipCore::op_ADD_RP_RP(cpu, &(cpu->regs.IX), cpu->regs.BC);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD A,(BC)
static void op_DD_0A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_A_MRP(cpu, cpu->regs.BC);
    Z80ChipCore::do_PREF_00(cpu);
}

// *DEC BC
static void op_DD_0B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_DEC_16, &(cpu->regs.BC));
    Z80ChipCore::do_PREF_00(cpu);
}

// *INC C
static void op_DD_0C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// *DEC C
static void op_DD_0D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD C,N
static void op_DD_0E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.C));
    Z80ChipCore::do_PREF_00(cpu);
}

// *RRCA
static void op_DD_0F(Z80Chip* cpu) {
    Z80ChipCore::op_RRCA(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x1n
//

// *DJNZ O
static void op_DD_10(Z80Chip* cpu) {
    Z80ChipCore::op_DJNZ(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD DE,NN
static void op_DD_11(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RP_NN(cpu, &(cpu->regs.DE));
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD (DE),A
static void op_DD_12(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MRP_A(cpu, cpu->regs.DE);
    Z80ChipCore::do_PREF_00(cpu);
}

// *INC DE
static void op_DD_13(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_INC_16, &(cpu->regs.DE));
    Z80ChipCore::do_PREF_00(cpu);
}

// *INC D
static void op_DD_14(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// *DEC D
static void op_DD_15(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD D,N
static void op_DD_16(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.D));
    Z80ChipCore::do_PREF_00(cpu);
}

// *RLA
static void op_DD_17(Z80Chip* cpu) {
    Z80ChipCore::op_RLA(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *JR O
static void op_DD_18(Z80Chip* cpu) {
    Z80ChipCore::op_JR(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// ADD IX,DE
static void op_DD_19(Z80Chip* cpu) {
    Z80ChipCore::op_ADD_RP_RP(cpu, &(cpu->regs.IX), cpu->regs.DE);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD A,(DE)
static void op_DD_1A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_A_MRP(cpu, cpu->regs.DE);
    Z80ChipCore::do_PREF_00(cpu);
}

// *DEC DE
static void op_DD_1B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_DEC_16, &(cpu->regs.DE));
    Z80ChipCore::do_PREF_00(cpu);
}

// *INC E
static void op_DD_1C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// *DEC E
static void op_DD_1D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD E,N
static void op_DD_1E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.E));
    Z80ChipCore::do_PREF_00(cpu);
}

// *RRA
static void op_DD_1F(Z80Chip* cpu) {
    Z80ChipCore::op_RRA(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x2n
//

// *JR NZ,O
static void op_DD_20(Z80Chip* cpu) {
    Z80ChipCore::op_JR_CC(cpu, Z80ChipCore::cc_NZ);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IX,NN
static void op_DD_21(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RP_NN(cpu, &(cpu->regs.IX));
    Z80ChipCore::do_PREF_00(cpu);
}

// LD (NN),IX
static void op_DD_22(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MNN_RP(cpu, cpu->regs.IX);
    Z80ChipCore::do_PREF_00(cpu);
}

// INC IX
static void op_DD_23(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_INC_16, &(cpu->regs.IX));
    Z80ChipCore::do_PREF_00(cpu);
}

// INC IXH
static void op_DD_24(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.IXH));
    Z80ChipCore::do_PREF_00(cpu);
}

// DEC IXH
static void op_DD_25(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.IXH));
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXH,N
static void op_DD_26(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.IXH));
    Z80ChipCore::do_PREF_00(cpu);
}

// *DAA
static void op_DD_27(Z80Chip* cpu) {
    Z80ChipCore::op_DAA(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *JR Z,O
static void op_DD_28(Z80Chip* cpu) {
    Z80ChipCore::op_JR_CC(cpu, Z80ChipCore::cc_Z);
    Z80ChipCore::do_PREF_00(cpu);
}

// ADD IX,IX
static void op_DD_29(Z80Chip* cpu) {
    Z80ChipCore::op_ADD_RP_RP(cpu, &(cpu->regs.IX), cpu->regs.IX);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IX,(NN)
static void op_DD_2A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RP_MNN(cpu, &(cpu->regs.IX));
    Z80ChipCore::do_PREF_00(cpu);
}

// DEC IX
static void op_DD_2B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_DEC_16, &(cpu->regs.IX));
    Z80ChipCore::do_PREF_00(cpu);
}

// INC IXL
static void op_DD_2C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.IXL));
    Z80ChipCore::do_PREF_00(cpu);
}

// DEC IXL
static void op_DD_2D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.IXL));
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXL,N
static void op_DD_2E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.IXL));
    Z80ChipCore::do_PREF_00(cpu);
}

// *CPL
static void op_DD_2F(Z80Chip* cpu) {
    Z80ChipCore::op_CPL(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x3n
//

// *JR NC,O
static void op_DD_30(Z80Chip* cpu) {
    Z80ChipCore::op_JR_CC(cpu, Z80ChipCore::cc_NC);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD SP,NN
static void op_DD_31(Z80Chip* cpu) {
    Z80ChipCore::op_LD_RP_NN(cpu, &(cpu->regs.SP));
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD (NN),A
static void op_DD_32(Z80Chip* cpu) {
    Z80ChipCore::op_LD_MNN_A(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *INC SP
static void op_DD_33(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_INC_16, &(cpu->regs.SP));
    Z80ChipCore::do_PREF_00(cpu);
}

// INC (IX+N)
static void op_DD_34(Z80Chip* cpu) {
    Z80ChipCore::op_DO_ORP_P00(cpu, Z80ChipCore::do_INC_8, cpu->regs.IX);
}

// DEC (IX+N)
static void op_DD_35(Z80Chip* cpu) {
    Z80ChipCore::op_DO_ORP_P00(cpu, Z80ChipCore::do_DEC_8, cpu->regs.IX);
}

// LD (IX+N),N
static void op_DD_36(Z80Chip* cpu) {
    Z80ChipCore::op_LD_ORP_N_P00(cpu, cpu->regs.IX);
}

// *SCF
static void op_DD_37(Z80Chip* cpu) {
    Z80ChipCore::op_SCF(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *JR C,O
static void op_DD_38(Z80Chip* cpu) {
    Z80ChipCore::op_JR_CC(cpu, Z80ChipCore::cc_C);
    Z80ChipCore::do_PREF_00(cpu);
}

// ADD IX,SP
static void op_DD_39(Z80Chip* cpu) {
    Z80ChipCore::op_ADD_RP_RP(cpu, &(cpu->regs.IX), cpu->regs.SP);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD A,(NN)
static void op_DD_3A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_A_MNN(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *DEC SP
static void op_DD_3B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_RP(cpu, Z80ChipCore::do_DEC_16, &(cpu->regs.SP));
    Z80ChipCore::do_PREF_00(cpu);
}

// *INC A
static void op_DD_3C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_INC_8, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// *DEC A
static void op_DD_3D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_R(cpu, Z80ChipCore::do_DEC_8, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD A,N
static void op_DD_3E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_N(cpu, &(cpu->regs.A));
    Z80ChipCore::do_PREF_00(cpu);
}

// *CCF
static void op_DD_3F(Z80Chip* cpu) {
    Z80ChipCore::op_CCF(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x4n
//

// *LD B,B
static void op_DD_40(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD B,C
static void op_DD_41(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD B,D
static void op_DD_42(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD B,E
static void op_DD_43(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD B,IXH
static void op_DD_44(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD B,IXL
static void op_DD_45(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD B,(IX+N)
static void op_DD_46(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.B), cpu->regs.IX);
}

// *LD B,A
static void op_DD_47(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD C,B
static void op_DD_48(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD C,C
static void op_DD_49(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD C,D
static void op_DD_4A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD C,E
static void op_DD_4B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD C,IXH
static void op_DD_4C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD C,IXL
static void op_DD_4D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD C,(IX+N)
static void op_DD_4E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.C), cpu->regs.IX);
}

// *LD C,A
static void op_DD_4F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x5n
//

// *LD D,B
static void op_DD_50(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD D,C
static void op_DD_51(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD D,D
static void op_DD_52(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD D,E
static void op_DD_53(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD D,IXH
static void op_DD_54(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD D,IXL
static void op_DD_55(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD D,(IX+N)
static void op_DD_56(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.D), cpu->regs.IX);
}

// *LD D,A
static void op_DD_57(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD E,B
static void op_DD_58(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD E,C
static void op_DD_59(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD E,D
static void op_DD_5A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD E,E
static void op_DD_5B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD E,IXH
static void op_DD_5C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD E,IXL
static void op_DD_5D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD E,(IX+N)
static void op_DD_5E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.E), cpu->regs.IX);
}

// *LD E,A
static void op_DD_5F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x6n
//

// LD IXH,B
static void op_DD_60(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXH), cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXH,C
static void op_DD_61(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXH), cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXH,D
static void op_DD_62(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXH), cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXH,E
static void op_DD_63(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXH), cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXH,H
static void op_DD_64(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXH), cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXH,L
static void op_DD_65(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXH), cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD H,(IX+N)
static void op_DD_66(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.H), cpu->regs.IX);
}

// LD IXH,A
static void op_DD_67(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXH), cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXL,B
static void op_DD_68(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXL), cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXL,C
static void op_DD_69(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXL), cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXL,D
static void op_DD_6A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXL), cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXL,E
static void op_DD_6B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXL), cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXL,H
static void op_DD_6C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXL), cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD IXL,L
static void op_DD_6D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXL), cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD L,(IX+N)
static void op_DD_6E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.L), cpu->regs.IX);
}

// LD IXL,A
static void op_DD_6F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.IXL), cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x7n
//

// LD (IX+N),B
static void op_DD_70(Z80Chip* cpu) {
    Z80ChipCore::op_LD_ORP_R_P00(cpu, cpu->regs.IX, cpu->regs.B);
}

// LD (IX+N),C
static void op_DD_71(Z80Chip* cpu) {
    Z80ChipCore::op_LD_ORP_R_P00(cpu, cpu->regs.IX, cpu->regs.C);
}

// LD (IX+N),D
static void op_DD_72(Z80Chip* cpu) {
    Z80ChipCore::op_LD_ORP_R_P00(cpu, cpu->regs.IX, cpu->regs.D);
}

// LD (IX+N),E
static void op_DD_73(Z80Chip* cpu) {
    Z80ChipCore::op_LD_ORP_R_P00(cpu, cpu->regs.IX, cpu->regs.E);
}

// LD (IX+N),H
static void op_DD_74(Z80Chip* cpu) {
    Z80ChipCore::op_LD_ORP_R_P00(cpu, cpu->regs.IX, cpu->regs.H);
}

// LD (IX+N),L
static void op_DD_75(Z80Chip* cpu) {
    Z80ChipCore::op_LD_ORP_R_P00(cpu, cpu->regs.IX, cpu->regs.L);
}

// *HALT
static void op_DD_76(Z80Chip* cpu) {
    Z80ChipCore::op_HALT(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD (IX+N),A
static void op_DD_77(Z80Chip* cpu) {
    Z80ChipCore::op_LD_ORP_R_P00(cpu, cpu->regs.IX, cpu->regs.A);
}

// *LD A,B
static void op_DD_78(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD A,C
static void op_DD_79(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD A,D
static void op_DD_7A(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *LD A,E
static void op_DD_7B(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD A,IXH
static void op_DD_7C(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD A,IXL
static void op_DD_7D(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD A,(IX+N)
static void op_DD_7E(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.A), cpu->regs.IX);
}

// *LD A,A
static void op_DD_7F(Z80Chip* cpu) {
    Z80ChipCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x8n
//

// *ADD A,B
static void op_DD_80(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *ADD A,C
static void op_DD_81(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *ADD A,D
static void op_DD_82(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *ADD A,E
static void op_DD_83(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// ADD A,IXH
static void op_DD_84(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// ADD A,IXL
static void op_DD_85(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// ADD A,(IX+N)
static void op_DD_86(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_ORP_P00(cpu, Z80ChipCore::do_ADD_8, cpu->regs.IX);
}

// *ADD A,A
static void op_DD_87(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADD_8, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// *ADC A,B
static void op_DD_88(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *ADC A,C
static void op_DD_89(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *ADC A,D
static void op_DD_8A(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *ADC A,E
static void op_DD_8B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// ADC A,IXH
static void op_DD_8C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// ADC A,IXL
static void op_DD_8D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// ADC A,(IX+N)
static void op_DD_8E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_ORP_P00(cpu, Z80ChipCore::do_ADC_8, cpu->regs.IX);
}

// *ADC A,A
static void op_DD_8F(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_ADC_8, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0x9n
//

// *SUB B
static void op_DD_90(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *SUB C
static void op_DD_91(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *SUB D
static void op_DD_92(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *SUB E
static void op_DD_93(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// SUB IXH
static void op_DD_94(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// SUB IXL
static void op_DD_95(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// SUB (IX+N)
static void op_DD_96(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_ORP_P00(cpu, Z80ChipCore::do_SUB_8, cpu->regs.IX);
}

// *SUB A
static void op_DD_97(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SUB_8, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// *SBC A,B
static void op_DD_98(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *SBC A,C
static void op_DD_99(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *SBC A,D
static void op_DD_9A(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *SBC A,E
static void op_DD_9B(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// SBC A,IXH
static void op_DD_9C(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// SBC A,IXL
static void op_DD_9D(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// SBC A,(IX+N)
static void op_DD_9E(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_ORP_P00(cpu, Z80ChipCore::do_SBC_8, cpu->regs.IX);
}

// *SBC A,A
static void op_DD_9F(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_SBC_8, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xAn
//

// *AND B
static void op_DD_A0(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *AND C
static void op_DD_A1(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *AND D
static void op_DD_A2(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *AND E
static void op_DD_A3(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// AND IXH
static void op_DD_A4(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// AND IXL
static void op_DD_A5(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// AND (IX+N)
static void op_DD_A6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_ORP_P00(cpu, Z80ChipCore::do_AND_8, cpu->regs.IX);
}

// *AND A
static void op_DD_A7(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_AND_8, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// *XOR B
static void op_DD_A8(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *XOR C
static void op_DD_A9(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *XOR D
static void op_DD_AA(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *XOR E
static void op_DD_AB(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// XOR IXH
static void op_DD_AC(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// XOR IXL
static void op_DD_AD(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// XOR (IX+N)
static void op_DD_AE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_ORP_P00(cpu, Z80ChipCore::do_XOR_8, cpu->regs.IX);
}

// *XOR A
static void op_DD_AF(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_XOR_8, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xBn
//

// *OR B
static void op_DD_B0(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *OR C
static void op_DD_B1(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *OR D
static void op_DD_B2(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *OR E
static void op_DD_B3(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// OR IXH
static void op_DD_B4(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// OR IXL
static void op_DD_B5(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// OR (IX+N)
static void op_DD_B6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_ORP_P00(cpu, Z80ChipCore::do_OR_8, cpu->regs.IX);
}

// *OR A
static void op_DD_B7(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_OR_8, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CP B
static void op_DD_B8(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.B);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CP C
static void op_DD_B9(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CP D
static void op_DD_BA(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.D);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CP E
static void op_DD_BB(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.E);
    Z80ChipCore::do_PREF_00(cpu);
}

// CP IXH
static void op_DD_BC(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.IXH);
    Z80ChipCore::do_PREF_00(cpu);
}

// CP IXL
static void op_DD_BD(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.IXL);
    Z80ChipCore::do_PREF_00(cpu);
}

// CP (IX+N)
static void op_DD_BE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_ORP_P00(cpu, Z80ChipCore::do_CP_8, cpu->regs.IX);
}

// *CP A
static void op_DD_BF(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_R(cpu, Z80ChipCore::do_CP_8, cpu->regs.A);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xCn
//

// *RET NZ
static void op_DD_C0(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_NZ);
    Z80ChipCore::do_PREF_00(cpu);
}

// *POP BC
static void op_DD_C1(Z80Chip* cpu) {
    Z80ChipCore::op_POP_RP(cpu, &(cpu->regs.BC));
    Z80ChipCore::do_PREF_00(cpu);
}

// *JP NZ,NN
static void op_DD_C2(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_NZ);
    Z80ChipCore::do_PREF_00(cpu);
}

// *JP NN
static void op_DD_C3(Z80Chip* cpu) {
    Z80ChipCore::op_JP(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CALL NZ,NN
static void op_DD_C4(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_NZ);
    Z80ChipCore::do_PREF_00(cpu);
}

// *PUSH BC
static void op_DD_C5(Z80Chip* cpu) {
    Z80ChipCore::op_PUSH_RP(cpu, cpu->regs.BC);
    Z80ChipCore::do_PREF_00(cpu);
}

// *ADD A,N
static void op_DD_C6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_ADD_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RST #00
static void op_DD_C7(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x00);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RET Z
static void op_DD_C8(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_Z);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RET
static void op_DD_C9(Z80Chip* cpu) {
    Z80ChipCore::op_RET(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *JP Z,NN
static void op_DD_CA(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_Z);
    Z80ChipCore::do_PREF_00(cpu);
}

// prefix #DD_CB
static void op_DD_CB(Z80Chip* cpu) {
    Z80ChipCore::op_PREF_XX_CB(cpu, z80ChipOptable_DD_CB);
}

// *CALL Z,NN
static void op_DD_CC(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_Z);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CALL NN
static void op_DD_CD(Z80Chip* cpu) {
    Z80ChipCore::op_CALL(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *ADC A,N
static void op_DD_CE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_ADC_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RST #08
static void op_DD_CF(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x08);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xDn
//

// *RET NC
static void op_DD_D0(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_NC);
    Z80ChipCore::do_PREF_00(cpu);
}

// *POP DE
static void op_DD_D1(Z80Chip* cpu) {
    Z80ChipCore::op_POP_RP(cpu, &(cpu->regs.DE));
    Z80ChipCore::do_PREF_00(cpu);
}

// *JP NC,NN
static void op_DD_D2(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_NC);
    Z80ChipCore::do_PREF_00(cpu);
}

// *OUT (N),A
static void op_DD_D3(Z80Chip* cpu) {
    Z80ChipCore::op_OUT_N_A(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CALL NC,NN
static void op_DD_D4(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_NC);
    Z80ChipCore::do_PREF_00(cpu);
}

// *PUSH DE
static void op_DD_D5(Z80Chip* cpu) {
    Z80ChipCore::op_PUSH_RP(cpu, cpu->regs.DE);
    Z80ChipCore::do_PREF_00(cpu);
}

// *SUB N
static void op_DD_D6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_SUB_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RST #10
static void op_DD_D7(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x10);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RET C
static void op_DD_D8(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *EXX
static void op_DD_D9(Z80Chip* cpu) {
    Z80ChipCore::op_EXX(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *JP C,NN
static void op_DD_DA(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_C);
    Z80ChipCore::do_PREF_00(cpu);
}

// *IN A,(N)
static void op_DD_DB(Z80Chip* cpu) {
    Z80ChipCore::op_IN_A_N(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CALL C,NN
static void op_DD_DC(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_C);
    Z80ChipCore::do_PREF_00(cpu);
}

// prefix #DD
static void op_DD_DD(Z80Chip* cpu) {
    Z80ChipCore::op_PREF_DD(cpu);
}

// *SBC A,N
static void op_DD_DE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_SBC_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RST #18
static void op_DD_DF(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x18);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xEn
//

// *RET PO
static void op_DD_E0(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_PO);
    Z80ChipCore::do_PREF_00(cpu);
}

// POP IX
static void op_DD_E1(Z80Chip* cpu) {
    Z80ChipCore::op_POP_RP(cpu, &(cpu->regs.IX));
    Z80ChipCore::do_PREF_00(cpu);
}

// *JP PO,NN
static void op_DD_E2(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_PO);
    Z80ChipCore::do_PREF_00(cpu);
}

// EX (SP),IX
static void op_DD_E3(Z80Chip* cpu) {
    Z80ChipCore::op_EX_MSP_RP(cpu, &(cpu->regs.IX));
    Z80ChipCore::do_PREF_00(cpu);
}

// *CALL PO,NN
static void op_DD_E4(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_PO);
    Z80ChipCore::do_PREF_00(cpu);
}

// PUSH IX
static void op_DD_E5(Z80Chip* cpu) {
    Z80ChipCore::op_PUSH_RP(cpu, cpu->regs.IX);
    Z80ChipCore::do_PREF_00(cpu);
}

// *AND N
static void op_DD_E6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_AND_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RST #20
static void op_DD_E7(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x20);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RET PE
static void op_DD_E8(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_PE);
    Z80ChipCore::do_PREF_00(cpu);
}

// JP IX
static void op_DD_E9(Z80Chip* cpu) {
    Z80ChipCore::op_JP_RP(cpu, cpu->regs.IX);
    Z80ChipCore::do_PREF_00(cpu);
}

// *JP PE,NN
static void op_DD_EA(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_PE);
    Z80ChipCore::do_PREF_00(cpu);
}

// *EX DE,HL
static void op_DD_EB(Z80Chip* cpu) {
    Z80ChipCore::op_EX_DE_HL(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CALL PE,NN
static void op_DD_EC(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_PE);
    Z80ChipCore::do_PREF_00(cpu);
}

// prefix #ED
static void op_DD_ED(Z80Chip* cpu) {
    Z80ChipCore::op_PREF_ED(cpu);
}

// *XOR N
static void op_DD_EE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_XOR_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RST #28
static void op_DD_EF(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x28);
    Z80ChipCore::do_PREF_00(cpu);
}

//
// 0xFn
//

// *RET P
static void op_DD_F0(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_P);
    Z80ChipCore::do_PREF_00(cpu);
}

// *POP AF
static void op_DD_F1(Z80Chip* cpu) {
    Z80ChipCore::op_POP_RP(cpu, &(cpu->regs.AF));
    Z80ChipCore::do_PREF_00(cpu);
}

// *JP P,NN
static void op_DD_F2(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_P);
    Z80ChipCore::do_PREF_00(cpu);
}

// *DI
static void op_DD_F3(Z80Chip* cpu) {
    Z80ChipCore::op_DI(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CALL P,NN
static void op_DD_F4(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_P);
    Z80ChipCore::do_PREF_00(cpu);
}

// *PUSH AF
static void op_DD_F5(Z80Chip* cpu) {
    Z80ChipCore::op_PUSH_RP(cpu, cpu->regs.AF);
    Z80ChipCore::do_PREF_00(cpu);
}

// *OR N
static void op_DD_F6(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_OR_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RST #30
static void op_DD_F7(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x30);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RET M
static void op_DD_F8(Z80Chip* cpu) {
    Z80ChipCore::op_RET_CC(cpu, Z80ChipCore::cc_M);
    Z80ChipCore::do_PREF_00(cpu);
}

// LD SP,IX
static void op_DD_F9(Z80Chip* cpu) {
    Z80ChipCore::op_LD_SP_RP(cpu, cpu->regs.IX);
    Z80ChipCore::do_PREF_00(cpu);
}

// *JP M,NN
static void op_DD_FA(Z80Chip* cpu) {
    Z80ChipCore::op_JP_CC(cpu, Z80ChipCore::cc_M);
    Z80ChipCore::do_PREF_00(cpu);
}

// *EI
static void op_DD_FB(Z80Chip* cpu) {
    Z80ChipCore::op_EI(cpu);
    Z80ChipCore::do_PREF_00(cpu);
}

// *CALL M,NN
static void op_DD_FC(Z80Chip* cpu) {
    Z80ChipCore::op_CALL_CC(cpu, Z80ChipCore::cc_M);
    Z80ChipCore::do_PREF_00(cpu);
}

// prefix #FD
static void op_DD_FD(Z80Chip* cpu) {
    Z80ChipCore::op_PREF_FD(cpu);
}

// *CP N
static void op_DD_FE(Z80Chip* cpu) {
    Z80ChipCore::op_DO_A_N(cpu, Z80ChipCore::do_CP_8);
    Z80ChipCore::do_PREF_00(cpu);
}

// *RST #38
static void op_DD_FF(Z80Chip* cpu) {
    Z80ChipCore::op_RST(cpu, 0x38);
    Z80ChipCore::do_PREF_00(cpu);
}

Z80Chip::Opcode z80ChipOptable_DD[0x100] = {
        op_DD_00, op_DD_01, op_DD_02, op_DD_03, op_DD_04, op_DD_05, op_DD_06, op_DD_07,
        op_DD_08, op_DD_09, op_DD_0A, op_DD_0B, op_DD_0C, op_DD_0D, op_DD_0E, op_DD_0F,
        op_DD_10, op_DD_11, op_DD_12, op_DD_13, op_DD_14, op_DD_15, op_DD_16, op_DD_17,
        op_DD_18, op_DD_19, op_DD_1A, op_DD_1B, op_DD_1C, op_DD_1D, op_DD_1E, op_DD_1F,
        op_DD_20, op_DD_21, op_DD_22, op_DD_23, op_DD_24, op_DD_25, op_DD_26, op_DD_27,
        op_DD_28, op_DD_29, op_DD_2A, op_DD_2B, op_DD_2C, op_DD_2D, op_DD_2E, op_DD_2F,
        op_DD_30, op_DD_31, op_DD_32, op_DD_33, op_DD_34, op_DD_35, op_DD_36, op_DD_37,
        op_DD_38, op_DD_39, op_DD_3A, op_DD_3B, op_DD_3C, op_DD_3D, op_DD_3E, op_DD_3F,
        op_DD_40, op_DD_41, op_DD_42, op_DD_43, op_DD_44, op_DD_45, op_DD_46, op_DD_47,
        op_DD_48, op_DD_49, op_DD_4A, op_DD_4B, op_DD_4C, op_DD_4D, op_DD_4E, op_DD_4F,
        op_DD_50, op_DD_51, op_DD_52, op_DD_53, op_DD_54, op_DD_55, op_DD_56, op_DD_57,
        op_DD_58, op_DD_59, op_DD_5A, op_DD_5B, op_DD_5C, op_DD_5D, op_DD_5E, op_DD_5F,
        op_DD_60, op_DD_61, op_DD_62, op_DD_63, op_DD_64, op_DD_65, op_DD_66, op_DD_67,
        op_DD_68, op_DD_69, op_DD_6A, op_DD_6B, op_DD_6C, op_DD_6D, op_DD_6E, op_DD_6F,
        op_DD_70, op_DD_71, op_DD_72, op_DD_73, op_DD_74, op_DD_75, op_DD_76, op_DD_77,
        op_DD_78, op_DD_79, op_DD_7A, op_DD_7B, op_DD_7C, op_DD_7D, op_DD_7E, op_DD_7F,
        op_DD_80, op_DD_81, op_DD_82, op_DD_83, op_DD_84, op_DD_85, op_DD_86, op_DD_87,
        op_DD_88, op_DD_89, op_DD_8A, op_DD_8B, op_DD_8C, op_DD_8D, op_DD_8E, op_DD_8F,
        op_DD_90, op_DD_91, op_DD_92, op_DD_93, op_DD_94, op_DD_95, op_DD_96, op_DD_97,
        op_DD_98, op_DD_99, op_DD_9A, op_DD_9B, op_DD_9C, op_DD_9D, op_DD_9E, op_DD_9F,
        op_DD_A0, op_DD_A1, op_DD_A2, op_DD_A3, op_DD_A4, op_DD_A5, op_DD_A6, op_DD_A7,
        op_DD_A8, op_DD_A9, op_DD_AA, op_DD_AB, op_DD_AC, op_DD_AD, op_DD_AE, op_DD_AF,
        op_DD_B0, op_DD_B1, op_DD_B2, op_DD_B3, op_DD_B4, op_DD_B5, op_DD_B6, op_DD_B7,
        op_DD_B8, op_DD_B9, op_DD_BA, op_DD_BB, op_DD_BC, op_DD_BD, op_DD_BE, op_DD_BF,
        op_DD_C0, op_DD_C1, op_DD_C2, op_DD_C3, op_DD_C4, op_DD_C5, op_DD_C6, op_DD_C7,
        op_DD_C8, op_DD_C9, op_DD_CA, op_DD_CB, op_DD_CC, op_DD_CD, op_DD_CE, op_DD_CF,
        op_DD_D0, op_DD_D1, op_DD_D2, op_DD_D3, op_DD_D4, op_DD_D5, op_DD_D6, op_DD_D7,
        op_DD_D8, op_DD_D9, op_DD_DA, op_DD_DB, op_DD_DC, op_DD_DD, op_DD_DE, op_DD_DF,
        op_DD_E0, op_DD_E1, op_DD_E2, op_DD_E3, op_DD_E4, op_DD_E5, op_DD_E6, op_DD_E7,
        op_DD_E8, op_DD_E9, op_DD_EA, op_DD_EB, op_DD_EC, op_DD_ED, op_DD_EE, op_DD_EF,
        op_DD_F0, op_DD_F1, op_DD_F2, op_DD_F3, op_DD_F4, op_DD_F5, op_DD_F6, op_DD_F7,
        op_DD_F8, op_DD_F9, op_DD_FA, op_DD_FB, op_DD_FC, op_DD_FD, op_DD_FE, op_DD_FF };

}
