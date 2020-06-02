#include <mutex>
#include <zemux_z80/z80cpu.h>

namespace zemux {

static bool isSharedDataInitialized = false;
static std::mutex initSharedDataMutex;

uint8_t Z80Cpu::parityLookup[0x100];

void Z80Cpu::initSharedData() {
    std::lock_guard _ { initSharedDataMutex };

    if (isSharedDataInitialized) {
        return;
    }

    for (int i = 0; i < 0x100; ++i) {
        int p = ((i & 0x80) >> 7)
            ^ ((i & 0x40) >> 6)
            ^ ((i & 0x20) >> 5)
            ^ ((i & 0x10) >> 4)
            ^ ((i & 0x08) >> 3)
            ^ ((i & 0x04) >> 2)
            ^ ((i & 0x02) >> 1)
            ^ (i & 0x01);

        Z80Cpu::parityLookup[i] = (p ? 0 : Z80Cpu::FLAG_PV);
    }

    isSharedDataInitialized = true;
}

Z80Cpu::Z80Cpu(Z80CpuCallback* cb) : cb { cb } {
    initSharedData();
    reset();
}

void Z80Cpu::reset() {
    regs.AF = 0xFFFF;
    regs.SP = 0xFFFF;
    regs.PC = 0x0000;
    regs.MP = 0x0000;

    regs.I = 0x00;
    regs.R = 0x00;
    regs.IFF1 = false;
    regs.IFF2 = false;
    regs.IM = 0;

    // all others registers may has any value

    isHalted = false;
    shouldResetPv = false;
    isProcessingInstruction = false;
    pcIncrement = 1;

    // optable = optable_00;
    prefix = 0;
}

void Z80Cpu::doInt() {
    if (!regs.IFF1 || isProcessingInstruction || prefix) {
        return;
    }

    if (isHalted) {
        ++regs.PC;
        isHalted = false;
    }

    regs.IFF1 = false;
    regs.IFF2 = false;

    if (shouldResetPv) {
        regs.F &= ~FLAG_PV;
        shouldResetPv = false;
    }

    isProcessingInstruction = true;
    uint8_t intVec = fetchIntVec();

    switch (regs.IM) {
        case 0: {
            // From "The Z80 Family Program Interrupt Structure":
            //
            // The first byte of a multi-byte instruction is read during the interrupt acknowledge cycle.
            // Subsequent bytes are read in by a normal memory read sequence (the PC, however, remains
            // at its pre-interrupt state and the user must insure that memory will not respond
            // to these read sequences).

            pcIncrement = 0;
            (this->*optable[intVec])();
            pcIncrement = 1;

            break;
        }

        case 1: {
            // From "The Z80 Family Program Interrupt Structure":
            //
            // Note that when doing programmed I/O the CPU will ignore any data put onto
            // the data bus during the interrupt acknowledge cycle.

            // (this->*optable_00[0xFF])(); // RST #38
            break;
        }

        // IM 2
        default: {
            uint16_t address = (static_cast<uint16_t>(regs.I) << 8) | intVec;

            memoryWrite(--regs.SP, regs.PCH);
            memoryWrite(--regs.SP, regs.PCL);

            regs.PCL = memoryRead(address);
            regs.PCH = memoryRead(address + 1);

            regs.MP = regs.PC;
            ++tstate;

            break;
        }
    }

    isProcessingInstruction = false;
}

void Z80Cpu::doNmi() {
    if (isProcessingInstruction || prefix) {
        return;
    }

    if (isHalted) {
        ++regs.PC;
        isHalted = false;
    }

    isProcessingInstruction = true;
    ++tstate; // Additional T-state

    // From "Z80 CPU User Manual":
    //
    // The CPU response to a nonmaskable interrupt is similar to a normal memory read operation.
    // The only difference is that the contents of the data bus are ignored while the processor
    // automatically stores the Program Counter in the external stack and jumps to address 0066h.
    [[maybe_unused]] auto unused = fetchOpcode();

    // NOTICE: if you thinking about submitting bug-report about IFF1 not copied to IFF2, you are not right :)
    // actually IFF1 *not* copied to IFF2. this is tested on real hardware.
    regs.IFF1 = false;

    memoryWrite(--regs.SP, regs.PCH);
    memoryWrite(--regs.SP, regs.PCL);

    regs.PC = 0x0066;
    regs.MP = regs.PC;

    isProcessingInstruction = false;
}

unsigned int Z80Cpu::getTicks() {
    if (clockRatio < -1) {
        return ticks + tstate * (-clockRatio);
    }

    if (clockRatio > 1) {
        return ticks + (tstate + clockRatio - 1) / clockRatio;
    }

    return ticks + tstate;
}

void Z80Cpu::consumeTicks(unsigned int consumedTicks) {
    if (clockRatio < -1) {
        ticks += tstate * (-clockRatio);
        tstate = 0;
    } else if (clockRatio > 1) {
        unsigned int consumed = (tstate + clockRatio - 1) / clockRatio;
        ticks += consumed;
        tstate -= consumed * clockRatio;
    } else {
        ticks += tstate;
        tstate = 0;
    }

    ticks -= consumedTicks;
}

void Z80Cpu::setClockRatio(int newClockRatio) {
    if (clockRatio < -1) {
        ticks += tstate * (-clockRatio);
    } else if (clockRatio > 1) {
        ticks += (tstate + clockRatio - 1) / clockRatio;
    } else {
        ticks += tstate;
    }

    tstate = 0;
    clockRatio = newClockRatio;
}

void Z80Cpu::step() {
    shouldResetPv = false;

    isProcessingInstruction = true;
    (this->*optable[fetchOpcode()])();
    isProcessingInstruction = false;
}

}
