#include "devices/extport_device.h"

namespace zemux {

class MemoryDevice {
public:

    void remap();
};

ExtPortDevice::ExtPortDevice(Bus* bus) : Device { bus } {
}

void ExtPortDevice::onAttach() {
    Device::onAttach();
    bus->extPortDevice = this;

    if (bus->memoryDevice != nullptr) {
        bus->memoryDevice->remap();
    }
}

void ExtPortDevice::onDetach() {
    bus->extPortDevice = nullptr;

    if (bus->memoryDevice != nullptr) {
        bus->memoryDevice->remap();
    }

    Device::onDetach();
}

}
