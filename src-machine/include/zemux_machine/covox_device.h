#ifndef ZEMUX_MACHINE__COVOX_DEVICE
#define ZEMUX_MACHINE__COVOX_DEVICE

#include "device.h"
#include <zemux_core/non_copyable.h>
#include <zemux_core/loudspeaker.h>

namespace zemux {

static void onCovoxDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);

class CovoxDevice final : public Device, private NonCopyable {
public:

    explicit CovoxDevice(Loudspeaker* loudspeaker);
    virtual ~CovoxDevice() = default;

    BusIorqWrElement onConfigureIorqWr(BusIorqWrElement prev, int /* iorqWrLayer */, uint16_t port) override;
    void onReset() override;

private:

    Loudspeaker* loudspeaker;

    void onIorqWr(uint8_t value);

    friend void onCovoxDeviceIorqWr(void* data, uint16_t /* port */, uint8_t value);
};

}

#endif
