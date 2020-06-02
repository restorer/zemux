#include <iostream>
#include <zemux_core/non_copyable.h>
#include <zemux_z80/z80cpu.h>

namespace zemux {

class ZemuX : public Z80CpuCallback, private NonCopyable {
public:

    uint8_t onZ80Read(uint16_t /* address */, bool /* isM1 */) {
        return 0;
    }

    void onZ80Write(uint16_t /* address */, uint8_t /* value */) {
    }

    uint8_t onZ80In(uint16_t /* port */) {
        return 0;
    }

    void onZ80Out(uint16_t /* port */, uint8_t /* value */) {
    }

    uint8_t onZ80ReadInt() {
        return 0;
    }
};

}

int main() {
    zemux::ZemuX zemuX;
    zemux::Z80Cpu cpu(&zemuX);

    std::cout << "Test\n";

    return 0;
}