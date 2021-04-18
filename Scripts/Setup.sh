#!/bin/sh

set -e

DIR=$(pwd)
cd $(dirname "$0")

if [ $# -ne 1 ] ; then
	BUILD_TYPE=Release
else
	BUILD_TYPE=$1
fi

VCPKG_FEATURE_FLAGS=manifests,registries
cmake -S .. -B ../Binaries/ -DCMAKE_TOOLCHAIN_FILE=../ThirdParty/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"

cd "${DIR}"