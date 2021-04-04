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

add_library (zemux_machine SHARED
        src/bus.cpp
        src/device.cpp
        src/devices/border_device.cpp
        src/devices/memory_device.cpp
        src/devices/covox_device.cpp
        src/devices/extport_device.cpp
        src/machine.cpp
        src/video_surface.cpp)

target_include_directories (zemux_machine
        PUBLIC include
        PRIVATE src include/zemux_machine)

target_link_libraries (zemux_machine PRIVATE zemux_core zemux_chips)
target_compile_features (zemux_machine PRIVATE cxx_std_17)

target_compile_options (zemux_machine PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wno-unknown-pragmas
        "$<$<NOT:$<CONFIG:RELEASE>>:-O1>"
        "$<$<NOT:$<CONFIG:RELEASE>>:-fstack-protector>"
        "$<$<CONFIG:RELEASE>:-Ofast>")
