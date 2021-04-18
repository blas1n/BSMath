#!/bin/sh

set -e

DIR=$(pwd)
cd $(dirname "$0")

VCPKG_FEATURE_FLAGS=manifests,registries
cmake -S .. -B ../Binaries/ -DCMAKE_TOOLCHAIN_FILE=../ThirdParty/vcpkg/scripts/buildsystems/vcpkg.cmake

cd "${DIR}"