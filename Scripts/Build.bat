@echo off

if "%1%" == "" (
	set BUILD_TYPE=Release
) else (
	set BUILD_TYPE=%1
)

pushd %~dp0
cmake --build ../Binaries/%BUILD_TYPE%
popd