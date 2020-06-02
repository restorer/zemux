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
