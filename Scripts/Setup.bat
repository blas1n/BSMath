@echo off

pushd %~dp0

if "%1%" == "Debug" (
	set BUILD_TYPE=Debug
) else if "%1%" == "Develop" (
	set BUILD_TYPE=RelWithDebInfo
) else if "%1%" == "Shipping" (
	set BUILD_TYPE=MinSizeRel
) else if "%1" == "" (
	set BUILD_TYPE=RelWithDebInfo
) else (
	echo "Unknown build type."
	exit /b 1
)

set VCPKG_FEATURE_FLAGS=manifests,registries
cmake -S .. -B ../Binaries/ -DCMAKE_TOOLCHAIN_FILE=../ThirdParty/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
popd