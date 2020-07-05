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

add_library (zemux_chips SHARED
        src/ay_chip.cpp
        src/z80_chip.cpp
        src/z80_chip_optable_00.cpp
        src/z80_chip_optable_CB.cpp
        src/z80_chip_optable_DD.cpp
        src/z80_chip_optable_ED.cpp
        src/z80_chip_optable_FD.cpp
        src/z80_chip_optable_DD_CB.cpp
        src/z80_chip_optable_FD_CB.cpp)

target_include_directories (zemux_chips
        PUBLIC include
        PRIVATE src include/zemux_chips)

target_link_libraries (zemux_chips PRIVATE zemux_core)
target_compile_features (zemux_chips PRIVATE cxx_std_17)

target_compile_options (zemux_chips PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wno-unknown-pragmas
        "$<$<NOT:$<CONFIG:RELEASE>>:-O1>"
        "$<$<NOT:$<CONFIG:RELEASE>>:-fstack-protector>"
        "$<$<CONFIG:RELEASE>:-Ofast>")
