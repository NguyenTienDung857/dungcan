@echo off
setlocal enabledelayedexpansion

:: Chuyển vào thư mục chứa file bat
cd /d "%~dp0"

echo Dang xoa cac file khong phai duoi .can...

for %%f in (*) do (
    if /I not "%%~xf"==".can" (
        if /I not "%%~nxf"=="%~nx0" (
            echo Dang xoa: %%f
            del "%%f"
        )
    )
)

echo.
echo Da xoa xong cac file khong phai .can.
pause
