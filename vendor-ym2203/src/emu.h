#ifndef ZEMUX_VENDOR_YM2203__EMU
#define ZEMUX_VENDOR_YM2203__EMU

#include <zemux_core/force_inline.h>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <utility>
#include <new>

template<typename T, typename... Args>
ZEMUX_FORCE_INLINE T* global_alloc_clear(Args&& ... args) {
    void* const ptr = ::operator new(sizeof(T));
    std::memset(ptr, 0, sizeof(T));
    return new(ptr) T { std::forward<Args>(args)... };
}

#define auto_alloc_clear(_machine, _type) global_alloc_clear _type
#define auto_free(_machine, v) ::operator delete(v);

#endif
