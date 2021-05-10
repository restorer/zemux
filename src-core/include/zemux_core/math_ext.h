#ifndef ZEMUX_CORE__MATH_EXT
#define ZEMUX_CORE__MATH_EXT

#include <cstdint>
#include "force_inline.h"

namespace zemux {

ZEMUX_FORCE_INLINE int32_t extend24To32(uint32_t value) {
    constexpr uint32_t signBit = 1 << 23;
    return (value ^ signBit) - signBit;
}

ZEMUX_FORCE_INLINE uint32_t getNearestPot(uint32_t value) {
    --value;

    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;

    return ++value;
}

}

#endif
