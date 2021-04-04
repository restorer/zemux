#ifndef ZEMUX_MACHINE__EXTPORT_DEVICE
#define ZEMUX_MACHINE__EXTPORT_DEVICE

#include "device.h"
#include <zemux_core/non_copyable.h>
#include <zemux_core/force_inline.h>
#include <cstdint>

namespace zemux {

class ExtPortDevice final : public Device, private NonCopyable {
public:

    static constexpr uint8_t BIT_16_COLORS = 1; // 16 colors (4bits per pixel)
    static constexpr uint8_t BIT_512x192 = 2; // 512x192 monochrome
    static constexpr uint8_t BIT_128_LOCK = 4; // disable memory above 128k
    static constexpr uint8_t BIT_RAM_MAP_ROM = 8; // first ram page (0 bank) maps to rom
    static constexpr uint8_t BIT_TURBO = 16; // turbo (0 - on, 1 - off) pentagon sl 2.2
    static constexpr uint8_t BIT_MULTICOLOR = 32; // hardware multicolor
    static constexpr uint8_t BIT_384x304 = 64; // 384x304 mode
    static constexpr uint8_t BIT_CMOS = 128; // cmos

    static constexpr uint8_t BIT_OLD_MULTICOLOR = 1; // hardware multicolor
    static constexpr uint8_t BIT_OLD_CACHE_RO = 8; // read only cache
    static constexpr uint8_t BIT_OLD_GIGASCREEN = 16; // gigascreen
    static constexpr uint8_t BIT_OLD_16_COLORS = 32; // 16 colors (4bits per pixel)

    explicit ExtPortDevice(Bus* bus);
    virtual ~ExtPortDevice() = default;

    void onAttach() override;
    void onDetach() override;

    ZEMUX_FORCE_INLINE bool is16Colors() {
        return portEFF7 & (isOldMode ? BIT_OLD_16_COLORS : BIT_16_COLORS);
    }

    ZEMUX_FORCE_INLINE bool is512x192() {
        return portEFF7 & BIT_512x192;
    }

    ZEMUX_FORCE_INLINE bool is128Lock() {
        return portEFF7 & BIT_128_LOCK;
    }

    ZEMUX_FORCE_INLINE bool isRamMapRom() {
        return portEFF7 & BIT_RAM_MAP_ROM;
    }

    ZEMUX_FORCE_INLINE bool isGigascreen() {
        return !isTurboMode && (portEFF7 & BIT_OLD_GIGASCREEN);
    }

    ZEMUX_FORCE_INLINE bool isMulticolor() {
        return portEFF7 & (isOldMode ? BIT_OLD_MULTICOLOR : BIT_MULTICOLOR);
    }

    ZEMUX_FORCE_INLINE bool is384x304() {
        return portEFF7 & BIT_384x304;
    }

    ZEMUX_FORCE_INLINE bool isCmos() {
        return portEFF7 & BIT_CMOS;
    }

private:

    uint8_t portEFF7 = 0;
    bool isOldMode = false;
    bool isTurboMode = false;
};

}

#endif
