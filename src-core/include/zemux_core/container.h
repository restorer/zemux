#ifndef ZEMUX__CONTAINER
#define ZEMUX__CONTAINER

/*
 * MIT License (http://www.opensource.org/licenses/mit-license.php)
 *
 * Copyright (c) 2020, Viachaslau Tratsiak (aka restorer)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "non_copyable.h"
#include "force_inline.h"
#include <cstddef>
#include <utility>
#include <new>
#include <stdexcept>

namespace zemux {

template<typename T>
class Container final : private NonCopyable {
public:

    explicit Container(std::size_t capacity)
            : capacity_ { capacity },
            container_ { static_cast<T*>(::operator new(sizeof(T) * capacity)) } {
    }

    ~Container() {
        while (size_--) {
            container_[size_].~T();
        }

        ::operator delete(container_);
    }

    ZEMUX_FORCE_INLINE T& operator[](std::size_t index) noexcept {
        return container_[index];
    }

    ZEMUX_FORCE_INLINE const T& operator[](std::size_t index) const noexcept {
        return container_[index];
    }

    ZEMUX_FORCE_INLINE std::size_t capacity() const noexcept {
        return capacity_;
    }

    ZEMUX_FORCE_INLINE std::size_t size() noexcept {
        return size_;
    }

    template<typename... Args>
    void emplace_back(Args&& ... args) {
        if (size_ == capacity_) {
            throw std::out_of_range("emplace_back: out of range");
        }

        new(container_ + (size_++)) T { std::forward<Args>(args)... };
    }

private:

    Container(Container&&) = delete;
    Container& operator=(Container&&) = delete;

    std::size_t capacity_;
    std::size_t size_ = 0;
    T* container_;
};

}

#endif
