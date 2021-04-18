@echo off

pushd %~dp0
set VCPKG_FEATURE_FLAGS=manifests,registries
cmake -S .. -B ../Binaries/ -DCMAKE_TOOLCHAIN_FILE=../ThirdParty/vcpkg/scripts/buildsystems/vcpkg.cmake
popd