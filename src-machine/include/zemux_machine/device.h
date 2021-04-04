#ifndef ZEMUX_MACHINE__DEVICE
#define ZEMUX_MACHINE__DEVICE

#include "bus.h"
#include "action_handler.h"

namespace zemux {

class Device : public ActionHandler {
public:

    virtual void onAttach(Bus* bus);

    virtual void onDetach();

    virtual BusMreqRdElement onConfigureMreqRd(BusMreqRdElement prev, int mreqRdLayer, uint16_t address, bool isM1) {
        return prev;
    }

    virtual BusMreqWrElement onConfigureMreqWr(BusMreqWrElement prev, int mreqWrLayer, uint16_t address) {
        return prev;
    }

    virtual BusIorqRdElement onConfigureIorqRd(BusIorqRdElement prev, int iorqRdLayer, uint16_t port) {
        return prev;
    }

    virtual BusIorqWrElement onConfigureIorqWr(BusIorqWrElement prev, int iorqWrLayer, uint16_t port) {
        return prev;
    }

    virtual void onReset() {
    }

protected:

    Bus* attachedBus = nullptr;

    constexpr Device() = default;
    virtual ~Device() = default;
};

}

#endif
