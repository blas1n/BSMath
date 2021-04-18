@echo off

pushd %~dp0
setx VCPKG_FEATURE_FLAGS manifests,registries
cmake -S .. -B ../Binaries/
popd