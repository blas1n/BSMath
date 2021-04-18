#!/bin/sh

set -e

DIR=$(pwd)
cd $(dirname "$0")

if [ $# -ne 1 ] ; then
	BUILD_TYPE=$1
else
	BUILD_TYPE=Release
fi

export VCPKG_FEATURE_FLAGS=manifests,registries
cmake -S .. -B ../Binaries/${BUILD_TYPE} -DCMAKE_TOOLCHAIN_FILE="$(pwd)"/../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE}

cd "${DIR}"