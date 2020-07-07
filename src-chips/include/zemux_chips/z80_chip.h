#ifndef ZEMUX_CHIPS__Z80_CHIP
#define ZEMUX_CHIPS__Z80_CHIP

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

#include <algorithm>
#include <cstdint>
#include <zemux_core/config.h>
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>

namespace zemux {

class Z80ChipCallback {
public:

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
    virtual void onZ80PutAddress(uint16_t /* address */, unsigned int /* cycles */) {
    }

protected:

    constexpr Z80ChipCallback() = default;
    ~Z80ChipCallback() = default;
};

struct Z80ChipRegs {
#pragma pack(push, 1)
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
#pragma pack(pop)

    bool IFF1 = false;
    bool IFF2 = false;
    int IM = 0;
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedStructInspection"
class Z80ChipCore;
#pragma clang diagnostic pop

class Z80Chip final : private NonCopyable {
public:

    using Opcode = void (*)(Z80Chip*);

    enum ChipType {
        TypeNmos,
        TypeCmos
    };

    static constexpr unsigned int FLAG_C = 0x01; // carry
    static constexpr unsigned int FLAG_N = 0x02; // subtract
    static constexpr unsigned int FLAG_PV = 0x04; // parity or overflow
    static constexpr unsigned int FLAG_3 = 0x08; // copy of bit 3
    static constexpr unsigned int FLAG_H = 0x10; // half carry
    static constexpr unsigned int FLAG_5 = 0x20; // copy of bit 5
    static constexpr unsigned int FLAG_Z = 0x40; // zero
    static constexpr unsigned int FLAG_S = 0x80; // sign

    explicit Z80Chip(Z80ChipCallback* cb, ChipType chipType = TypeNmos);

    Z80ChipRegs regs;

    [[nodiscard]] ZEMUX_FORCE_INLINE ChipType getChipType() const {
        return chipType;
    };

    [[nodiscard]] ZEMUX_FORCE_INLINE uint8_t getOpcodePrefix() const {
        return prefix;
    }

    // Should only be used in the debugger.
    // During emulation, you should call doInt() directly, without checking isIntPossible().
    // If you do not do it, it will not allow you to correctly emulate the behavior of the NMOS chip.
    [[nodiscard]] ZEMUX_FORCE_INLINE bool isIntPossible() const {
        return regs.IFF1 && !isProcessingInstruction && !prefix && !shouldSkipNextInterrupt;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE bool isNmiPossible() const {
        return !isProcessingInstruction && !prefix && !shouldSkipNextInterrupt;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE unsigned int getTstate() const {
        return tstate;
    }

    ZEMUX_FORCE_INLINE void wait(unsigned int cycles) {
        tstate += cycles;
    }

    void setChipType(ChipType type);
    void reset();
    unsigned int step();
    unsigned int doInt();
    unsigned int doNmi();

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

    Z80ChipCallback* cb;
    ChipType chipType;
    Opcode* optable;
    bool isHalted;
    bool shouldResetPv;
    bool isProcessingInstruction;
    bool shouldSkipNextInterrupt;
    uint16_t pcIncrement = 1;
    uint8_t prefix;
    int8_t cbOffset = 0;
    unsigned int tstate = 0;

    ZEMUX_FORCE_INLINE void incR() {
        regs.R = (regs.R & 0x80) | ((regs.R + 1) & 0x7Fu);
    }

    ZEMUX_FORCE_INLINE void putAddressOnBus(uint16_t address, unsigned int cycles) {
        cb->onZ80PutAddress(address, cycles);
        tstate += cycles;
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
        auto offset = static_cast<int8_t>(fetchByte());
        regs.MP = rp + offset;
        return offset;
    }

    ZEMUX_FORCE_INLINE uint16_t fetchWord() {
        uint8_t value = fetchByte();
        return value | static_cast<uint16_t>(static_cast<uint16_t>(fetchByte()) << 8);
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

        putAddressOnBus(port, 1);
        uint8_t value = cb->onZ80IorqRd(port);
        tstate += 3;
        return value;
    }

    ZEMUX_FORCE_INLINE void ioWrite(uint16_t port, uint8_t value) {
        // From "Z80 CPU User Manual":
        // During I/O operations, a single wait state is automatically inserted.

        putAddressOnBus(port, 1);
        cb->onZ80IorqWr(port, value);
        tstate += 3;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedStructInspection"
    friend class Z80ChipCore;
#pragma clang diagnostic pop
};

}

#endif
