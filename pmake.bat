@echo off

tools\premake\bin\win32\premake5 --file=code/premake5.lua vs2019
timeout /t 3 /nobreak
