# MIT License (http://www.opensource.org/licenses/mit-license.php)
#
# Copyright (c) 2020, Viachaslau Tratsiak (aka restorer)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

cmake_minimum_required (VERSION 3.10)
project (TestSuite)

##
## Config
##

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release)
endif ()

if (NOT USE_DEFAULT_FIND_FRAMEWORK)
    # Fix to be able to compile on macOS Mojave
    set (CMAKE_FIND_FRAMEWORK LAST)
endif ()

##
## Core
##

add_subdirectory (../src-core "${CMAKE_BINARY_DIR}/src-core")
link_directories ("${CMAKE_BINARY_DIR}/src-core")

##
## Z80
##

add_subdirectory (../src-z80 "${CMAKE_BINARY_DIR}/src-z80")
link_directories ("${CMAKE_BINARY_DIR}/src-z80")

##
## Zame Z80
##

set (ZAME_SRC_OUTPUT_DIR "${CMAKE_BINARY_DIR}/src-zame-z80-processed")
include_directories (${ZAME_SRC_OUTPUT_DIR})

add_subdirectory (src-zame-z80 "${CMAKE_BINARY_DIR}/src-zame-z80")
link_directories ("${CMAKE_BINARY_DIR}/src-zame-z80")

##
## ZemuX-Tests
##

add_executable (test_suite WIN32
    src/test_suite.cpp
    src/z80_test.cpp
)

target_link_libraries (test_suite PRIVATE
    zemux_core
    zemux_z80
    z80ex_wrapper
)

target_include_directories (test_suite PRIVATE src)
target_compile_features (test_suite PRIVATE cxx_std_17)

target_compile_options (test_suite PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wno-unknown-pragmas
    "$<$<NOT:$<CONFIG:RELEASE>>:-fstack-protector>"
    "$<$<CONFIG:RELEASE>:-Ofast>"
)
