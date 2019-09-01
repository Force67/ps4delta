@echo off
setlocal enabledelayedexpansion

tools\premake\bin\win32\premake5 --file=code/premake5.lua vs2019

for /f "usebackq tokens=*" %%i in (`%cd%/tools/win/vswhere -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
  set MSBUILDDIR=%%i
)

"%MSBUILDDIR%" "%cd%/build/PS4Delta.sln" /p:Configuration=Release