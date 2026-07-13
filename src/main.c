#include "raylib.h"
#include "player.h"

#define SCREEN_W 800
#define SCREEN_H 600


int main(void)
{
    InitWindow(SCREEN_W, SCREEN_H, "Madrasah of the Dead");
    SetTargetFPS(60);


    Player player;

    // Initialize player stats
    InitPlayer(&player);


    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();


        // Update
        UpdatePlayer(&player, delta);


        // Draw
        BeginDrawing();

            ClearBackground(BLACK);

            DrawPlayer(&player);

            DrawText(
                "Madrasah of the Dead",
                10,
                10,
                20,
                YELLOW
            );

            DrawText(
                "WASD to Move",
                10,
                40,
                20,
                WHITE
            );

            DrawFPS(700,10);


        EndDrawing();
    }


    CloseWindow();

    return 0;
}