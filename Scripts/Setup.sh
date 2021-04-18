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
echo "${VCPKG_FEATURE_FLAGS}"
cmake -S .. -B ../Binaries/ -DCMAKE_TOOLCHAIN_FILE=../ThirdParty/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"

cd "${DIR}"