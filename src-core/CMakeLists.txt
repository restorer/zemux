cmake_minimum_required (VERSION 3.10)

add_library (zemux_core INTERFACE)

include (TestBigEndian)
test_big_endian (ZEMUX_BIG_ENDIAN)

configure_file (
    template/config.h.in
    "${PROJECT_BINARY_DIR}/zemux_core__template/zemux_core/config.h"
)

target_include_directories (zemux_core INTERFACE
    include
    "${PROJECT_BINARY_DIR}/zemux_core__template"
)
