@echo off

pushd %~dp0
set VCPKG_FEATURE_FLAGS=manifests,registries
cmake -S .. -B ../Binaries/
popd