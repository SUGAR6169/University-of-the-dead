@echo off
SET PATH=C:\raylib\w64devkit\bin;%PATH%

gcc ^
src\main.c ^
src\player.c ^
-o game.exe ^
-IC:\raylib\include ^
-LC:\raylib\lib ^
-lraylib ^
-lopengl32 ^
-lgdi32 ^
-lwinmm

pause