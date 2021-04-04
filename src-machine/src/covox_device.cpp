#include "covox_device.h"

namespace zemux {

static void onCovoxDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value) {
    static_cast<CovoxDevice*>(data)->onIorqWr(value);
}

CovoxDevice::CovoxDevice(Loudspeaker* loudspeaker) : loudspeaker { loudspeaker } {
}

BusIorqWrElement CovoxDevice::onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) {
    return ((port & 0x07) == 0x03) ? BusIorqWrElement { .callback = onCovoxDeviceIorqWr, .data = this } : prev;
}

void CovoxDevice::onReset() {
    loudspeaker->onLoudspeakerStepBy(0, 0, 0);
}

void CovoxDevice::onIorqWr(uint8_t value) {
    uint16_t volume = static_cast<uint16_t>(value) << 8;
    loudspeaker->onLoudspeakerStepTo(volume, volume, attachedBus->getFrameTicksPassed());
}

}