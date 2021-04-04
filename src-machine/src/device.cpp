#include "device.h"

namespace zemux {

void Device::onAttach(Bus* bus) {
    attachedBus = bus;
}

void Device::onDetach() {
    attachedBus = nullptr;
}

}
