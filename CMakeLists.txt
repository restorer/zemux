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

# https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html

cmake_minimum_required (VERSION 3.10)
project (ZemuX)

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
## Core
##

add_subdirectory (src-core "${CMAKE_BINARY_DIR}/src-core")
link_directories ("${CMAKE_BINARY_DIR}/src-core")

##
## Z80
##

add_subdirectory (src-z80 "${CMAKE_BINARY_DIR}/src-z80")
link_directories ("${CMAKE_BINARY_DIR}/src-z80")

##
## ZemuX
##

add_executable (zemux WIN32
    src/zemux.cpp
)

target_link_libraries (zemux PRIVATE
    zemux_core
    zemux_z80
)

target_include_directories (zemux PRIVATE src)
target_compile_features (zemux PRIVATE cxx_std_17)

target_compile_options (zemux PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wno-unknown-pragmas
    "$<$<NOT:$<CONFIG:RELEASE>>:-fstack-protector>"
    "$<$<CONFIG:RELEASE>:-Ofast>"
)

##
## Analyze
##

if (USE_ANALYZE)
    add_custom_command (
        TARGET zemux
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/analysis"
    )

    add_custom_command (
        TARGET zemux
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/analysis"
    )

    add_custom_command (
        TARGET zemux
        POST_BUILD
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMAND ${PVS_STUDIO_ANALYZER_COMMAND} analyze -o analysis.log -i -j 2 -f compile_commands.json
        VERBATIM
    )

    add_custom_command (
        TARGET zemux
        POST_BUILD
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMAND ${PLOG_CONVERTER_COMMAND} -a "GA;64;OP;CS;MISRA" -t fullhtml -o analysis analysis.log
        VERBATIM
    )
endif ()
