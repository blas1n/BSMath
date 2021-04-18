#!/bin/sh

set -e

DIR=$(pwd)
cd $(dirname "$0")

if [ $# -ne 1 ] ; then
	BUILD_TYPE=$1
else
	BUILD_TYPE=Release
fi

cmake --build ../Binaries/${BUILD_TYPE}

cd "${DIR}"