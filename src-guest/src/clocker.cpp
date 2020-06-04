public:

    ZEMUX_FORCE_INLINE unsigned int getTicks() {
        return ticks + convertCyclesToTicks(tstate);
    }

    ZEMUX_FORCE_INLINE int getClockRatio() {
        return clockRatio;
    }

    ZEMUX_FORCE_INLINE void waitTicks(unsigned int ticksToWait) {
        ticks += ticksToWait;
    }

    void consumeTicks(unsigned int consumedTicks);
    void setClockRatio(int newClockRatio);

private:

    unsigned int ticks = 0;
    int clockRatio = 0;

    unsigned int convertCyclesToTicks(unsigned int cycles);

    ZEMUX_FORCE_INLINE void putAddressOnBus(uint16_t address, unsigned int cycles) {
        cb->onZ80PutAddress(address, std::max(1U, convertCyclesToTicks(cycles)));
        tstate += cycles;
    }

////

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

unsigned int Z80Cpu::convertCyclesToTicks(unsigned int cycles) {
    if (clockRatio < -1) {
        return cycles * (-clockRatio);
    }

    if (clockRatio > 1) {
        return (cycles + clockRatio - 1) / clockRatio;
    }

    return cycles;
}
