#ifndef ZEMUX__Z80__Z80CPU
#define ZEMUX__Z80__Z80CPU

#include <cstdint>
#include <zemux_core/config.h>
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/unroll.h>

namespace zemux {

class Z80CpuCallback {
public:

    virtual ~Z80CpuCallback() = default;

    virtual uint8_t onZ80MreqRd(uint16_t address, bool isM1) = 0;
    virtual void onZ80MreqWr(uint16_t address, uint8_t value) = 0;
    virtual uint8_t onZ80IorqRd(uint16_t port) = 0;
    virtual void onZ80IorqWr(uint16_t port, uint8_t value) = 0;

    // From "Z80 CPU User Manual":
    //
    // IORQ is also generated concurrently with M1 during an interrupt acknowledge cycle to indicate
    // that an interrupt response vector can be placed on the data bus.
    virtual uint8_t onZ80IorqM1() {
        return 0xFF;
    }

    // To handle contended memory with original ULA.
    virtual void onZ80AddressOnBus(uint16_t address) {
    }
};

struct Z80CpuRegs {
    union {
        uint16_t BC = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t B; uint8_t C; };
        #else
        struct { uint8_t C; uint8_t B; };
        #endif
    };

    union {
        uint16_t DE = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t D; uint8_t E; };
        #else
        struct { uint8_t E; uint8_t D; };
        #endif
    };

    union {
        uint16_t HL = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t H; uint8_t L; };
        #else
        struct { uint8_t L; uint8_t H; };
        #endif
    };

    union {
        uint16_t AF = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t A; uint8_t F; };
        #else
        struct { uint8_t F; uint8_t A; };
        #endif
    };

    union {
        uint16_t IX = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t IXH; uint8_t IXL; };
        #else
        struct { uint8_t IXL; uint8_t IXH; };
        #endif
    };

    union {
        uint16_t IY = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t IYH; uint8_t IYL; };
        #else
        struct { uint8_t IYL; uint8_t IYH; };
        #endif
    };

    union {
        uint16_t SP = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t SPH; uint8_t SPL; };
        #else
        struct { uint8_t SPL; uint8_t SPH; };
        #endif
    };

    union {
        uint16_t PC = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t PCH; uint8_t PCL; };
        #else
        struct { uint8_t PCL; uint8_t PCH; };
        #endif
    };

    union {
        uint16_t MP = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t MPH; uint8_t MPL; };
        #else
        struct { uint8_t MPL; uint8_t MPH; };
        #endif
    };

    union {
        uint16_t BC_ = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t B_; uint8_t C_; };
        #else
        struct { uint8_t C_; uint8_t B_; };
        #endif
    };

    union {
        uint16_t DE_ = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t D_; uint8_t E_; };
        #else
        struct { uint8_t E_; uint8_t D_; };
        #endif
    };

    union {
        uint16_t HL_ = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t H_; uint8_t L_; };
        #else
        struct { uint8_t L_; uint8_t H_; };
        #endif
    };

    union {
        uint16_t AF_ = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t A_; uint8_t F_; };
        #else
        struct { uint8_t A_; uint8_t F_; };
        #endif
    };

    union {
        uint16_t IR = 0;

        #ifdef ZEMUX_BIG_ENDIAN
        struct { uint8_t I; uint8_t R; };
        #else
        struct { uint8_t R; uint8_t I; };
        #endif
    };

    bool IFF1 = false;
    bool IFF2 = false;
    int IM = 0;
};

using Z80CpuOpcode = void (Z80Cpu::*)();

class Z80Cpu final : private NonCopyable {
public:

    static constexpr unsigned int FLAG_C = 0x01; // carry
    static constexpr unsigned int FLAG_N = 0x02; // subtract
    static constexpr unsigned int FLAG_PV = 0x04; // parity or overflow
    static constexpr unsigned int FLAG_3 = 0x08; // copy of bit 3
    static constexpr unsigned int FLAG_H = 0x10; // half carry
    static constexpr unsigned int FLAG_5 = 0x20; // copy of bit 5
    static constexpr unsigned int FLAG_Z = 0x40; // zero
    static constexpr unsigned int FLAG_S = 0x80; // sign

    Z80Cpu(Z80CpuCallback* cb);

    Z80CpuRegs regs;

    ZEMUX_FORCE_INLINE bool isIntPossible() {
        return regs.IFF1 && !isProcessingInstruction && !prefix;
    }

    ZEMUX_FORCE_INLINE bool isNmiPossible() {
        return !isProcessingInstruction && !prefix;
    }

    ZEMUX_FORCE_INLINE void waitTicks(unsigned int ticksToWait) {
        ticks += ticksToWait;
    }

    ZEMUX_FORCE_INLINE int getClockRatio() {
        return clockRatio;
    }

    void step();
    void reset();
    void doInt();
    void doNmi();
    unsigned int getTicks();
    void consumeTicks(unsigned int consumedTicks);
    void setClockRatio(int newClockRatio);

private:

    static constexpr unsigned int FLAG_C_M16 = 0x10000;
    static constexpr unsigned int FLAG_C_S16 = 0x10;
    static constexpr unsigned int FLAG_C_M8 = 0x100;
    static constexpr unsigned int FLAG_C_S8 = 8;

    static constexpr unsigned int FLAG_H_M16 = 0x1000;
    static constexpr unsigned int FLAG_H_S16 = 8;

    static constexpr unsigned int FLAG_PV_C8 = 0x80;
    static constexpr unsigned int FLAG_PV_IFF_S8 = 2;

    static constexpr unsigned int FLAG_C_TO_H = 4;
    static constexpr unsigned int FLAG_H_TO_C = 4;
    static constexpr unsigned int FLAG_N_TO_5 = 4;
    static constexpr unsigned int FLAG_S_TO_N = 6;

    static uint8_t parityLookup[0x100];

    static void initSharedData();

    Z80CpuCallback* cb;
    Z80CpuOpcode* optable;

    bool isHalted;
    bool shouldResetPv;
    bool isProcessingInstruction;
    uint16_t pcIncrement = 1;
    uint8_t prefix;
    int8_t lastOffset;
    unsigned int tstate = 0;
    unsigned int ticks = 0;
    int clockRatio = 0;

    // Utilities

    ZEMUX_FORCE_INLINE void incR() {
        regs.R = (regs.R & 0x80) | ((regs.R + 1) & 0x7F);
    }

    ZEMUX_FORCE_INLINE uint8_t fetchOpcode() {
        uint8_t result = cb->onZ80MreqRd(regs.PC++, true);
        incR();
        tstate += 4;

        return result;
    }

    ZEMUX_FORCE_INLINE uint8_t fetchIntVec() {
        // From "Z80 CPU User Manual":
        //
        // Two wait states are automatically added to this cycle.
        // These states are added so that a ripple priority interrupt scheme can be easily implemented.

        uint8_t result = cb->onZ80IorqM1();
        incR();
        tstate += 6;

        return result;
    }

    ZEMUX_FORCE_INLINE uint8_t fetchByte() {
        uint8_t result = cb->onZ80MreqRd(regs.PC, false);
        regs.PC += pcIncrement;
        tstate += 3;
        return result;
    }

    ZEMUX_FORCE_INLINE int8_t fetchOffsetMp(uint16_t rp) {
        int8_t offset = static_cast<int8_t>(fetchByte());
        regs.MP = rp + offset;
        return offset;
    }

    ZEMUX_FORCE_INLINE uint16_t fetchWord() {
        uint8_t value = fetchByte();
        return value | (static_cast<uint16_t>(fetchByte()) << 8);
    }

    ZEMUX_FORCE_INLINE uint8_t memoryRead(uint16_t address) {
        uint8_t value = cb->onZ80MreqRd(address, false);
        tstate += 3;
        return value;
    }

    ZEMUX_FORCE_INLINE void memoryWrite(uint16_t address, uint8_t value) {
        cb->onZ80MreqWr(address, value);
        tstate += 3;
    }

    ZEMUX_FORCE_INLINE uint8_t ioRead(uint16_t port) {
        // From "Z80 CPU User Manual":
        // During I/O operations, a single wait state is automatically inserted.

        putAddressOnBus(port);
        uint8_t value = cb->onZ80IorqRd(port);
        tstate += 3;
        return value;
    }

    ZEMUX_FORCE_INLINE void ioWrite(uint16_t port, uint8_t value) {
        // From "Z80 CPU User Manual":
        // During I/O operations, a single wait state is automatically inserted.

        putAddressOnBus(port);
        cb->onZ80IorqWr(port, value);
        tstate += 3;
    }

    ZEMUX_FORCE_INLINE void putAddressOnBus(uint16_t address) {
        cb->onZ80AddressOnBus(address);
        ++tstate;
    }

    // Instructions (4 cycles)

    ZEMUX_FORCE_INLINE void op_NOP() {
    }

    ZEMUX_FORCE_INLINE void op_LD_R_R(uint8_t* r1, uint8_t r2) {
        *r1 = r2;
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_DO_A_R(F block, uint8_t r) {
        block(&regs.A, r);
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_DO_R(F block, uint8_t* r) {
        block(r);
    }

    ZEMUX_FORCE_INLINE void op_EXX() {
        uint8_t value = regs.BC;
        regs.BC = regs.BC_;
        regs.BC_ = value;

        value = regs.DE;
        regs.DE = regs.DE_;
        regs.DE_ = value;

        value = regs.HL;
        regs.HL = regs.HL_;
        regs.HL_ = value;
    }

    ZEMUX_FORCE_INLINE void op_EX_AF_AF_() {
        uint16_t value = regs.AF;
        regs.AF = regs.AF_;
        regs.AF_ = value;
    }

    ZEMUX_FORCE_INLINE void op_EX_DE_HL() {
        uint16_t value = regs.DE;
        regs.DE = regs.HL;
        regs.HL = value;
    }

    ZEMUX_FORCE_INLINE void op_DAA() {
        uint8_t value = regs.A;

        if (regs.F & FLAG_N) {
            if ((regs.F & FLAG_H) || (regs.A & 0x0F) > 9) {
                value -= 6;
            }

            if ((regs.F & FLAG_C) || regs.A > 0x99) {
                value -= 0x60;
            }
        } else {
            if ((regs.F & FLAG_H) || (regs.A & 0x0F) > 9) {
                value += 6;
            }

            if ((regs.F & FLAG_C) || regs.A > 0x99) {
                value += 0x60;
            }
        }

        regs.F = (regs.F & (FLAG_N | FLAG_C))
            | (value & (FLAG_S | FLAG_5 | FLAG_3))
            | (value ? 0 : FLAG_Z)
            | parityLookup[value]
            | ((value ^ regs.A) & FLAG_H)
            | (regs.A > 0x99 ? FLAG_C : 0);

        regs.A = value;
    }

    ZEMUX_FORCE_INLINE op_CPL() {
        regs.A ^= 0xFF;
        regs.F = (regs.F & (FLAG_C | FLAG_PV | FLAG_Z | FLAG_S)) | (regs.A & (FLAG_3 | FLAG_5)) | (FLAG_N | FLAG_H);
    }

    ZEMUX_FORCE_INLINE op_CCF() {
        regs.F = (regs.F & (FLAG_PV | FLAG_Z | FLAG_S))
            | ((regs.F & FLAG_C) << FLAG_C_TO_H)
            | ((regs.F & FLAG_C) ^ FLAG_C)
            | (regs.A & (FLAG_3 | FLAG_5));
    }

    ZEMUX_FORCE_INLINE op_SCF() {
        regs.F = (regs.F & (FLAG_PV | FLAG_Z | FLAG_S)) | (regs.A & (FLAG_5 | FLAG_3)) | FLAG_C;
    }

    ZEMUX_FORCE_INLINE op_DI() {
        regs.IFF1 = false;
        regs.IFF2 = false;
    }

    ZEMUX_FORCE_INLINE op_EI() {
        regs.IFF1 = true;
        regs.IFF2 = true;

        // From "The Z80 Family Program Interrupt Structure":
        //
        // The enable interrupt instruction (EI) will set both IFF1 and IFF2 to a logic 'one'
        // allowing recognition of any maskable interrupt at the completion of the instruction
        // following the EI instruction.
        isProcessingInstruction = true;
    }

    ZEMUX_FORCE_INLINE void op_RLA() {
        uint8_t value = regs.A;
        regs.A = (regs.A << 1) | (regs.F & FLAG_C);
        regs.F = (regs.F & (FLAG_PV | FLAG_Z | FLAG_S)) | (regs.A & (FLAG_3 | FLAG_5)) | (value >> 7);
    }

    ZEMUX_FORCE_INLINE void op_RRA() {
        uint8_t value = regs.A;
        regs.A = (regs.A >> 1) | (regs.F << 7);
        regs.F = (regs.F & (FLAG_PV | FLAG_Z | FLAG_S)) | (regs.A & (FLAG_3 | FLAG_5)) | (value & FLAG_C);
    }

    ZEMUX_FORCE_INLINE void op_RLCA() {
        regs.A = (regs.A << 1) | (regs.A >> 7);
        regs.F = (regs.F & (FLAG_PV | FLAG_Z | FLAG_S)) | (regs.A & (FLAG_C | FLAG_3 | FLAG_5));
    }

    ZEMUX_FORCE_INLINE void op_RRCA() {
        regs.F = (regs.F & (FLAG_PV | FLAG_Z | FLAG_S)) | (regs.A & FLAG_C);
        regs.A = (regs.A >> 1) | (regs.A << 7);
        regs.F |= (regs.A & (FLAG_3 | FLAG_5));
    }

    ZEMUX_FORCE_INLINE void op_JP_RP(uint16_t rp) {
        regs.PC = rp;
    }

    ZEMUX_FORCE_INLINE void op_PREF_CB() {
        prefix = 0xCB;
        // optable = optable_CB;
    }

    ZEMUX_FORCE_INLINE void op_PREF_DD() {
        prefix = 0xDD;
        // optable = optable_DD;
    }

    ZEMUX_FORCE_INLINE void op_PREF_ED() {
        prefix = 0xED;
        // optable = optable_ED;
    }

    ZEMUX_FORCE_INLINE void op_PREF_FD() {
        prefix = 0xFD;
        // optable = optable_FD;
    }

    ZEMUX_FORCE_INLINE void op_HALT() {
        isHalted = true;
        ++regs.PC;
    }

    // Instructions (6 cycles)

    template< typename F >
    ZEMUX_FORCE_INLINE void op_DO_RP(F block, uint16_t* rp) {
        block(rp);
        Unroll<2>::loop([&] { putAddressOnBus(regs.IR); });
    }

    ZEMUX_FORCE_INLINE void OP_LD_SP_RP(uint16_t rp) {
        regs.SP = rp;
        Unroll<2>::loop([&] { putAddressOnBus(regs.IR); });
    }

    // Instructions (7 cycles)

    ZEMUX_FORCE_INLINE void op_LD_R_N(uint8_t* r) {
        *r = memoryRead();
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_DO_A_N(F block) {
        block(&regs.A, fetchByte());
    }

    ZEMUX_FORCE_INLINE void op_LD_R_MRP(uint8_t* r, uint16_t rp) {
        *r = memoryRead(rp);
    }

    ZEMUX_FORCE_INLINE op_LD_MRP_R(uint16_t rp, uint8_t r) {
        memoryWrite(rp, r);
    }

    ZEMUX_FORCE_INLINE void op_LD_A_MRP(uint16_t rp) {
        regs.A = memoryRead(rp);
        regs.MP = rp + 1;
    }

    ZEMUX_FORCE_INLINE void op_LD_MRP_A(uint16_t rp) {
        regs.MPH = regs.A;
        regs.MPL = rp + 1;
        memoryWrite(rp, regs.A);
    }

    template< typename F >
    ZEMUX_FORCE_INLINE op_DO_A_MHL(F block) {
        block(&regs.A, memoryRead(regs.HL));
    }

    // Instructions (8 cycles)

    ZEMUX_FORCE_INLINE void op_BIT_R(int bit, uint8_t r) {
        uint8_t value = r & (1 << bit);

        regs.F = (regs.F & FLAG_C)
            | FLAG_H
            | (value & (FLAG_S | FLAG_5 | FLAG_3))
            | (value ? 0 : FLAG_Z)
            | parityLookup[value]
            | (r & (FLAG_5 | FLAG_3));
    }

    ZEMUX_FORCE_INLINE void op_SET_R(int bit, uint8_t* r) {
        *r |= (1 << bit);
    }

    ZEMUX_FORCE_INLINE void op_RES_R(int bit, uint8_t* r) {
        *r &= ~(1 << bit);
    }

    ZEMUX_FORCE_INLINE void op_NEG_P00() {
        uint8_t value = regs.A;
        regs.A = 0;
        do_SUB_8(&regs.A, value);
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_IM_P00(int mode) {
        regs.IM = mode;
        do_PREF_00();
    }

    // Instructions (9 cycles)

    ZEMUX_FORCE_INLINE void op_LD_A_I_P00() {
        regs.A = regs.I;

        regs.F = (regs.F & FLAG_C)
            | (regs.A & (FLAG_S | FLAG_5 | FLAG_3))
            | (regs.A ? 0 : FLAG_Z)
            | (regs.IFF2 << FLAG_PV_IFF_S8);

        putAddressOnBus(regs.IR);
        shouldResetPv = true;

        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_LD_A_RR_P00() {
        regs.A = regs.R;

        regs.F = (regs.F & FLAG_C)
            | (regs.A & (FLAG_S | FLAG_5 | FLAG_3))
            | (regs.A ? 0 : FLAG_Z)
            | (regs.IFF2 << FLAG_PV_IFF_S8);

        putAddressOnBus(regs.IR);
        shouldResetPv = true;

        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_LD_I_A_P00() {
        regs.I = regs.A;
        putAddressOnBus(regs.IR);
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_LD_RR_A_P00() {
        regs.R = regs.A;
        putAddressOnBus(regs.IR);
        do_PREF_00();
    }

    // Instructions (10 cycles)

    ZEMUX_FORCE_INLINE void op_LD_RP_NN(uint16_t* rp) {
        *rp = fetchWord();
    }

    ZEMUX_FORCE_INLINE void op_JP() {
        regs.PC = fetchWord();
        regs.MP = regs.PC;
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_JP_CC(F block) {
        uint16_t address = fetchWord();
        regs.MP = address;

        if (block()) {
            regs.PC = address;
        }
    }

    ZEMUX_FORCE_INLINE void op_LD_MHL_N() {
        memoryWrite(regs.HL, memoryRead());
    }

    ZEMUX_FORCE_INLINE void op_POP_RP(uint16_t* rp) {
        *rp = do_POP();
    }

    ZEMUX_FORCE_INLINE void op_RET() {
        do_RET();
    }

    // Instructions (11 cycles)

    ZEMUX_FORCE_INLINE void op_ADD_RP_RP(uint16_t* rp1, uint16_t rp2) {
        do_ADD_16(&rp1, rp2);
        Unroll<7>::loop([&] { putAddressOnBus(regs.IR); });
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_DO_MHL(F block) {
        uint8_t value = block(memoryRead(regs.HL));
        putAddressOnBus(regs.HL);
        memoryWrite(regs.HL, value);
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_RET_CC(F block) {
        putAddressOnBus(regs.IR);

        if (block()) {
            do_RET();
        }
    }

    ZEMUX_FORCE_INLINE void op_PUSH_RP(uint16_t rp) {
        putAddressOnBus(regs.IR);
        do_PUSH(rp);
    }

    ZEMUX_FORCE_INLINE op_RST(uint16_t address) {
        putAddressOnBus(regs.IR);
        do_PUSH(regs.PC);
        regs.PC = address;
        regs.MP = address;
    }

    ZEMUX_FORCE_INLINE void op_IN_A_N() {
        uint16_t port = (static_cast<uint16_t>(regs.A) << 8) | fetchByte();
        regs.A = do_IN(port);
    }

    ZEMUX_FORCE_INLINE void op_OUT_N_A() {
        uint16_t port = (static_cast<uint16_t>(regs.A) << 8) | fetchByte();
        do_OUT(port, regs.A);
    }

    // Instructions (12 cycles)

    ZEMUX_FORCE_INLINE void op_BIT_MHL(int bit) {
        do_BIT(memoryRead(regs.HL), BIT);
        putAddressOnBus(regs.HL);
    }

    ZEMUX_FORCE_INLINE void op_JR() {
        uint16_t address = regs.PC;
        int8_t offset = static_cast<int8_t>(fetchByte());
        Unroll<5>::loop([&] { putAddressOnBus(address); });

        regs.PC += offset;
        regs.MP = regs.PC;
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_JR_CC(F block) {
        uint16_t address = regs.PC;
        int8_t offset = static_cast<int8_t>(fetchByte());

        if (block) {
            Unroll<5>::loop([&] { putAddressOnBus(address); });

            regs.PC += offset;
            regs.MP = regs.PC;
        }
    }

    ZEMUX_FORCE_INLINE void op_IN_R_BC_P00(uint8_t* r) {
        uint8_t value = ioRead(regs.BC);
        *r = value;
        regs.MP = regs.BC + 1;

        regs.F = (regs.F & FLAG_C)
            | (value & (FLAG_S | FLAG_5 | FLAG_3))
            | (value ? 0 : FLAG_Z)
            | parityLookup[value];

        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_OUT_BC_R_P00(uint8_t r) {
        ioWrite(regs.BC, r);
        regs.MP = regs.BC + 1;
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_IN_F_BC_P00(FN) {
        uint8_t value = ioRead(regs.BC);
        regs.MP = regs.BC + 1;

        regs.F = (regs.F & FLAG_C)
            | (value & (FLAG_S | FLAG_5 | FLAG_3))
            | (value ? 0 : FLAG_Z)
            | parityLookup[value];

        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_OUT_BC_0_P00() {
        ioWrite(regs.BC, 0);
        regs.MP = regs.BC + 1;
        do_PREF_00();
    }

    // Instructions (13 cycles)

    ZEMUX_FORCE_INLINE void op_LD_A_MNN() {
        uint16_t address = fetchWord();
        regs.A = memoryRead(address);
        regs.MP = address + 1;
    }

    ZEMUX_FORCE_INLINE void op_MNN_A() {
        uint16_t address = fetchWord();
        regs.MPH = regs.A;
        regs.MPL = static_cast<uint8_t>(address + 1);
        memoryWrite(address, regs.A);
    }

    ZEMUX_FORCE_INLINE void op_DJNZ() {
        putAddressOnBus(regs.IR);
        uint16_t address = regs.PC;
        int8_t offset = static_cast<int8_t>(fetchByte());

        if (--regs.B) {
            Unroll<5>::loop([&] { putAddressOnBus(address); });
            regs.PC += offset;
            regs.MP = regs.PC;
        }
    }

    // Instructions (14 cycles)

    ZEMUX_FORCE_INLINE void op_RETI_P00() {
        regs.IFF1 = regs.IFF2;
        do_RET();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_RETN_P00() {
        op_RETI_P00();
    }

    // Instructions (15 cycles)

    ZEMUX_FORCE_INLINE void op_ADC_HL_RP_P00(uint16_t rp) {
        do_ADC_16(&regs.HL, rp);
        Unroll<7>::loop([&] { putAddressOnBus(regs.IR); });
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_SBC_HL_RP_P00(uint16_t rp) {
        do_SBC_16(&regs.HL, rp);
        Unroll<7>::loop([&] { putAddressOnBus(regs.IR); });
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_SET_MHL(int bit) {
        uint8_t value = memoryRead(regs.HL);
        putAddressOnBus(regs.HL);
        memoryWrite(regs.HL, value | (1 << bit));
    }

    ZEMUX_FORCE_INLINE void op_RES_MHL(int bit) {
        uint8_t value = memoryRead(regs.HL);
        putAddressOnBus(regs.HL);
        memoryWrite(regs.HL, value & ~(1 << bit));
    }

    // Instructions (16 cycles)

    ZEMUX_FORCE_INLINE op_PREF_XX_CB(Z80CpuOpcode* withOptable) {
        lastOffset = static_cast<int8_t>(fetchByte());

        uint16_t address = regs.PC;
        uint8_t opcode = fetchByte();
        Unroll<2>::loop([&] { putAddressOnBus(address); });

        withOptable[opcode]();
    }

    ZEMUX_FORCE_INLINE void op_LD_RP_MNN(uint8_t* rl, uint8_t* rh) {
        uint16_t address = fetchWord();
        *rl = memoryRead(address);
        *rh = memoryRead(address + 1);
        regs.MP = address + 1;
    }

    ZEMUX_FORCE_INLINE void op_LD_MNN_RP(uint8_t rl, uint8_t rh) {
        uint16_t address = fetchWord();
        regs.MP = address + 1;
        memoryWrite(address, rl);
        memoryWrite(address + 1, rh);
    }

    ZEMUX_FORCE_INLINE void op_LDI_P00() {
        do_REP_LD();
        do_REP_LD_INC();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_LDD_P00() {
        do_REP_LD();
        do_REP_LD_DEC();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_CPI_P00(FN) {
        do_REP_CP();
        do_REP_CP_INC();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_CPD_P00(FN) {
        do_REP_CP();
        do_REP_CP_DEC();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_INI_P00(FN) {
        do_REP_INI();
        ++regs.HL;
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_IND_P00(FN) {
        do_REP_IND();
        --regs.HL;
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_OUTI_P00(FN) {
        do_REP_OUTI();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_OUTD_P00(FN) {
        do_REP_OUTD();
        do_PREF_00();
    }

    // Instructions (17 cycles)

    ZEMUX_FORCE_INLINE void op_CALL() {
        uint16_t address = fetchWord();
        putAddressOnBus(regs.PC - 1);
        do_PUSH(regs.PC);

        regs.PC = address;
        regs.MP = address;
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_CALL_CC(F block) {
        uint16_t address = fetchWord();
        regs.MP = address;

        if (block()) {
            putAddressOnBus(regs.PC - 1);
            do_PUSH(regs.PC);
            regs.PC = address;
        }
    }

    // Instructions (18 cycles)

    ZEMUX_FORCE_INLINE void op_RLD_P00() {
        uint8_t value = memoryRead(regs.HL);
        Unroll<4>::loop([&] { putAddressOnBus(regs.HL); });
        memoryWrite(regs.HL, (value << 4) | (regs.A & 0x0F));
        regs.A = (regs.A & 0xF0) | (value >> 4);

        regs.F = (regs.F & FLAG_C)
            | (regs.A & (FLAG_S | FLAG_5 | FLAG_3))
            | (regs.A ? 0 : FLAG_Z)
            | parityLookup[regs.A];

        regs.MP = regs.HL + 1;
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_RRD_P00() {
        uint8_t value = memoryRead(value);
        Unroll<4>::loop([&] { putAddressOnBus(regs.HL); });
        memoryWrite(regs.HL, (regs.A << 4) | (value >> 4));
        regs.A = (regs.A & 0xF0) | (value & 0x0F);

        regs.F = (regs.F & FLAG_C)
            | (regs.A & (FLAG_S | FLAG_5 | FLAG_3))
            | (regs.A ? 0 : FLAG_Z)
            | parityLookup[regs.A];

        regs.MP = regs.HL + 1;
        do_PREF_00();
    }

    // Instructions (19 cycles)

    ZEMUX_FORCE_INLINE void op_LD_R_ORP_P00(uint8_t* r, uint16_t rp) {
        uint16_t address = regs.PC;
        int8_t offset = fetchOffsetMp(rp);
        Unroll<5>::loop([&] { putAddressOnBus(address); });
        *r = memoryRead(rp + offset);
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_LD_ORP_R_P00(uint16_t rp, uint8_t r) {
        uint16_t address = regs.PC;
        int8_t offset = fetchOffsetMp(rp);
        Unroll<5>::loop([&] { putAddressOnBus(address); });
        memoryWrite(rp + offset, r);
        do_PREF_00();
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_DO_A_ORP_P00(F block, uint16_t rp) {
        uint16_t address = regs.PC;
        int8_t offset = fetchOffsetMp(rp);
        Unroll<5>::loop([&] { putAddressOnBus(address); });
        block(&regs.A, memoryRead(rp + offset));
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_LD_ORP_N_P00(uint16_t rp) {
        int8_t offset = fetchOffsetMp(rp);

        uint16_t address = regs.PC;
        uint8_t value = fetchByte();
        Unroll<2>::loop([&] { putAddressOnBus(address); });

        memoryWrite(rp + offset, value);
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_EX_MSP_RP(uint8_t rl, uint8_t rh, uint16_t* rp) {
        uint16_t value = memoryRead(regs.SP) | (static_cast<uint16_t>(memoryRead(regs.SP + 1)) << 8);
        putAddressOnBus(regs.SP + 1);
        regs.MP = value;

        memoryWrite(regs.SP, rl);
        memoryWrite(regs.SP + 1, rh);
        Unroll<2>::loop([&] { putAddressOnBus(regs.SP + 1); });
        *rp = value;
    }

    // Instructions (20 cycles)

    ZEMUX_FORCE_INLINE void op_BIT_PORP_P00(int bit, uint16_t rp) {
        uint16_t address = rp + lastOffset;
        regs.MP = address;

        do_BIT(memoryRead(address), bit);
        putAddressOnBus(address);
        do_PREF_00();
    }

    // Instructions (21 cycle)

    ZEMUX_FORCE_INLINE void op_LDIR_P00() {
        do_REP_LD();
        do_REP_LD_INCR();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_LDDR_P00() {
        do_REP_LD();
        do_REP_LD_DECR();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_CPIR_P00(FN) {
        do_REP_CP();
        do_REP_CP_INCR();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_CPDR_P00(FN) {
        do_REP_CP();
        do_REP_CP_DECR();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_INIR_P00(FN) {
        do_REP_INI();
        do_REP_IN_INCR();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_INDR_P00(FN) {
        do_REP_IND();
        do_REP_IN_DECR();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_OTIR_P00(FN) {
        do_REP_OUTI();
        do_REP_OUT_INCR_DECR();
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_OTDR_P00(FN) {
        do_REP_OUTD();
        do_REP_OUT_INCR_DECR();
        do_PREF_00();
    }

    // Instructions (23 cycles)

    template< typename F >
    ZEMUX_FORCE_INLINE void op_DO_ORP(F block, uint16_t rp) {
        uint16_t offsetAddress = regs.PC;
        uint16_t valueAddress = rp + fetchOffsetMp(rp);
        Unroll<5>::loop([&] { putAddressOnBus(offsetAddress); });

        uint8_t value = block(memoryRead(valueAddress));
        putAddressOnBus(valueAddress);

        memoryWrite(address, value);
    }

    ZEMUX_FORCE_INLINE void op_SET_PORP_P00(int bit, uint16_t rp) {
        uint16_t address = rp + lastOffset;
        regs.MP = address;

        uint8_t value = memoryRead(address);
        putAddressOnBus(address);

        memoryWrite(address, value | (1 << bit));
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_RES_PORP_P00(int bit, uint16_t rp) {
        uint16_t address = rp + lastOffset;
        regs.MP = address;

        uint8_t value = memoryRead(address);
        putAddressOnBus(address);

        memoryWrite(address, value & ~(1 << bit));
        do_PREF_00();
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_DO_PORP_P00(F block, uint16_t rp) {
        uint16_t address = rp + lastOffset;
        regs.MP = address;

        uint8_t value = block(memoryRead(address));
        putAddressOnBus(address);

        memoryWrite(address, value);
        do_PREF_00();
    }

    template< typename F >
    ZEMUX_FORCE_INLINE void op_LD_DO_R_PORP_P00(F block, uint8_t* r, uint16_t rp) {
        uint16_t address = rp + lastOffset;
        regs.MP = address;

        uint8_t value = block(memoryRead(address));
        putAddressOnBus(address);

        *r = value;
        memoryWrite(address, value);
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_LD_RES_PORP_P00(int bit, uint8_t* r, uint16_t rp) {
        uint16_t address = rp + address;
        regs.MP = address;

        uint8_t value = memoryRead(address) & ~(1 << bit);
        putAddressOnBus(address);

        *r = value;
        memoryWrite(address, value);
        do_PREF_00();
    }

    ZEMUX_FORCE_INLINE void op_LD_SET_PORP_P00(int bit, uint8_t* r, uint16_t rp) {
        uint16_t address = rp + address;
        regs.MP = address;

        uint8_t value = memoryRead(address) | (1 << bit);
        putAddressOnBus(address);

        *r = value;
        memoryWrite(address, value);
        do_PREF_00();
    }

    // Conditions

    ZEMUX_FORCE_INLINE bool cc_NZ() {
        return !(regs.F & FLAG_Z);
    }

    ZEMUX_FORCE_INLINE bool cc_Z() {
        return regs.F & FLAG_Z;
    }

    ZEMUX_FORCE_INLINE bool cc_NC() {
        return !(regs.F & FLAG_C);
    }

    ZEMUX_FORCE_INLINE bool cc_C() {
        return regs.F & FLAG_C;
    }

    ZEMUX_FORCE_INLINE bool cc_PO() {
        return !(regs.F & FLAG_PV); // parity odd
    }

    ZEMUX_FORCE_INLINE bool cc_PE() {
        return regs.F & FLAG_PV; // parity even
    }

    ZEMUX_FORCE_INLINE bool cc_P() {
        return !(regs.F & FLAG_S); // positive
    }

    ZEMUX_FORCE_INLINE bool cc_M() {
        return regs.F & FLAG_S; // minus
    }

    // Actions (not affecting T-state)

    ZEMUX_FORCE_INLINE void do_PREF_00() {
        prefix = 0x00;
        // optable = optable_00;
    }

    ZEMUX_FORCE_INLINE void do_SUB_8(uint8_t* x, uint8_t y) {
        uint16_t result = (*x) - y;
        uint8_t halfResult = ((*x) & 0x0F) - (y & 0x0F);
        int16_t signedResult = static_cast<int8_t>(*x) - static_cast<int8_t>(y);

        regs.F = ((result & FLAG_C_M8) >> FLAG_C_S8)
            | FLAG_N
            | (halfResult & FLAG_H)
            | ((signedResult < -128 || signedResult > 127) ? FLAG_PV : 0)
            | (result & (FLAG_S | FLAG_5 | FLAG_3))
            | (static_cast<uint8_t>(result) ? 0 : FLAG_Z);

        *x = result;
    }

    ZEMUX_FORCE_INLINE void do_INC_8(uint8_t* x) {
        regs.F = (regs.F & FLAG_C) | ((((*x) & 0x0F) + 1) & FLAG_H);
        ++(*x);
        regs.F |= (*x == FLAG_PV_C8 ? FLAG_PV : 0) | ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z);
    }

    ZEMUX_FORCE_INLINE void do_DEC_8(uint8_t* x) {
        regs.F = (regs.F & FLAG_C) | FLAG_N | ((((*x) & 0x0F) - 1) & FLAG_H) | (*x == FLAG_PV_C8 ? FLAG_PV : 0);
        --(*x);
        regs.F |= ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z);
    }

    ZEMUX_FORCE_INLINE void do_ADD_8(uint8_t* x, uint8_t y) {
        uint16_t result = (*x) + y;
        uint8_t halfResult = ((*x) & 0x0F) + (y & 0x0F);
        int16_t signedResult = static_cast<int8_t>(*x) + static_cast<int8_t>(y);

        regs.F = ((result & FLAG_C_M8) >> FLAG_C_S8)
            | (halfResult & FLAG_H)
            | ((signedResult < -128 || signedResult > 127) ? FLAG_PV : 0)
            | (result & (FLAG_S | FLAG_5 | FLAG_3))
            | (static_cast<uint8_t>(result) ? 0 : FLAG_Z);

        *x = result;
    }

    ZEMUX_FORCE_INLINE void do_ADC_8(uint8_t* x, uint8_t y) {
        uint16_t result = (*x) + y + (regs.F & FLAG_C);
        uint8_t halfResult = ((*x) & 0x0F) + (y & 0x0F) + (regs.F & FLAG_C);
        int16_t signedResult = static_cast<int8_t>(*x) + static_cast<int8_t>(y) + static_cast<int16_t>(regs.F & FLAG_C);

        regs.F = ((result & FLAG_C_M8) >> FLAG_C_S8)
            | (halfResult & FLAG_H)
            | ((signedResult < -128 || signedResult > 127) ? FLAG_PV : 0)
            | (result & (FLAG_S | FLAG_5 | FLAG_3))
            | (static_cast<uint8_t>(result) ? 0 : FLAG_Z);

        *x = result;
    }

    ZEMUX_FORCE_INLINE void do_SBC_8(uint8_t* x, uint8_t y) {
        uint16_t result = (*x) - y - (regs.F & FLAG_C);
        uint8_t halfResult = ((*x) & 0x0F) - (y & 0x0F) - (regs.F & FLAG_C);
        int16_t signedResult = static_cast<int8_t>(*x) - static_cast<int8_t>(y) - static_cast<int16_t>(regs.F & FLAG_C);

        regs.F = ((result & FLAG_C_M8) >> FLAG_C_S8)
            | FLAG_N
            | (halfResult & FLAG_H)
            | ((signedResult < -128 || signedResult > 127) ? FLAG_PV : 0)
            | (result & (FLAG_S | FLAG_5 | FLAG_3))
            | (static_cast<uint8_t>(result) ? 0 : FLAG_Z);

        *x = result;
    }

    ZEMUX_FORCE_INLINE void do_AND_8(uint8_t* x, uint8_t y) {
        *x &= y;
        regs.F = FLAG_H | ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_XOR_8(uint8_t* x, uint8_t y) {
        (*x) ^= y;
        regs.F = ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_OR_8(uint8_t* x, uint8_t y) {
        (*x) |= y;
        regs.F = ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_CP_8(uint8_t* x, uint8_t y) {
        uint16_t result = (*x) - y;
        uint8_t halfResult = ((*x) & 0x0F) - (y & 0x0F);
        int16_t signedResult = static_cast<int8_t>(*x) - static_cast<int8_t>y;

        regs.F = ((result & FLAG_C_M8) >> FLAG_C_S8)
            | FLAG_N
            | (halfResult & FLAG_H)
            | ((signedResult < -128 || signedResult > 127) ? FLAG_PV : 0)
            | (y & (FLAG_5 | FLAG_3))
            | (result & FLAG_S)
            | (static_cast<uint8_t>(result) ? 0 : FLAG_Z);
    }

    ZEMUX_FORCE_INLINE void do_RLC_8(uint8_t* x) {
        *x = ((*x) << 1) | ((*x) >> 7);
        regs.F = ((*x) & (FLAG_S | FLAG_5 | FLAG_3 | FLAG_C)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_RRC_8(uint8_t* x) {
        regs.F = (*x) & FLAG_C;
        *x = ((*x) >> 1) | ((*x) << 7);
        regs.F |= ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_RL_8(uint8_t* x) {
        uint8_t value = *x;
        *x = ((*x) << 1) | (regs.F & FLAG_C);
        regs.F = (value >> 7) | ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_RR_8(uint8_t* x) {
        uint8_t value = *x;
        *x = ((*x) >> 1) | (regs.F << 7);
        regs.F = (value & FLAG_C) | ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_SLA_8(uint8_t* x) {
        regs.F = (*x) >> 7;
        *x <<= 1;
        regs.F |= ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_SRA_8(uint8_t* x) {
        regs.F = (*x) & FLAG_C;
        *x = ((*x) & 0x80) | ((*x) >> 1);
        regs.F |= ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_SLL_8(uint8_t* x) {
        regs.F = (*x) >> 7;
        *x = ((*x) << 1) | 0x01;
        regs.F |= ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_SRL_8(uint8_t* x) {
        regs.F = (*x) & FLAG_C;
        *x >>= 1;
        regs.F |= ((*x) & (FLAG_S | FLAG_5 | FLAG_3)) | ((*x) ? 0 : FLAG_Z) | parityLookup[*x];
    }

    ZEMUX_FORCE_INLINE void do_INC_16(uint16_t* rp) {
        ++(*rp);
    }

    ZEMUX_FORCE_INLINE void do_DEC_16(uint16_t* rp) {
        --(*rp);
    }

    ZEMUX_FORCE_INLINE void do_ADD_16(uint16_t* x, uint16_t y) {
        uint32_t result = (*x) + y;
        uint16_t halfResult = ((*x) & 0x0FFF) + (y & 0x0FFF);

        regs.MP = (*x) + 1;
        *x = result;

        regs.F = (regs.F & (FLAG_PV | FLAG_Z | FLAG_S))
            | ((result & FLAG_C_M16) >> FLAG_C_S16)
            | ((result >> 8) & (FLAG_5 | FLAG_3))
            | ((halfResult & FLAG_H_M16) >> FLAG_H_S16);
    }

    ZEMUX_FORCE_INLINE void do_ADC_16(uint16_t* x, uint16_t y) {
        uint32_t result = (*x) + y + (regs.F & FLAG_C);
        uint16_t halfResult = ((*x) & 0x0FFF) + (y & 0x0FFF) + (regs.F & FLAG_C);
        int32_t signedResult static_cast<int16_t>(*x) + static_cast<int16_t>(y) + static_cast<int32_t>(regs.F & FLAG_C);

        regs.MP = (*x) + 1;
        *x = result;

        regs.F = ((result & FLAG_C_M16) >> FLAG_C_S16)
            | ((halfResult & FLAG_H_M16) >> FLAG_H_S16)
            | ((signedResult < -32768 || signedResult > 32767) ? FLAG_PV : 0)
            | ((result >> 8) & (FLAG_S | FLAG_5 | FLAG_3))
            | (static_cast<uint8_t>(result) ? 0 : FLAG_Z);
    }

    ZEMUX_FORCE_INLINE void do_SBC_16(uint16_t* x, uint16_t y) {
        uint32_t result = (*x) - y - (regs.F & FLAG_C);
        uint16_t halfResult = ((*x) & 0x0FFF) - (y & 0x0FFF) - (regs.F & FLAG_C);
        int32_t signedResult = static_cast<int16_t>(*x) - static_cast<int16_t>(y) - static_cast<int32_t>(regs.F & FLAG_C);

        regs.F = ((result & FLAG_C_M16) >> FLAG_C_S16)
            | FLAG_N
            | ((halfResult & FLAG_H_M16) >> FLAG_H_S16)
            | ((signedResult < -32768 || signedResult > 32767) ? FLAG_PV : 0)
            | ((result >> 8) & (FLAG_S | FLAG_5 | FLAG_3))
            | (static_cast<uint8_t>(result) ? 0 : FLAG_Z);

        regs.MP = (*x) + 1;
        *x = result;
    }

    ZEMUX_FORCE_INLINE void do_BIT(uint8_t value, int bit) {
        uint8_t result = value & (1 << bit);

        regs.F = (regs.F & FLAG_C)
            | FLAG_H
            | (result & FLAG_S)
            | (result ? 0 : FLAG_Z)
            | parityLookup[result]
            | (regs.MPH & (FLAG_5 | FLAG_3));
    }

    ZEMUX_FORCE_INLINE void do_REP_LD_INC() {
        ++regs.HL;
        ++regs.DE;
    }

    ZEMUX_FORCE_INLINE void do_REP_LD_DEC() {
        --regs.HL;
        --regs.DE;
    }

    ZEMUX_FORCE_INLINE void do_REP_CP_INC() {
        ++regs.HL;
        ++regs.MP;
    }

    ZEMUX_FORCE_INLINE void do_REP_CP_DEC() {
        --regs.HL;
        --regs.MP;
    }

    // Actions (affecting T-state)

    ZEMUX_FORCE_INLINE uint16_t do_POP() {
        uint8_t value = memoryRead(regs.SP++);
        return value | static_cast<uint16_t>(memoryRead(regs.SP++))
    }

    ZEMUX_FORCE_INLINE void do_RET() {
        regs.PC = do_POP();
        regs.MP = regs.PC;
    }

    ZEMUX_FORCE_INLINE void do_PUSH(uint16_t rp) {
        memoryWrite(--regs.SP, static_cast<uint8_t>(rp >> 8));
        memoryWrite(--regs.SP, static_cast<uint8_t>(rp));
    }

    ZEMUX_FORCE_INLINE uint8_t do_IN(uint16_t port) {
        uint8_t result = ioRead(port);
        regs.MP = port + 1;
        return result;
    }

    ZEMUX_FORCE_INLINE void do_OUT(uint16_t port, uint8_t value) {
        ioWrite(port, value);
        regs.MPL = static_cast<uint8_t>(port + 1);
        regs.MPH = regs.A;
    }

    ZEMUX_FORCE_INLINE void do_REP_LD() {
        uint8_t value = memoryRead(regs.HL);
        memoryWrite(regs.DE, value);
        Unroll<2>::loop([&]() { putAddressOnBus(regs.DE); });

        --regs.BC;
        value += regs.A;

        regs.F = (regs.F & (FLAG_S | FLAG_Z | FLAG_C))
            | (regs.BC ? FLAG_PV : 0)
            | (value & FLAG_3)
            | ((value << FLAG_N_TO_5) & FLAG_5);
    }

    ZEMUX_FORCE_INLINE void do_REP_LD_INCR() {
        if (regs.BC) {
            Unroll<5>::loop([&]() { putAddressOnBus(regs.DE); });
            regs.PC -= 2;
            regs.MP = regs.PC + 1;
        }

        ++regs.HL;
        ++regs.DE;
    }

    ZEMUX_FORCE_INLINE void do_REP_LD_DECR() {
        if (regs.BC) {
            Unroll<5>::loop([&]() { putAddressOnBus(regs.DE); });
            regs.PC -= 2;
            regs.MP = regs.PC + 1;
        }

        --regs.HL;
        --regs.DE;
    }

    ZEMUX_FORCE_INLINE void do_REP_CP() {
        uint8_t value = memoryRead(regs.HL);
        Unroll<5>::loop([&]() { putAddressOnBus(regs.HL); });

        uint8_t halfResult = (regs.A & 0x0F) - (value & 0x0F);
        value = regs.A - value;
        --regs.BC;

        regs.F = (regs.F & FLAG_C)
            | FLAG_N
            | (regs.BC ? FLAG_PV : 0)
            | (halfResult & FLAG_H)
            | (value ? 0 : FLAG_Z)
            | (value & FLAG_S);

        value -= ((regs.F & FLAG_H) >> FLAG_H_TO_C);
        regs.F |= (value & FLAG_3) | ((value << FLAG_N_TO_5) & FLAG_5);
    }

    ZEMUX_FORCE_INLINE void do_REP_CP_INCR() {
        if ((regs.F & (FLAG_Z | FLAG_PV)) == FLAG_PV) {
            Unroll<5>::loop([&]() { putAddressOnBus(regs.HL); });
            regs.PC -= 2;
            regs.MP = regs.PC + 1;
        } else {
            ++regs.MP;
        }

        ++regs.HL;
    }

    ZEMUX_FORCE_INLINE void do_REP_CP_DECR() {
        if ((regs.F & (FLAG_Z | FLAG_PV)) == FLAG_PV) {
            Unroll<5>::loop([&]() { putAddressOnBus(regs.HL); });
            regs.PC -= 2;
            regs.MP = regs.PC + 1;
        } else {
            --regs.MP;
        }

        --regs.HL;
    }

    ZEMUX_FORCE_INLINE void do_REP_INI() {
        putAddressOnBus(regs.IR);
        uint8_t byteValue = ioRead(regs.BC);
        memoryWrite(regs.HL, byteValue);

        regs.MP = regs.BC + 1;
        --regs.B;
        uint16_t wordValue = byteValue + static_cast<uint8_t>(regs.C + 1);

        regs.F = ((byteValue >> FLAG_S_TO_N) & FLAG_N)
            | (regs.B & (FLAG_S | FLAG_5 | FLAG_3))
            | (regs.B ? 0 : FLAG_Z)
            | parityLookup[static_cast<uint8_t>(wordValue & 0x07) ^ regs.B]
            | ((wordValue > 255) ? (FLAG_C | FLAG_H) : 0);
    }

    ZEMUX_FORCE_INLINE void do_REP_IND() {
        putAddressOnBus(regs.IR);
        uint8_t byteValue = ioRead(regs.BC);
        memoryWrite(regs.HL, byteValue);

        regs.MP = regs.BC - 1;
        --regs.B;
        uint16_t wordValue = byteValue + static_cast<uint8_t>(regs.C - 1);

        regs.F = ((byteValue >> FLAG_S_TO_N) & FLAG_N)
            | (regs.B & (FLAG_S | FLAG_5 | FLAG_3))
            | (regs.B ? 0 : FLAG_Z)
            | parityLookup[static_cast<uint8_t>(wordValue & 0x07) ^ regs.B]
            | ((wordValue > 255) ? (FLAG_C | FLAG_H) : 0);
    }

    ZEMUX_FORCE_INLINE void do_REP_IN_INCR() {
        if (regs.B) {
            Unroll<5>::loop([&]() { putAddressOnBus(regs.HL); });
            regs.PC -= 2;
        }

        ++regs.HL;
    }

    ZEMUX_FORCE_INLINE void do_REP_IN_DECR() {
        if (regs.B) {
            Unroll<5>::loop([&]() { putAddressOnBus(regs.HL); });
            regs.PC -= 2;
        }

        --regs.HL;
    }

    ZEMUX_FORCE_INLINE void do_REP_OUTI() {
        putAddressOnBus(regs.IR);
        uint8_t byteValue = memoryRead(regs.HL);

        --regs.B;
        regs.MP = regs.BC + 1;
        ioWrite(regs.BC, byteValue);

        ++regs.HL;
        uint16_t wordValue = byteValue + regs.L;

        regs.F = ((byteValue >> FLAG_S_TO_N) & FLAG_N)
            | (regs.B & (FLAG_S | FLAG_5 | FLAG_3))
            | (regs.B ? 0 : FLAG_Z)
            | parityLookup[static_cast<uint8_t>(wordValue & 0x07) ^ regs.B]
            | ((wordValue > 255) ? (FLAG_C | FLAG_H) : 0);
    }

    ZEMUX_FORCE_INLINE void do_REP_OUTD() {
        putAddressOnBus(regs.IR);
        uint8_t byteValue = memoryRead(regs.HL);

        --regs.B;
        regs.MP = regs.BC - 1;
        ioWrite(regs.BC, byteValue);

        --regs.HL;
        uint16_t wordValue = byteValue + regs.L;

        regs.F = ((tmp_byte >> FLAG_S_TO_N) & FLAG_N)
            | (regs.B & (FLAG_S | FLAG_5 | FLAG_3))
            | (regs.B ? 0 : FLAG_Z)
            | parityLookup[static_cast<uint8_t>(wordValue & 0x07) ^ regs.B]
            | ((wordValue > 255) ? (FLAG_C | FLAG_H) : 0);
    }

    ZEMUX_FORCE_INLINE void do_REP_OUT_INCR_DECR() {
        if (regs.B) {
            Unroll<5>::loop([&]() { putAddressOnBus(regs.BC); });
            regs.PC -= 2;
        }
    }
};

#endif
