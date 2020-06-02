cmake_minimum_required (VERSION 3.10)

add_library (zemux_z80 SHARED
    src/z80cpu.cpp
)

target_include_directories (zemux_z80
    PUBLIC include
    PRIVATE src
)

target_link_libraries (zemux_z80 PRIVATE zemux_core)
target_compile_features (zemux_z80 PRIVATE cxx_std_17)

target_compile_options (zemux_z80 PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wno-unknown-pragmas
    "$<$<NOT:$<CONFIG:RELEASE>>:-O1>"
    "$<$<NOT:$<CONFIG:RELEASE>>:-fstack-protector>"
    "$<$<CONFIG:RELEASE>:-Ofast>"
)
