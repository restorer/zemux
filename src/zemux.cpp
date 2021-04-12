#include <iostream>
#include <zemux_core/non_copyable.h>
#include <zemux_core/sound.h>
#include <zemux_integrated/z80_chip.h>
#include <zemux_integrated/ay_chip.h>

namespace zemux {

class ZemuXStub : public Z80ChipCallback, public SoundSink, private NonCopyable {
public:

    uint8_t onZ80MreqRd(uint16_t /* address */, bool /* isM1 */) override {
        return 0;
    }

    void onZ80MreqWr(uint16_t /* address */, uint8_t /* value */) override {
    }

    uint8_t onZ80IorqRd(uint16_t /* port */) override {
        return 0;
    }

    void onZ80IorqWr(uint16_t /* port */, uint8_t /* value */) override {
    }

    void sinkForwardTo(uint16_t /* left */, uint16_t /* right */, uint32_t /* ticks */) override {
    }

    void sinkAdvanceBy(uint16_t /* left */, uint16_t /* right */, uint32_t /* ticksDelta */) override {
    }
};

}

int main() {
    zemux::ZemuXStub zemuXStub;

    zemux::Z80Chip cpu(&zemuXStub);
    zemux::AyChip ayChip(&zemuXStub);

    std::cout << "Test\n";
    return 0;
}
