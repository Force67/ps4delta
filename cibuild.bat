@echo off
setlocal enabledelayedexpansion

for /f "usebackq tokens=*" %%i in (`%cd%/tools/win/vswhere -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
  set MSBUILDDIR=%%i
)

"%MSBUILDDIR%" "%cd%/build/PS4Delta.sln" /p:Configuration=Release