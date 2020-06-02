#ifndef ZEMUX_CORE__UNROLL
#define ZEMUX_CORE__UNROLL

#include "force_inline.h"

namespace zemux {

template< std::size_t N >
struct Unroll {
    template< typename F >
    ZEMUX_FORCE_INLINE static void loop(F const& block) {
        block();
        Unroll<N - 1>::loop(block);
    }
};

template<>
struct Unroll<0> {
    template< typename F >
    ZEMUX_FORCE_INLINE static void loop(F const&) {}
};

}

#endif
