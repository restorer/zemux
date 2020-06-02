#ifndef ZEMUX_CORE__FORCE_INLINE
#define ZEMUX_CORE__FORCE_INLINE

// Taken out from Boost to reduce core dependencies.
// This version is modified for ZemuX. Original info:
//
// Copyright (c) 2001-2003 John Maddock
// Copyright (c) 2001 Darin Adler
// Copyright (c) 2001 Peter Dimov
// Copyright (c) 2002 Bill Kempf
// Copyright (c) 2002 Jens Maurer
// Copyright (c) 2002-2003 David Abrahams
// Copyright (c) 2003 Gennaro Prota
// Copyright (c) 2003 Eric Friedman
// Copyright (c) 2010 Eric Jourdanneau, Joel Falcou
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if defined(_MSC_VER)
    #define ZEMUX_FORCE_INLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ > 3 // Also for clang
    #define ZEMUX_FORCE_INLINE inline __attribute__ ((__always_inline__))
#else
    #define ZEMUX_FORCE_INLINE inline
#endif

#endif
