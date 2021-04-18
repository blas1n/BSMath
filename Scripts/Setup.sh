#!/bin/sh

set -e

DIR=$(pwd)
cd $(dirname "$0")

export VCPKG_FEATURE_FLAGS=manifests,registries
cmake -S .. -B ../Binaries/

cd "${DIR}"