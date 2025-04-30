@echo off
setlocal

:: Get parameters
set SOLUTION_DIR=%1
set PROJECT_DIR=%2
set TARGET_PATH=%3
set CONFIGURATION=%4

:: Trim extra spaces or quotes from the configuration value
set CONFIGURATION=%CONFIGURATION:"=%
set CONFIGURATION=%CONFIGURATION: =%

:: Define dist directory paths dynamically
set DIST_DIR=%SOLUTION_DIR%\..\dist
set LIB_DIR=%DIST_DIR%\lib
set INCLUDE_DIR=%DIST_DIR%\include

:: Create the destination directories if they don't exist
if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"
if not exist "%INCLUDE_DIR%" mkdir "%INCLUDE_DIR%"

:: Determine the appropriate lib file name based on configuration (Debug/Release)
if /I "%CONFIGURATION%"=="Debug" (
    echo Debug build detected
    set LIB_FILE_NAME=Meshborn_d.lib
) else (
    echo Release build detected
    set LIB_FILE_NAME=Meshborn.lib
)

:: Copy the .lib file and rename it accordingly
echo Copying lib file...
copy /Y "%TARGET_PATH%" "%LIB_DIR%\%LIB_FILE_NAME%"

:: Copy all header files from the project's include directory
echo Copying header files...
xcopy /Y /I /Q "%PROJECT_DIR%\*.h" "%INCLUDE_DIR%"

endlocal
