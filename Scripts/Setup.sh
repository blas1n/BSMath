#!/bin/sh

set -e

DIR=$(pwd)
cd $(dirname "$0")

case "$1" in
	"Debug")
		BUILD_TYPE=Debug;;
	"Develop"|"")
		BUILD_TYPE=RelWithDebInfo;;
	"Shipping")
		BUILD_TYPE=MinSizeRel;;
	*)
		echo "Unknown build type."
		exit 1;;
esac

VCPKG_FEATURE_FLAGS=manifests,registries
cmake -S .. -B ../Binaries/ -DCMAKE_TOOLCHAIN_FILE=../ThirdParty/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"

cd "${DIR}"