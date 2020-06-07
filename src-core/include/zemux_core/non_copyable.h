#ifndef ZEMUX_CORE__NON_COPYABLE
#define ZEMUX_CORE__NON_COPYABLE

// Taken out from Boost to reduce core dependencies.
// This version is modified for ZemuX. Original info:
//
// (C) Copyright Beman Dawes 1999-2003. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

namespace zemux {

class NonCopyable {
public:

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

protected:

    constexpr NonCopyable() = default;
    ~NonCopyable() = default;
};

}

#endif
