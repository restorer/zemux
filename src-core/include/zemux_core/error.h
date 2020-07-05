#ifndef ZEMUX_CORE__ERROR
#define ZEMUX_CORE__ERROR

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

#include <stdexcept>
#include <string>
#include <vector>

namespace zemux {

class RuntimeError : public std::runtime_error {
public:

    explicit RuntimeError(const std::string& key) : runtime_error { key }, key { key } {
    }

    inline const std::string& getKey() const {
        return key;
    }

    inline const std::vector<std::string>& getArguments() const {
        return arguments;
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

    Concrete& operator<<(const std::string& arg) {
        arguments.push_back(arg);
        return static_cast<Concrete&>(*this);
    }
};

}

#endif
