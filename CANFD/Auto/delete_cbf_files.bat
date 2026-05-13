@echo off
setlocal

set "TARGET_DIR=%~dp0"

echo Dang xoa cac file .cbf va .vtestreport trong: "%TARGET_DIR%"

set "FOUND_FILES="

if exist "%TARGET_DIR%*.cbf" (
    del /f /q "%TARGET_DIR%*.cbf"
    set "FOUND_FILES=1"
)

if exist "%TARGET_DIR%*.vtestreport" (
    del /f /q "%TARGET_DIR%*.vtestreport"
    set "FOUND_FILES=1"
)

if defined FOUND_FILES (
    echo Da xoa xong cac file .cbf va .vtestreport.
) else (
    echo Khong tim thay file .cbf hoac .vtestreport nao trong thu muc nay.
)

pause
endlocal
