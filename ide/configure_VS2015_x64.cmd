@echo off
set TARGET="Visual Studio 14 2015 Win64"
set TOOLSET=v140
set CFGS=Debug;RelWithDebInfo;Release
set OUT=VS2015_%TOOLSET%_x64
mkdir %OUT% 2>NUL
cmd /C "pushd %OUT% & cmake ../.. -G %TARGET% -T %TOOLSET% -DCMAKE_CONFIGURATION_TYPES=%CFGS%"
