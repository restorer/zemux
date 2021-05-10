#ifndef ZEMUX_CORE__VECTOR_EXT
#define ZEMUX_CORE__VECTOR_EXT

#include <vector>
#include <algorithm>
#include "force_inline.h"

namespace zemux {

template<typename T, typename P>
ZEMUX_FORCE_INLINE void vectorEraseIf(std::vector<T>& v, P predicate) {
    v.erase(std::remove_if(v.begin(), v.end(), predicate), v.end());
}

}

#endif
