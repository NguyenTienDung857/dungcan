@echo off
setlocal

rem Run from the folder where this batch file is located
pushd "%~dp0"

del /f /q "*.cbf" 2>nul
echo Deleted all .cbf files in: %cd%

popd
endlocal
