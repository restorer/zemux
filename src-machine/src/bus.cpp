#include "bus.h"

namespace zemux {

Bus::Bus() {
    for (int i = 0; i < LAYERS_MREQ_RD; ++i) {
        mreqRdMapLayers[i].reset(new BusMreqRdElement[ELEMENTS_MREQ_RD]);
    }

    for (int i = 0; i < LAYERS_MREQ_WR; ++i) {
        mreqWrMapLayers[i].reset(new BusMreqWrElement[ELEMENTS_MREQ_WR]);
    }

    for (int i = 0; i < LAYERS_IORQ_RD; ++i) {
        iorqRdMapLayers[i].reset(new BusIorqRdElement[ELEMENTS_IORQ_RD]);
    }

    for (int i = 0; i < LAYERS_IORQ_WR; ++i) {
        iorqWrMapLayers[i].reset(new BusIorqWrElement[ELEMENTS_IORQ_WR]);
    }
}

void Bus::onReset() {
    mreqRdLayer = 0;
    mreqWrLayer = 0;
    iorqRdLayer = 0;
    iorqWrLayer = 0;

    mreqRdMap = mreqRdMapLayers[0].get();
    mreqWrMap = mreqWrMapLayers[0].get();
    iorqRdMap = iorqRdMapLayers[0].get();
    iorqWrMap = iorqWrMapLayers[0].get();
}

void Bus::toggleMreqRdOverlay(int mreqRdOverlay, bool isEnabled) {
    if (isEnabled) {
        mreqRdLayer |= mreqRdOverlay;
    } else {
        mreqRdLayer &= (~mreqRdOverlay);
    }

    mreqRdMap = mreqRdMapLayers[mreqRdLayer].get();
}

void Bus::toggleMreqWrOverlay(int mreqWrOverlay, bool isEnabled) {
    if (isEnabled) {
        mreqWrLayer |= mreqWrOverlay;
    } else {
        mreqWrLayer &= (~mreqWrOverlay);
    }

    mreqWrMap = mreqWrMapLayers[mreqWrLayer].get();
}

void Bus::toggleIorqRdOverlay(int iorqRdOverlay, bool isEnabled) {
    if (isEnabled) {
        iorqRdLayer |= iorqRdOverlay;
    } else {
        iorqRdLayer &= (~iorqRdOverlay);
    }

    iorqRdMap = iorqRdMapLayers[iorqRdOverlay].get();
}

void Bus::toggleIorqWrOverlay(int iorqWrOverlay, bool isEnabled) {
    if (isEnabled) {
        iorqWrLayer |= iorqWrOverlay;
    } else {
        iorqWrLayer &= (~iorqWrOverlay);
    }

    iorqWrMap = iorqWrMapLayers[iorqWrOverlay].get();
}

uint32_t Bus::getFrameTicksPassed() {
    // return frameTicksPassed + cpu->getTstate();
    return 0;
}

}
