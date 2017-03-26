@echo off
call %~dp0config.bat

if "%1"=="so" goto :so
if "%1"=="apk" goto :apk
if "%1"=="update" goto :update
if "%1"=="install" goto :install
if "%1"=="all" goto :all

:so
echo ===================== BUILD SO FILE =====================
%ANDROID_NDK_HOME%\ndk-build
goto :end

:apk
echo ===================== BUILD APK FILE =====================
call %APACHE_ANT%\ant debug -Dsdk.dir=%ANDROID_SDK_HOME%
goto :end

:update
echo ===================== UPDATE PROJECT =====================
call %ANDROID_SDK_HOME%\tools\android update project --target 1 --path %CD%
goto :end

:install
echo ===================== INSTALL APK =====================
call %ANDROID_SDK_HOME%\platform-tools\adb install -r %~dp0bin\GameActivity-debug.apk
goto :end

:end