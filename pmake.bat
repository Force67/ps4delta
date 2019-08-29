@echo off

cd code
..\tools\premake\bin\win32\premake5 %* vs2019
timeout /t 3 /nobreak
