# MIT License (http://www.opensource.org/licenses/mit-license.php)
#
# Copyright (c) 2021, Viachaslau Tratsiak (aka restorer)
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

# https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html

cmake_minimum_required (VERSION 3.10)
project (ZemuX)
enable_testing ()

##
## Config
##

if (USE_ANALYZE)
    find_program (PVS_STUDIO_ANALYZER_COMMAND "pvs-studio-analyzer")
    find_program (PLOG_CONVERTER_COMMAND "plog-converter")

    if (PVS_STUDIO_ANALYZER_COMMAND AND PLOG_CONVERTER_COMMAND)
        message ("PVS-Studio files found")
    else ()
        message (FATAL_ERROR "PVS-Studio files not found (required to perform static analysis)")
    endif ()

    set (CMAKE_EXPORT_COMPILE_COMMANDS On)
elseif (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release)
endif ()

if (NOT USE_DEFAULT_FIND_FRAMEWORK)
    # Fix to be able to compile on macOS Mojave
    set (CMAKE_FIND_FRAMEWORK LAST)
endif ()

##
## ZemuX modules
##

add_subdirectory (vendor-ym2203 "${CMAKE_BINARY_DIR}/vendor-ym2203")
link_directories ("${CMAKE_BINARY_DIR}/vendor-ym2203")

add_subdirectory (vendor-saa1099 "${CMAKE_BINARY_DIR}/vendor-saa1099")
link_directories ("${CMAKE_BINARY_DIR}/vendor-saa1099")

add_subdirectory (src-core "${CMAKE_BINARY_DIR}/src-core")
link_directories ("${CMAKE_BINARY_DIR}/src-core")

add_subdirectory (src-integrated "${CMAKE_BINARY_DIR}/src-chips")
link_directories ("${CMAKE_BINARY_DIR}/src-chips")

add_subdirectory (src-vendor "${CMAKE_BINARY_DIR}/src-vendor")
link_directories ("${CMAKE_BINARY_DIR}/src-vendor")

add_subdirectory (src-machine "${CMAKE_BINARY_DIR}/src-machine")
link_directories ("${CMAKE_BINARY_DIR}/src-machine")

##
## ZemuX
##

add_executable (zemux WIN32
        src/zemux.cpp)

target_link_libraries (zemux PRIVATE
        zemux_core
        zemux_integrated
        zemux_machine
        zemux_vendor
        vendor_ym2203
        vendor_saa1099)

target_include_directories (zemux PRIVATE src)
target_compile_features (zemux PRIVATE cxx_std_17)

target_compile_options (zemux PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wno-unknown-pragmas
        "$<$<NOT:$<CONFIG:RELEASE>>:-fstack-protector>"
        "$<$<CONFIG:RELEASE>:-Ofast>")

##
## Test modules
##

set (ZAME_SRC_OUTPUT_DIR "${CMAKE_BINARY_DIR}/test-vendor-zame-z80__processed")
add_subdirectory (test-vendor-zame-z80 "${CMAKE_BINARY_DIR}/test-vendor-zame-z80")
link_directories ("${CMAKE_BINARY_DIR}/test-vendor-zame-z80")
include_directories ("${ZAME_SRC_OUTPUT_DIR}")

##
## Test
##

find_package (Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIR})

add_executable (zemux_test WIN32
        test/runner.cpp
        test/chronometer_test.cpp
        test/z80_correctness_test.cpp
        test/z80_speed_test.cpp
        test/tape_test.cpp)

target_link_libraries (zemux_test PRIVATE
        zemux_core
        zemux_integrated
        z80ex_wrapper
        ${Boost_LIBRARIES})

target_include_directories (zemux_test PRIVATE test)
target_compile_features (zemux_test PRIVATE cxx_std_17)

target_compile_options (zemux_test PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wno-unknown-pragmas
        "$<$<NOT:$<CONFIG:RELEASE>>:-fstack-protector>"
        "$<$<CONFIG:RELEASE>:-Ofast>")

add_test (NAME test COMMAND zemux_test)

##
## Analyze
##

if (USE_ANALYZE)
    add_custom_command (
            TARGET zemux
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/analysis")

    add_custom_command (
            TARGET zemux
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/analysis")

    add_custom_command (
            TARGET zemux
            POST_BUILD
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMAND ${PVS_STUDIO_ANALYZER_COMMAND} analyze -o analysis.log -i -j 2 -f compile_commands.json
            VERBATIM)

    add_custom_command (
            TARGET zemux
            POST_BUILD
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMAND ${PLOG_CONVERTER_COMMAND} -a "GA;64;OP;CS;MISRA" -t fullhtml -o analysis analysis.log
            VERBATIM)
endif ()
