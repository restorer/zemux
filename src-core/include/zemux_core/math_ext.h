#ifndef ZEMUX_CORE__MATH
#define ZEMUX_CORE__MATH

#include <cstdint>
#include "force_inline.h"

namespace zemux {

ZEMUX_FORCE_INLINE int32_t extend24To32(uint32_t value) {
    constexpr uint32_t signBit = 1 << 23;
    return (value ^ signBit) - signBit;
}

}

#endif
