#include <iostream>
#include <zemux_core/non_copyable.h>
#include <zemux_core/sound.h>
#include <zemux_integrated/z80_chip.h>
#include <zemux_integrated/ay_chip.h>
#include <cstdint>

namespace zemux {

static uint8_t onStubMreqRd(void* /* data */, uint16_t /* address */, bool /* isM1 */) {
    return 0x00;
}

static void onStubMreqWr(void* /* data */, uint16_t /* address */, uint8_t /* value */) {
}

static uint8_t onStubIorqRd(void* /* data */, uint16_t /* port */) {
    return 0x00;
}

static void onStubIorqWr(void* /* data */, uint16_t /* port */, uint8_t /* value */) {
}

static uint8_t onStubIorqM1(void* /* data */) {
    return 0xFF;
}

static void onStubPutAddress(void* /* data */, uint16_t /* address */, uint_fast32_t /* cycles */) {
}

class ZemuXStub : public SoundSink, private NonCopyable {
public:

    void sinkForwardTo(uint16_t /* left */, uint16_t /* right */, uint32_t /* ticks */) override {
    }

    void sinkAdvanceBy(uint16_t /* left */, uint16_t /* right */, uint32_t /* ticksDelta */) override {
    }
};

}

int main() {
    zemux::ZemuXStub zemuXStub;

    zemux::Z80Chip cpu { &zemuXStub,
            zemux::onStubMreqRd,
            zemux::onStubMreqWr,
            zemux::onStubIorqRd,
            zemux::onStubIorqWr,
            zemux::onStubIorqM1,
            zemux::onStubPutAddress };

    zemux::AyChip ayChip { &zemuXStub };

    std::cout << "Test\n";
    return 0;
}
