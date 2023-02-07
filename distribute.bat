@echo off
copy release\mouse.exe distribute\mouse.exe
"C:\Qt\Qt5.13.2\6.0.4\msvc2019_64\bin\windeployqt.exe" distribute\mouse.exe