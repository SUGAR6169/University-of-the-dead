@echo off
SET PATH=C:\raylib\w64devkit\bin;%PATH%
<<<<<<< HEAD
gcc src\*.c -o game.exe ^
-IC:\raylib-6.0_win64_mingw-w64\include ^
-LC:\raylib-6.0_win64_mingw-w64\lib ^
-lraylib -lopengl32 -lgdi32 -lwinmm
pause
=======

gcc ^
src\main.c ^
src\player.c ^
src\menu.c ^
src\timer.c ^
-o game.exe ^
-IC:\raylib\include ^
-LC:\raylib\lib ^
-lraylib ^
-lopengl32 ^
-lgdi32 ^
-lwinmm

pause
>>>>>>> 5c65b08979f78725aa3cef7059bb432c7d343e78
