#ifndef ZEMUX_TEST__STUB_SOUND_SINK
#define ZEMUX_TEST__STUB_SOUND_SINK

#include <zemux_core/sound_sink.h>

class StubSoundSink : public zemux::SoundSink {
public:

    StubSoundSink() {
    }

    inline uint32_t getLeftSum() {
        return leftSum;
    }

    inline uint32_t getRightSum() {
        return rightSum;
    }

    inline unsigned int getNumSteps() {
        return numSteps;
    }

    inline uint32_t getNumTicks() {
        return numTicks;
    }

    void soundForwardTo(uint16_t left, uint16_t right, uint32_t ticks) override {
        leftSum += left;
        rightSum += right;
        ++numSteps;
        numTicks = ticks;
    }

    void soundAdvanceBy(uint16_t left, uint16_t right, uint32_t ticksDelta) override {
        leftSum += left;
        rightSum += right;
        ++numSteps;
        numTicks += ticksDelta;
    }

    void reset() {
        leftSum = 0;
        rightSum = 0;
        numSteps = 0;
        numTicks = 0;
    }

private:

    uint32_t leftSum = 0;
    uint32_t rightSum = 0;
    unsigned int numSteps = 0;
    uint32_t numTicks = 0;
};

#endif
