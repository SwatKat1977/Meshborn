@echo off
setlocal

:: Use Visual Studio macros to get directories dynamically
set SOLUTION_DIR=%1
set PROJECT_DIR=%2
set TARGET_PATH=%3

:: Define dist directory paths dynamically
set DIST_DIR=%SOLUTION_DIR%\..\dist
set LIB_DIR=%DIST_DIR%\lib
set INCLUDE_DIR=%DIST_DIR%\include

:: Create the destination directories if they don't exist
if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"
if not exist "%INCLUDE_DIR%" mkdir "%INCLUDE_DIR%"

:: Copy the .lib file
copy /Y "%TARGET_PATH%" "%LIB_DIR%"

:: Copy all header files from the project's include directory
xcopy /Y /I /Q "%PROJECT_DIR%\*.h" "%INCLUDE_DIR%"

endlocal
