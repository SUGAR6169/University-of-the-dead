@echo off
SET PATH=C:\raylib\w64devkit\bin;%PATH%
gcc src\*.c -o game.exe ^
-IC:\raylib-6.0_win64_mingw-w64\include ^
-LC:\raylib-6.0_win64_mingw-w64\lib ^
-lraylib -lopengl32 -lgdi32 -lwinmm
pause