#!/bin/bash

cd "$(dirname "$0")"

CMAKE_PARAMS=""
CTEST="true"

# BOOST_TEST_RUN_FILTERS="..."

for ARG in "$@" ; do
    case "$ARG" in
        --clean)
            [ -e build ] && rm -r build
            ;;

        --analyze)
            CMAKE_PARAMS="$CMAKE_PARAMS -D USE_ANALYZE=On"
            ;;

        --test)
            export BOOST_TEST_LOG_LEVEL="test_suite"
            CTEST="ctest --verbose"
            ;;
    esac
done

mkdir -p build \
&& pushd build \
&& cmake $CMAKE_PARAMS .. \
&& cmake --build . \
&& $CTEST \
&& popd

exit "$?"
