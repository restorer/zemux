#ifndef ZEMUX_TEST__TEST_LOUDSPEAKER
#define ZEMUX_TEST__TEST_LOUDSPEAKER

#include <zemux_core/loudspeaker.h>

class StubLoudspeaker : public zemux::Loudspeaker {
public:

    StubLoudspeaker() {
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

    void onLoudspeakerStep(uint16_t left, uint16_t right, uint32_t ticks) override {
        leftSum += left;
        rightSum += right;
        ++numSteps;
        numTicks += ticks;
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
