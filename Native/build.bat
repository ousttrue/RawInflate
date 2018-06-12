@echo off
for /f "usebackq tokens=1* delims=: " %%i in (`vswhere -latest -requires Microsoft.Component.MSBuild`) do (
  if /i "%%i"=="installationPath" set InstallDir=%%j
)
set MSBUILD="%InstallDir%\MSBuild\15.0\Bin\MSBuild.exe"
set CMAKE="%InstallDir%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

set BUILD_DIR="build_vs2017_x64"
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%
%CMAKE% -DSKIP_INSTALL_ALL=1 -G "Visual Studio 15 2017 Win64" ..
%MSBUILD% INSTALL.vcxproj /p:Configuration=Release
popd

set BUILD_DIR="build_vs2017_x32"
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%
%CMAKE% -DSKIP_INSTALL_ALL=1 -G "Visual Studio 15 2017" ..
%MSBUILD% INSTALL.vcxproj /p:Configuration=Release
popd

