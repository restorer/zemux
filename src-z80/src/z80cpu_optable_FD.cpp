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
static void op_FD_00(Z80Cpu* cpu) {
    Z80CpuCore::op_NOP(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD BC,NN
static void op_FD_01(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RP_NN(cpu, &(cpu->regs.BC));
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD (BC),A
static void op_FD_02(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_MRP_A(cpu, cpu->regs.BC);
    Z80CpuCore::do_PREF_00(cpu);
}

// *INC BC
static void op_FD_03(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_RP(cpu, Z80CpuCore::do_INC_16, &(cpu->regs.BC));
    Z80CpuCore::do_PREF_00(cpu);
}

// *INC B
static void op_FD_04(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_INC_8, &(cpu->regs.B));
    Z80CpuCore::do_PREF_00(cpu);
}

// *DEC B
static void op_FD_05(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_DEC_8, &(cpu->regs.B));
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD B,N
static void op_FD_06(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_N(cpu, &(cpu->regs.B));
    Z80CpuCore::do_PREF_00(cpu);
}

// *RLCA
static void op_FD_07(Z80Cpu* cpu) {
    Z80CpuCore::op_RLCA(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *EX AF,AF'
static void op_FD_08(Z80Cpu* cpu) {
    Z80CpuCore::op_EX_AF_AF_(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// ADD IY,BC
static void op_FD_09(Z80Cpu* cpu) {
    Z80CpuCore::op_ADD_RP_RP(cpu, &(cpu->regs.IY), cpu->regs.BC);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD A,(BC)
static void op_FD_0A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_A_MRP(cpu, cpu->regs.BC);
    Z80CpuCore::do_PREF_00(cpu);
}

// *DEC BC
static void op_FD_0B(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_RP(cpu, Z80CpuCore::do_DEC_16, &(cpu->regs.BC));
    Z80CpuCore::do_PREF_00(cpu);
}

// *INC C
static void op_FD_0C(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_INC_8, &(cpu->regs.C));
    Z80CpuCore::do_PREF_00(cpu);
}

// *DEC C
static void op_FD_0D(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_DEC_8, &(cpu->regs.C));
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD C,N
static void op_FD_0E(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_N(cpu, &(cpu->regs.C));
    Z80CpuCore::do_PREF_00(cpu);
}

// *RRCA
static void op_FD_0F(Z80Cpu* cpu) {
    Z80CpuCore::op_RRCA(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0x1n
//

// *DJNZ O
static void op_FD_10(Z80Cpu* cpu) {
    Z80CpuCore::op_DJNZ(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD DE,NN
static void op_FD_11(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RP_NN(cpu, &(cpu->regs.DE));
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD (DE),A
static void op_FD_12(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_MRP_A(cpu, cpu->regs.DE);
    Z80CpuCore::do_PREF_00(cpu);
}

// *INC DE
static void op_FD_13(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_RP(cpu, Z80CpuCore::do_INC_16, &(cpu->regs.DE));
    Z80CpuCore::do_PREF_00(cpu);
}

// *INC D
static void op_FD_14(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_INC_8, &(cpu->regs.D));
    Z80CpuCore::do_PREF_00(cpu);
}

// *DEC D
static void op_FD_15(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_DEC_8, &(cpu->regs.D));
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD D,N
static void op_FD_16(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_N(cpu, &(cpu->regs.D));
    Z80CpuCore::do_PREF_00(cpu);
}

// *RLA
static void op_FD_17(Z80Cpu* cpu) {
    Z80CpuCore::op_RLA(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *JR O
static void op_FD_18(Z80Cpu* cpu) {
    Z80CpuCore::op_JR(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// ADD IY,DE
static void op_FD_19(Z80Cpu* cpu) {
    Z80CpuCore::op_ADD_RP_RP(cpu, &(cpu->regs.IY), cpu->regs.DE);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD A,(DE)
static void op_FD_1A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_A_MRP(cpu, cpu->regs.DE);
    Z80CpuCore::do_PREF_00(cpu);
}

// *DEC DE
static void op_FD_1B(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_RP(cpu, Z80CpuCore::do_DEC_16, &(cpu->regs.DE));
    Z80CpuCore::do_PREF_00(cpu);
}

// *INC E
static void op_FD_1C(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_INC_8, &(cpu->regs.E));
    Z80CpuCore::do_PREF_00(cpu);
}

// *DEC E
static void op_FD_1D(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_DEC_8, &(cpu->regs.E));
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD E,N
static void op_FD_1E(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_N(cpu, &(cpu->regs.E));
    Z80CpuCore::do_PREF_00(cpu);
}

// *RRA
static void op_FD_1F(Z80Cpu* cpu) {
    Z80CpuCore::op_RRA(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0x2n
//

// *JR NZ,O
static void op_FD_20(Z80Cpu* cpu) {
    Z80CpuCore::op_JR_CC(cpu, Z80CpuCore::cc_NZ);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IY,NN
static void op_FD_21(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RP_NN(cpu, &(cpu->regs.IY));
    Z80CpuCore::do_PREF_00(cpu);
}

// LD (NN),IY
static void op_FD_22(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_MNN_RP(cpu, cpu->regs.IYL, cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// INC IY
static void op_FD_23(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_RP(cpu, Z80CpuCore::do_INC_16, &(cpu->regs.IY));
    Z80CpuCore::do_PREF_00(cpu);
}

// INC IYH
static void op_FD_24(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_INC_8, &(cpu->regs.IYH));
    Z80CpuCore::do_PREF_00(cpu);
}

// DEC IYH
static void op_FD_25(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_DEC_8, &(cpu->regs.IYH));
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYH,N
static void op_FD_26(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_N(cpu, &(cpu->regs.IYH));
    Z80CpuCore::do_PREF_00(cpu);
}

// *DAA
static void op_FD_27(Z80Cpu* cpu) {
    Z80CpuCore::op_DAA(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *JR Z,O
static void op_FD_28(Z80Cpu* cpu) {
    Z80CpuCore::op_JR_CC(cpu, Z80CpuCore::cc_Z);
    Z80CpuCore::do_PREF_00(cpu);
}

// ADD IY,IY
static void op_FD_29(Z80Cpu* cpu) {
    Z80CpuCore::op_ADD_RP_RP(cpu, &(cpu->regs.IY), cpu->regs.IY);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IY,(NN)
static void op_FD_2A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RP_MNN(cpu, &(cpu->regs.IYL), &(cpu->regs.IYH));
    Z80CpuCore::do_PREF_00(cpu);
}

// DEC IY
static void op_FD_2B(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_RP(cpu, Z80CpuCore::do_DEC_16, &(cpu->regs.IY));
    Z80CpuCore::do_PREF_00(cpu);
}

// INC IYL
static void op_FD_2C(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_INC_8, &(cpu->regs.IYL));
    Z80CpuCore::do_PREF_00(cpu);
}

// DEC IYL
static void op_FD_2D(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_DEC_8, &(cpu->regs.IYL));
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYL,N
static void op_FD_2E(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_N(cpu, &(cpu->regs.IYL));
    Z80CpuCore::do_PREF_00(cpu);
}

// *CPL
static void op_FD_2F(Z80Cpu* cpu) {
    Z80CpuCore::op_CPL(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0x3n
//

// *JR NC,O
static void op_FD_30(Z80Cpu* cpu) {
    Z80CpuCore::op_JR_CC(cpu, Z80CpuCore::cc_NC);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD SP,NN
static void op_FD_31(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_RP_NN(cpu, &(cpu->regs.SP));
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD (NN),A
static void op_FD_32(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_MNN_A(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *INC SP
static void op_FD_33(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_RP(cpu, Z80CpuCore::do_INC_16, &(cpu->regs.SP));
    Z80CpuCore::do_PREF_00(cpu);
}

// INC (IY+N)
static void op_FD_34(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_ORP(cpu, Z80CpuCore::do_INC_8, cpu->regs.IY);
}

// DEC (IY+N)
static void op_FD_35(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_ORP(cpu, Z80CpuCore::do_DEC_8, cpu->regs.IY);
}

// LD (IY+N),N
static void op_FD_36(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_ORP_N_P00(cpu, cpu->regs.IY);
}

// *SCF
static void op_FD_37(Z80Cpu* cpu) {
    Z80CpuCore::op_SCF(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *JR C,O
static void op_FD_38(Z80Cpu* cpu) {
    Z80CpuCore::op_JR_CC(cpu, Z80CpuCore::cc_C);
    Z80CpuCore::do_PREF_00(cpu);
}

// ADD IY,SP
static void op_FD_39(Z80Cpu* cpu) {
    Z80CpuCore::op_ADD_RP_RP(cpu, &(cpu->regs.IY), cpu->regs.SP);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD A,(NN)
static void op_FD_3A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_A_MNN(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *DEC SP
static void op_FD_3B(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_RP(cpu, Z80CpuCore::do_DEC_16, &(cpu->regs.SP));
    Z80CpuCore::do_PREF_00(cpu);
}

// *INC A
static void op_FD_3C(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_INC_8, &(cpu->regs.A));
    Z80CpuCore::do_PREF_00(cpu);
}

// *DEC A
static void op_FD_3D(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_R(cpu, Z80CpuCore::do_DEC_8, &(cpu->regs.A));
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD A,N
static void op_FD_3E(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_N(cpu, &(cpu->regs.A));
    Z80CpuCore::do_PREF_00(cpu);
}

// *CCF
static void op_FD_3F(Z80Cpu* cpu) {
    Z80CpuCore::op_CCF(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0x4n
//

// *LD B,B
static void op_FD_40(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD B,C
static void op_FD_41(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD B,D
static void op_FD_42(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD B,E
static void op_FD_43(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD B,IYH
static void op_FD_44(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD B,IYL
static void op_FD_45(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD B,(IY+N)
static void op_FD_46(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.B), cpu->regs.IY);
}

// *LD B,A
static void op_FD_47(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.B), cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD C,B
static void op_FD_48(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD C,C
static void op_FD_49(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD C,D
static void op_FD_4A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD C,E
static void op_FD_4B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD C,IYH
static void op_FD_4C(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD C,IYL
static void op_FD_4D(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD C,(IY+N)
static void op_FD_4E(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.C), cpu->regs.IY);
}

// *LD C,A
static void op_FD_4F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.C), cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0x5n
//

// *LD D,B
static void op_FD_50(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD D,C
static void op_FD_51(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD D,D
static void op_FD_52(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD D,E
static void op_FD_53(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD D,IYH
static void op_FD_54(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD D,IYL
static void op_FD_55(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD D,(IY+N)
static void op_FD_56(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.D), cpu->regs.IY);
}

// *LD D,A
static void op_FD_57(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.D), cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD E,B
static void op_FD_58(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD E,C
static void op_FD_59(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD E,D
static void op_FD_5A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD E,E
static void op_FD_5B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD E,IYH
static void op_FD_5C(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD E,IYL
static void op_FD_5D(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD E,(IY+N)
static void op_FD_5E(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.E), cpu->regs.IY);
}

// *LD E,A
static void op_FD_5F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.E), cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0x6n
//

// LD IYH,B
static void op_FD_60(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYH), cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYH,C
static void op_FD_61(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYH), cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYH,D
static void op_FD_62(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYH), cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYH,E
static void op_FD_63(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYH), cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYH,H
static void op_FD_64(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYH), cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYH,L
static void op_FD_65(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYH), cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD H,(IY+N)
static void op_FD_66(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.H), cpu->regs.IY);
}

// LD IYH,A
static void op_FD_67(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYH), cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYL,B
static void op_FD_68(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYL), cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYL,C
static void op_FD_69(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYL), cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYL,D
static void op_FD_6A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYL), cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYL,E
static void op_FD_6B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYL), cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYL,H
static void op_FD_6C(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYL), cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD IYL,L
static void op_FD_6D(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYL), cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD L,(IY+N)
static void op_FD_6E(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.L), cpu->regs.IY);
}

// LD IYL,A
static void op_FD_6F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.IYL), cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0x7n
//

// LD (IY+N),B
static void op_FD_70(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_ORP_R_P00(cpu, cpu->regs.IY, cpu->regs.B);
}

// LD (IY+N),C
static void op_FD_71(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_ORP_R_P00(cpu, cpu->regs.IY, cpu->regs.C);
}

// LD (IY+N),D
static void op_FD_72(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_ORP_R_P00(cpu, cpu->regs.IY, cpu->regs.D);
}

// LD (IY+N),E
static void op_FD_73(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_ORP_R_P00(cpu, cpu->regs.IY, cpu->regs.E);
}

// LD (IY+N),H
static void op_FD_74(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_ORP_R_P00(cpu, cpu->regs.IY, cpu->regs.H);
}

// LD (IY+N),L
static void op_FD_75(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_ORP_R_P00(cpu, cpu->regs.IY, cpu->regs.L);
}

// *HALT
static void op_FD_76(Z80Cpu* cpu) {
    Z80CpuCore::op_HALT(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD (IY+N),A
static void op_FD_77(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_ORP_R_P00(cpu, cpu->regs.IY, cpu->regs.A);
}

// *LD A,B
static void op_FD_78(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD A,C
static void op_FD_79(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD A,D
static void op_FD_7A(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *LD A,E
static void op_FD_7B(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD A,IYH
static void op_FD_7C(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD A,IYL
static void op_FD_7D(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD A,(IY+N)
static void op_FD_7E(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_ORP_P00(cpu, &(cpu->regs.A), cpu->regs.IY);
}

// *LD A,A
static void op_FD_7F(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_R_R(cpu, &(cpu->regs.A), cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0x8n
//

// *ADD A,B
static void op_FD_80(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADD_8, cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *ADD A,C
static void op_FD_81(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADD_8, cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *ADD A,D
static void op_FD_82(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADD_8, cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *ADD A,E
static void op_FD_83(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADD_8, cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// ADD A,IYH
static void op_FD_84(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADD_8, cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// ADD A,IYL
static void op_FD_85(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADD_8, cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// ADD A,(IY+N)
static void op_FD_86(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_ORP_P00(cpu, Z80CpuCore::do_ADD_8, cpu->regs.IY);
}

// *ADD A,A
static void op_FD_87(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADD_8, cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

// *ADC A,B
static void op_FD_88(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADC_8, cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *ADC A,C
static void op_FD_89(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADC_8, cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *ADC A,D
static void op_FD_8A(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADC_8, cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *ADC A,E
static void op_FD_8B(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADC_8, cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// ADC A,IYH
static void op_FD_8C(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADC_8, cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// ADC A,IYL
static void op_FD_8D(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADC_8, cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// ADC A,(IY+N)
static void op_FD_8E(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_ORP_P00(cpu, Z80CpuCore::do_ADC_8, cpu->regs.IY);
}

// *ADC A,A
static void op_FD_8F(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_ADC_8, cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0x9n
//

// *SUB B
static void op_FD_90(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SUB_8, cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *SUB C
static void op_FD_91(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SUB_8, cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *SUB D
static void op_FD_92(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SUB_8, cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *SUB E
static void op_FD_93(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SUB_8, cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// SUB IYH
static void op_FD_94(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SUB_8, cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// SUB IYL
static void op_FD_95(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SUB_8, cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// SUB (IY+N)
static void op_FD_96(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_ORP_P00(cpu, Z80CpuCore::do_SUB_8, cpu->regs.IY);
}

// *SUB A
static void op_FD_97(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SUB_8, cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

// *SBC A,B
static void op_FD_98(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SBC_8, cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *SBC A,C
static void op_FD_99(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SBC_8, cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *SBC A,D
static void op_FD_9A(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SBC_8, cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *SBC A,E
static void op_FD_9B(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SBC_8, cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// SBC A,IYH
static void op_FD_9C(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SBC_8, cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// SBC A,IYL
static void op_FD_9D(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SBC_8, cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// SBC A,(IY+N)
static void op_FD_9E(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_ORP_P00(cpu, Z80CpuCore::do_SBC_8, cpu->regs.IY);
}

// *SBC A,A
static void op_FD_9F(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_SBC_8, cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0xAn
//

// *AND B
static void op_FD_A0(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_AND_8, cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *AND C
static void op_FD_A1(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_AND_8, cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *AND D
static void op_FD_A2(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_AND_8, cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *AND E
static void op_FD_A3(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_AND_8, cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// AND IYH
static void op_FD_A4(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_AND_8, cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// AND IYL
static void op_FD_A5(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_AND_8, cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// AND (IY+N)
static void op_FD_A6(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_ORP_P00(cpu, Z80CpuCore::do_AND_8, cpu->regs.IY);
}

// *AND A
static void op_FD_A7(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_AND_8, cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

// *XOR B
static void op_FD_A8(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_XOR_8, cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *XOR C
static void op_FD_A9(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_XOR_8, cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *XOR D
static void op_FD_AA(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_XOR_8, cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *XOR E
static void op_FD_AB(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_XOR_8, cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// XOR IYH
static void op_FD_AC(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_XOR_8, cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// XOR IYL
static void op_FD_AD(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_XOR_8, cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// XOR (IY+N)
static void op_FD_AE(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_ORP_P00(cpu, Z80CpuCore::do_XOR_8, cpu->regs.IY);
}

// *XOR A
static void op_FD_AF(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_XOR_8, cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0xBn
//

// *OR B
static void op_FD_B0(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_OR_8, cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *OR C
static void op_FD_B1(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_OR_8, cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *OR D
static void op_FD_B2(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_OR_8, cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *OR E
static void op_FD_B3(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_OR_8, cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// OR IYH
static void op_FD_B4(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_OR_8, cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// OR IYL
static void op_FD_B5(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_OR_8, cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// OR (IY+N)
static void op_FD_B6(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_ORP_P00(cpu, Z80CpuCore::do_OR_8, cpu->regs.IY);
}

// *OR A
static void op_FD_B7(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_OR_8, cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CP B
static void op_FD_B8(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_CP_8, cpu->regs.B);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CP C
static void op_FD_B9(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_CP_8, cpu->regs.C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CP D
static void op_FD_BA(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_CP_8, cpu->regs.D);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CP E
static void op_FD_BB(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_CP_8, cpu->regs.E);
    Z80CpuCore::do_PREF_00(cpu);
}

// CP IYH
static void op_FD_BC(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_CP_8, cpu->regs.IYH);
    Z80CpuCore::do_PREF_00(cpu);
}

// CP IYL
static void op_FD_BD(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_CP_8, cpu->regs.IYL);
    Z80CpuCore::do_PREF_00(cpu);
}

// CP (IY+N)
static void op_FD_BE(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_ORP_P00(cpu, Z80CpuCore::do_CP_8, cpu->regs.IY);
}

// *CP A
static void op_FD_BF(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_R(cpu, Z80CpuCore::do_CP_8, cpu->regs.A);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0xCn
//

// *RET NZ
static void op_FD_C0(Z80Cpu* cpu) {
    Z80CpuCore::op_RET_CC(cpu, Z80CpuCore::cc_NZ);
    Z80CpuCore::do_PREF_00(cpu);
}

// *POP BC
static void op_FD_C1(Z80Cpu* cpu) {
    Z80CpuCore::op_POP_RP(cpu, &(cpu->regs.BC));
    Z80CpuCore::do_PREF_00(cpu);
}

// *JP NZ,NN
static void op_FD_C2(Z80Cpu* cpu) {
    Z80CpuCore::op_JP_CC(cpu, Z80CpuCore::cc_NZ);
    Z80CpuCore::do_PREF_00(cpu);
}

// *JP NN
static void op_FD_C3(Z80Cpu* cpu) {
    Z80CpuCore::op_JP(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CALL NZ,NN
static void op_FD_C4(Z80Cpu* cpu) {
    Z80CpuCore::op_CALL_CC(cpu, Z80CpuCore::cc_NZ);
    Z80CpuCore::do_PREF_00(cpu);
}

// *PUSH BC
static void op_FD_C5(Z80Cpu* cpu) {
    Z80CpuCore::op_PUSH_RP(cpu, cpu->regs.BC);
    Z80CpuCore::do_PREF_00(cpu);
}

// *ADD A,N
static void op_FD_C6(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_N(cpu, Z80CpuCore::do_ADD_8);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RST #00
static void op_FD_C7(Z80Cpu* cpu) {
    Z80CpuCore::op_RST(cpu, 0x00);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RET Z
static void op_FD_C8(Z80Cpu* cpu) {
    Z80CpuCore::op_RET_CC(cpu, Z80CpuCore::cc_Z);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RET
static void op_FD_C9(Z80Cpu* cpu) {
    Z80CpuCore::op_RET(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *JP Z,NN
static void op_FD_CA(Z80Cpu* cpu) {
    Z80CpuCore::op_JP_CC(cpu, Z80CpuCore::cc_Z);
    Z80CpuCore::do_PREF_00(cpu);
}

// prefix #FD_CB
static void op_FD_CB(Z80Cpu* cpu) {
    Z80CpuCore::op_PREF_XX_CB(cpu, z80CpuOptable_FD_CB);
}

// *CALL Z,NN
static void op_FD_CC(Z80Cpu* cpu) {
    Z80CpuCore::op_CALL_CC(cpu, Z80CpuCore::cc_Z);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CALL NN
static void op_FD_CD(Z80Cpu* cpu) {
    Z80CpuCore::op_CALL(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *ADC A,N
static void op_FD_CE(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_N(cpu, Z80CpuCore::do_ADC_8);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RST #08
static void op_FD_CF(Z80Cpu* cpu) {
    Z80CpuCore::op_RST(cpu, 0x08);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0xDn
//

// *RET NC
static void op_FD_D0(Z80Cpu* cpu) {
    Z80CpuCore::op_RET_CC(cpu, Z80CpuCore::cc_NC);
    Z80CpuCore::do_PREF_00(cpu);
}

// *POP DE
static void op_FD_D1(Z80Cpu* cpu) {
    Z80CpuCore::op_POP_RP(cpu, &(cpu->regs.DE));
    Z80CpuCore::do_PREF_00(cpu);
}

// *JP NC,NN
static void op_FD_D2(Z80Cpu* cpu) {
    Z80CpuCore::op_JP_CC(cpu, Z80CpuCore::cc_NC);
    Z80CpuCore::do_PREF_00(cpu);
}

// *OUT (N),A
static void op_FD_D3(Z80Cpu* cpu) {
    Z80CpuCore::op_OUT_N_A(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CALL NC,NN
static void op_FD_D4(Z80Cpu* cpu) {
    Z80CpuCore::op_CALL_CC(cpu, Z80CpuCore::cc_NC);
    Z80CpuCore::do_PREF_00(cpu);
}

// *PUSH DE
static void op_FD_D5(Z80Cpu* cpu) {
    Z80CpuCore::op_PUSH_RP(cpu, cpu->regs.DE);
    Z80CpuCore::do_PREF_00(cpu);
}

// *SUB N
static void op_FD_D6(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_N(cpu, Z80CpuCore::do_SUB_8);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RST #10
static void op_FD_D7(Z80Cpu* cpu) {
    Z80CpuCore::op_RST(cpu, 0x10);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RET C
static void op_FD_D8(Z80Cpu* cpu) {
    Z80CpuCore::op_RET_CC(cpu, Z80CpuCore::cc_C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *EXX
static void op_FD_D9(Z80Cpu* cpu) {
    Z80CpuCore::op_EXX(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *JP C,NN
static void op_FD_DA(Z80Cpu* cpu) {
    Z80CpuCore::op_JP_CC(cpu, Z80CpuCore::cc_C);
    Z80CpuCore::do_PREF_00(cpu);
}

// *IN A,(N)
static void op_FD_DB(Z80Cpu* cpu) {
    Z80CpuCore::op_IN_A_N(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CALL C,NN
static void op_FD_DC(Z80Cpu* cpu) {
    Z80CpuCore::op_CALL_CC(cpu, Z80CpuCore::cc_C);
    Z80CpuCore::do_PREF_00(cpu);
}

// prefix #DD
static void op_FD_DD(Z80Cpu* cpu) {
    Z80CpuCore::op_PREF_DD(cpu);
}

// *SBC A,N
static void op_FD_DE(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_N(cpu, Z80CpuCore::do_SBC_8);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RST #18
static void op_FD_DF(Z80Cpu* cpu) {
    Z80CpuCore::op_RST(cpu, 0x18);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0xEn
//

// *RET PO
static void op_FD_E0(Z80Cpu* cpu) {
    Z80CpuCore::op_RET_CC(cpu, Z80CpuCore::cc_PO);
    Z80CpuCore::do_PREF_00(cpu);
}

// POP IY
static void op_FD_E1(Z80Cpu* cpu) {
    Z80CpuCore::op_POP_RP(cpu, &(cpu->regs.IY));
    Z80CpuCore::do_PREF_00(cpu);
}

// *JP PO,NN
static void op_FD_E2(Z80Cpu* cpu) {
    Z80CpuCore::op_JP_CC(cpu, Z80CpuCore::cc_PO);
    Z80CpuCore::do_PREF_00(cpu);
}

// EX (SP),IY
static void op_FD_E3(Z80Cpu* cpu) {
    Z80CpuCore::op_EX_MSP_RP(cpu, cpu->regs.IYL, cpu->regs.IYH, &(cpu->regs.IY));
    Z80CpuCore::do_PREF_00(cpu);
}

// *CALL PO,NN
static void op_FD_E4(Z80Cpu* cpu) {
    Z80CpuCore::op_CALL_CC(cpu, Z80CpuCore::cc_PO);
    Z80CpuCore::do_PREF_00(cpu);
}

// PUSH IY
static void op_FD_E5(Z80Cpu* cpu) {
    Z80CpuCore::op_PUSH_RP(cpu, cpu->regs.IY);
    Z80CpuCore::do_PREF_00(cpu);
}

// *AND N
static void op_FD_E6(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_N(cpu, Z80CpuCore::do_AND_8);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RST #20
static void op_FD_E7(Z80Cpu* cpu) {
    Z80CpuCore::op_RST(cpu, 0x20);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RET PE
static void op_FD_E8(Z80Cpu* cpu) {
    Z80CpuCore::op_RET_CC(cpu, Z80CpuCore::cc_PE);
    Z80CpuCore::do_PREF_00(cpu);
}

// JP IY
static void op_FD_E9(Z80Cpu* cpu) {
    Z80CpuCore::op_JP_RP(cpu, cpu->regs.IY);
    Z80CpuCore::do_PREF_00(cpu);
}

// *JP PE,NN
static void op_FD_EA(Z80Cpu* cpu) {
    Z80CpuCore::op_JP_CC(cpu, Z80CpuCore::cc_PE);
    Z80CpuCore::do_PREF_00(cpu);
}

// *EX DE,HL
static void op_FD_EB(Z80Cpu* cpu) {
    Z80CpuCore::op_EX_DE_HL(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CALL PE,NN
static void op_FD_EC(Z80Cpu* cpu) {
    Z80CpuCore::op_CALL_CC(cpu, Z80CpuCore::cc_PE);
    Z80CpuCore::do_PREF_00(cpu);
}

// prefix #ED
static void op_FD_ED(Z80Cpu* cpu) {
    Z80CpuCore::op_PREF_ED(cpu);
}

// *XOR N
static void op_FD_EE(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_N(cpu, Z80CpuCore::do_XOR_8);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RST #28
static void op_FD_EF(Z80Cpu* cpu) {
    Z80CpuCore::op_RST(cpu, 0x28);
    Z80CpuCore::do_PREF_00(cpu);
}

//
// 0xFn
//

// *RET P
static void op_FD_F0(Z80Cpu* cpu) {
    Z80CpuCore::op_RET_CC(cpu, Z80CpuCore::cc_P);
    Z80CpuCore::do_PREF_00(cpu);
}

// *POP AF
static void op_FD_F1(Z80Cpu* cpu) {
    Z80CpuCore::op_POP_RP(cpu, &(cpu->regs.AF));
    Z80CpuCore::do_PREF_00(cpu);
}

// *JP P,NN
static void op_FD_F2(Z80Cpu* cpu) {
    Z80CpuCore::op_JP_CC(cpu, Z80CpuCore::cc_P);
    Z80CpuCore::do_PREF_00(cpu);
}

// *DI
static void op_FD_F3(Z80Cpu* cpu) {
    Z80CpuCore::op_DI(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CALL P,NN
static void op_FD_F4(Z80Cpu* cpu) {
    Z80CpuCore::op_CALL_CC(cpu, Z80CpuCore::cc_P);
    Z80CpuCore::do_PREF_00(cpu);
}

// *PUSH AF
static void op_FD_F5(Z80Cpu* cpu) {
    Z80CpuCore::op_PUSH_RP(cpu, cpu->regs.AF);
    Z80CpuCore::do_PREF_00(cpu);
}

// *OR N
static void op_FD_F6(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_N(cpu, Z80CpuCore::do_OR_8);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RST #30
static void op_FD_F7(Z80Cpu* cpu) {
    Z80CpuCore::op_RST(cpu, 0x30);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RET M
static void op_FD_F8(Z80Cpu* cpu) {
    Z80CpuCore::op_RET_CC(cpu, Z80CpuCore::cc_M);
    Z80CpuCore::do_PREF_00(cpu);
}

// LD SP,IY
static void op_FD_F9(Z80Cpu* cpu) {
    Z80CpuCore::op_LD_SP_RP(cpu, cpu->regs.IY);
    Z80CpuCore::do_PREF_00(cpu);
}

// *JP M,NN
static void op_FD_FA(Z80Cpu* cpu) {
    Z80CpuCore::op_JP_CC(cpu, Z80CpuCore::cc_M);
    Z80CpuCore::do_PREF_00(cpu);
}

// *EI
static void op_FD_FB(Z80Cpu* cpu) {
    Z80CpuCore::op_EI(cpu);
    Z80CpuCore::do_PREF_00(cpu);
}

// *CALL M,NN
static void op_FD_FC(Z80Cpu* cpu) {
    Z80CpuCore::op_CALL_CC(cpu, Z80CpuCore::cc_M);
    Z80CpuCore::do_PREF_00(cpu);
}

// prefix #FD
static void op_FD_FD(Z80Cpu* cpu) {
    Z80CpuCore::op_PREF_FD(cpu);
}

// *CP N
static void op_FD_FE(Z80Cpu* cpu) {
    Z80CpuCore::op_DO_A_N(cpu, Z80CpuCore::do_CP_8);
    Z80CpuCore::do_PREF_00(cpu);
}

// *RST #38
static void op_FD_FF(Z80Cpu* cpu) {
    Z80CpuCore::op_RST(cpu, 0x38);
    Z80CpuCore::do_PREF_00(cpu);
}

Z80CpuOpcode z80CpuOptable_FD[0x100] = {
    op_FD_00, op_FD_01, op_FD_02, op_FD_03, op_FD_04, op_FD_05, op_FD_06, op_FD_07,
    op_FD_08, op_FD_09, op_FD_0A, op_FD_0B, op_FD_0C, op_FD_0D, op_FD_0E, op_FD_0F,
    op_FD_10, op_FD_11, op_FD_12, op_FD_13, op_FD_14, op_FD_15, op_FD_16, op_FD_17,
    op_FD_18, op_FD_19, op_FD_1A, op_FD_1B, op_FD_1C, op_FD_1D, op_FD_1E, op_FD_1F,
    op_FD_20, op_FD_21, op_FD_22, op_FD_23, op_FD_24, op_FD_25, op_FD_26, op_FD_27,
    op_FD_28, op_FD_29, op_FD_2A, op_FD_2B, op_FD_2C, op_FD_2D, op_FD_2E, op_FD_2F,
    op_FD_30, op_FD_31, op_FD_32, op_FD_33, op_FD_34, op_FD_35, op_FD_36, op_FD_37,
    op_FD_38, op_FD_39, op_FD_3A, op_FD_3B, op_FD_3C, op_FD_3D, op_FD_3E, op_FD_3F,
    op_FD_40, op_FD_41, op_FD_42, op_FD_43, op_FD_44, op_FD_45, op_FD_46, op_FD_47,
    op_FD_48, op_FD_49, op_FD_4A, op_FD_4B, op_FD_4C, op_FD_4D, op_FD_4E, op_FD_4F,
    op_FD_50, op_FD_51, op_FD_52, op_FD_53, op_FD_54, op_FD_55, op_FD_56, op_FD_57,
    op_FD_58, op_FD_59, op_FD_5A, op_FD_5B, op_FD_5C, op_FD_5D, op_FD_5E, op_FD_5F,
    op_FD_60, op_FD_61, op_FD_62, op_FD_63, op_FD_64, op_FD_65, op_FD_66, op_FD_67,
    op_FD_68, op_FD_69, op_FD_6A, op_FD_6B, op_FD_6C, op_FD_6D, op_FD_6E, op_FD_6F,
    op_FD_70, op_FD_71, op_FD_72, op_FD_73, op_FD_74, op_FD_75, op_FD_76, op_FD_77,
    op_FD_78, op_FD_79, op_FD_7A, op_FD_7B, op_FD_7C, op_FD_7D, op_FD_7E, op_FD_7F,
    op_FD_80, op_FD_81, op_FD_82, op_FD_83, op_FD_84, op_FD_85, op_FD_86, op_FD_87,
    op_FD_88, op_FD_89, op_FD_8A, op_FD_8B, op_FD_8C, op_FD_8D, op_FD_8E, op_FD_8F,
    op_FD_90, op_FD_91, op_FD_92, op_FD_93, op_FD_94, op_FD_95, op_FD_96, op_FD_97,
    op_FD_98, op_FD_99, op_FD_9A, op_FD_9B, op_FD_9C, op_FD_9D, op_FD_9E, op_FD_9F,
    op_FD_A0, op_FD_A1, op_FD_A2, op_FD_A3, op_FD_A4, op_FD_A5, op_FD_A6, op_FD_A7,
    op_FD_A8, op_FD_A9, op_FD_AA, op_FD_AB, op_FD_AC, op_FD_AD, op_FD_AE, op_FD_AF,
    op_FD_B0, op_FD_B1, op_FD_B2, op_FD_B3, op_FD_B4, op_FD_B5, op_FD_B6, op_FD_B7,
    op_FD_B8, op_FD_B9, op_FD_BA, op_FD_BB, op_FD_BC, op_FD_BD, op_FD_BE, op_FD_BF,
    op_FD_C0, op_FD_C1, op_FD_C2, op_FD_C3, op_FD_C4, op_FD_C5, op_FD_C6, op_FD_C7,
    op_FD_C8, op_FD_C9, op_FD_CA, op_FD_CB, op_FD_CC, op_FD_CD, op_FD_CE, op_FD_CF,
    op_FD_D0, op_FD_D1, op_FD_D2, op_FD_D3, op_FD_D4, op_FD_D5, op_FD_D6, op_FD_D7,
    op_FD_D8, op_FD_D9, op_FD_DA, op_FD_DB, op_FD_DC, op_FD_DD, op_FD_DE, op_FD_DF,
    op_FD_E0, op_FD_E1, op_FD_E2, op_FD_E3, op_FD_E4, op_FD_E5, op_FD_E6, op_FD_E7,
    op_FD_E8, op_FD_E9, op_FD_EA, op_FD_EB, op_FD_EC, op_FD_ED, op_FD_EE, op_FD_EF,
    op_FD_F0, op_FD_F1, op_FD_F2, op_FD_F3, op_FD_F4, op_FD_F5, op_FD_F6, op_FD_F7,
    op_FD_F8, op_FD_F9, op_FD_FA, op_FD_FB, op_FD_FC, op_FD_FD, op_FD_FE, op_FD_FF
};

}
