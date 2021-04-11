#ifndef ZEMUX_CORE__ERROR
#define ZEMUX_CORE__ERROR

/*
 * MIT License (http://www.opensource.org/licenses/mit-license.php)
 *
 * Copyright (c) 2021, Viachaslau Tratsiak (aka restorer)
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

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <zemux_core/force_inline.h>

namespace zemux {

class RuntimeError : public std::exception {
public:

    explicit RuntimeError(std::string key) noexcept: key { std::move(key) } {
    }

    RuntimeError(const RuntimeError& other) noexcept
            : key { other.key }, arguments { other.arguments } {
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE const std::string& getKey() const noexcept {
        return key;
    }

    [[nodiscard]] ZEMUX_FORCE_INLINE const std::vector<std::string>& getArguments() const noexcept {
        return arguments;
    }

    [[nodiscard]] const char* what() const noexcept override {
        return key.c_str();
    }

protected:

    std::string key;
    std::vector<std::string> arguments;
};

template<typename Concrete>
class AbstractRuntimeError : public RuntimeError {
public:

    explicit AbstractRuntimeError(const std::string& key) : RuntimeError { key } {
    }

    Concrete&& operator<<(const std::string& arg) noexcept {
        arguments.push_back(arg);
        return static_cast<Concrete&&>(*this);
    }
};

}

#endif
