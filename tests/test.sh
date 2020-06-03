#!/bin/bash

cd "$(dirname "$0")"
CMAKE_PARAMS=""

for ARG in "$@" ; do
    case "$ARG" in
        --clean)
            [ -e ../build-tests ] && rm -r ../build-tests
            ;;
    esac
done

mkdir -p ../build-tests \
&& pushd ../build-tests \
&& cmake $CMAKE_PARAMS ../tests \
&& cmake --build . \
&& popd \
&& ../build-tests/test_suite

exit "$?"
