@echo off
set TARGET="Visual Studio 14 2015"
set TOOLSET=v140
set CFGS=Debug;RelWithDebInfo;Release
set OUT=VS2015_%TOOLSET%
mkdir %OUT% 2>NUL
cmd /C "pushd %OUT% & cmake ../.. -G %TARGET% -T %TOOLSET% -DCMAKE_CONFIGURATION_TYPES=%CFGS%"
