#ifndef ZEMUX_MACHINE__BORDER_DEVICE
#define ZEMUX_MACHINE__BORDER_DEVICE

#include "device.h"
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <zemux_core/loudspeaker.h>
#include <cstdint>

namespace zemux {

static void onBorderDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);

class BorderDevice final : public Device, private NonCopyable {
public:

    static constexpr uint8_t MASK_COLOR = 7;

    static constexpr uint8_t BIT_TAPE = 8;
    static constexpr uint8_t BIT_SPEAKER = 0x10;

    static constexpr uint16_t VOLUME_TAPE = 0x4000;
    static constexpr uint16_t VOLUME_SPEAKER = 0xBFFF;

    explicit BorderDevice(Loudspeaker* loudspeaker);
    virtual ~BorderDevice() = default;

    BusIorqWrElement onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) override;

    ZEMUX_FORCE_INLINE uint8_t getPortFB() {
        return portFB;
    }

private:

    Loudspeaker* loudspeaker;
    uint8_t portFB = 0;

    void onIorqWr(uint8_t value);

    friend void onBorderDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);
};

}

#endif
