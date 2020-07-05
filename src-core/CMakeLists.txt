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

add_library (zemux_core SHARED
        src/data_io.cpp)

include (TestBigEndian)
test_big_endian (ZEMUX_BIG_ENDIAN)

configure_file (
        template/config.h.in
        "${PROJECT_BINARY_DIR}/zemux_core__template/zemux_core/config.h")

target_include_directories (zemux_core
        PUBLIC include "${PROJECT_BINARY_DIR}/zemux_core__template"
        PRIVATE src include/zemux_core)

target_compile_features (zemux_core PRIVATE cxx_std_17)

target_compile_options (zemux_core PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wno-unknown-pragmas
        "$<$<NOT:$<CONFIG:RELEASE>>:-O1>"
        "$<$<NOT:$<CONFIG:RELEASE>>:-fstack-protector>"
        "$<$<CONFIG:RELEASE>:-Ofast>")
