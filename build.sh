#!/bin/bash

cd "$(dirname "$0")" || exit

BUILD_DIR="cmake-build"
CMAKE_CMD="cmake"
CTEST_CMD="true"

for ARG in "$@"; do
    IFS="=" read -r -a ARGV <<< "$ARG"

    case "${ARGV[0]}" in
        --clean)
            [ -e "$BUILD_DIR" ] && rm -r "$BUILD_DIR"
            ;;

        --analyze)
            CMAKE_CMD="$CMAKE_CMD -D USE_ANALYZE=On"
            ;;

        --test)
            export BOOST_TEST_LOG_LEVEL="test_suite"
            [ "${ARGV[1]}" != "" ] && export BOOST_TEST_RUN_FILTERS="${ARGV[1]}"
            CTEST_CMD="ctest --verbose"
            ;;
    esac
done

# shellcheck disable=SC2164
mkdir -p "$BUILD_DIR" &&
    pushd "$BUILD_DIR" &&
    $CMAKE_CMD .. &&
    cmake --build . &&
    $CTEST_CMD &&
    popd

exit "$?"
