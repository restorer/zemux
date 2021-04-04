#include "border_device.h"

namespace zemux {

static void onBorderDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value) {
    static_cast<BorderDevice*>(data)->onIorqWr(value);
}

BorderDevice::BorderDevice(Loudspeaker* loudspeaker) : loudspeaker { loudspeaker } {
}

BusIorqWrElement BorderDevice::onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) {
    return (port & 1) ? prev : BusIorqWrElement { .callback = onBorderDeviceIorqWr, .data = this };
}

void BorderDevice::onIorqWr(uint8_t value) {
    uint32_t ticks = attachedBus->getFrameTicksPassed();
    uint16_t volume = 0;

    if (value & BIT_TAPE) {
        volume += VOLUME_TAPE;
    }

    if (value & BIT_SPEAKER) {
        volume += VOLUME_SPEAKER;
    }

    loudspeaker->onLoudspeakerStepTo(volume, volume, ticks);

    if ((value & MASK_COLOR) != (portFB & MASK_COLOR)) {
        // videoDevice->renderStepTo(ticks);
    }

    portFB = value;
}

}
