#ifndef ZEMUX_MACHINE__BUS
#define ZEMUX_MACHINE__BUS

#include <memory>
#include <array>
#include <zemux_core/non_copyable.h>

namespace zemux {

struct BusMreqRdElement {
    uint8_t (* callback)(void* data, uint16_t address, bool isM1);
    void* data;
};

struct BusMreqWrElement {
    void (* callback)(void* data, uint16_t address, uint8_t value);
    void* data;
};

struct BusIorqRdElement {
    uint8_t (* callback)(void* data, uint16_t port);
    void* data;
};

struct BusIorqWrElement {
    void (* callback)(void* data, uint16_t port, uint8_t value);
    void* data;
};

class Bus final : private NonCopyable {
public:

    static constexpr int ELEMENTS_MREQ_RD = 0x20000;
    static constexpr int ELEMENTS_MREQ_WR = 0x10000;
    static constexpr int ELEMENTS_IORQ_RD = 0x10000;
    static constexpr int ELEMENTS_IORQ_WR = 0x10000;

    static constexpr int LAYERS_MREQ_RD = 2;
    static constexpr int LAYERS_MREQ_WR = 1;
    static constexpr int LAYERS_IORQ_RD = 2;
    static constexpr int LAYERS_IORQ_WR = 2;

    static constexpr int OVERLAY_MREQ_RD_TRDOS = 1;
    static constexpr int OVERLAY_IORQ_RD_TRDOS = 1;
    static constexpr int OVERLAY_IORQ_WR_TRDOS = 1;

    BusMreqRdElement* mreqRdMap;
    BusMreqWrElement* mreqWrMap;
    BusIorqRdElement* iorqRdMap;
    BusIorqWrElement* iorqWrMap;

    std::array<std::unique_ptr<BusMreqRdElement[]>, LAYERS_MREQ_RD> mreqRdMapLayers;
    std::array<std::unique_ptr<BusMreqWrElement[]>, LAYERS_MREQ_WR> mreqWrMapLayers;
    std::array<std::unique_ptr<BusIorqRdElement[]>, LAYERS_IORQ_RD> iorqRdMapLayers;
    std::array<std::unique_ptr<BusIorqWrElement[]>, LAYERS_IORQ_WR> iorqWrMapLayers;

    Bus();
    ~Bus() = default;

    void onReset();
    void toggleMreqRdOverlay(int mreqRdOverlay, bool isEnabled);
    void toggleMreqWrOverlay(int mreqWrOverlay, bool isEnabled);
    void toggleIorqRdOverlay(int iorqRdOverlay, bool isEnabled);
    void toggleIorqWrOverlay(int iorqWrOverlay, bool isEnabled);
    uint32_t getFrameTicksPassed();

private:

    int mreqRdLayer;
    int mreqWrLayer;
    int iorqRdLayer;
    int iorqWrLayer;
};

}

#endif
