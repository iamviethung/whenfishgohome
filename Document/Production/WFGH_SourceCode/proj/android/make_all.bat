@echo off

call make so
if errorlevel 1 exit /B 1

call make apk
if errorlevel 1 exit /B 1

call make install