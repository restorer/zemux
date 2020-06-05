#!/bin/bash

cd "$(dirname "$0")"

CMAKE_PARAMS=""
CTEST="true"

for ARG in "$@" ; do
    IFS="=" read -a ARGV <<< "$ARG"

    case "${ARGV[0]}" in
        --clean)
            [ -e build ] && rm -r build
            ;;

        --analyze)
            CMAKE_PARAMS="$CMAKE_PARAMS -D USE_ANALYZE=On"
            ;;

        --test)
            export BOOST_TEST_LOG_LEVEL="test_suite"
            [ "${ARGV[1]}" != "" ] && export BOOST_TEST_RUN_FILTERS="${ARGV[1]}"
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
