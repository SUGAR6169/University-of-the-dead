#include "raylib.h"

#define SCREEN_W 800
#define SCREEN_H 600
#define PLAYER_SIZE 30

//Player Structure
typedef struct
{
    float x;
    float y;
    float speed;
} Player;

//Function Prototypes
void UpdateGame(Player *player, float delta);
void DrawGame(Player *player);


//Main
int main(void)
{
    InitWindow(SCREEN_W, SCREEN_H, "Madrasah of the Dead");
    SetTargetFPS(60);

    Player player = {
        400.0f,
        300.0f,
        200.0f
    };

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        UpdateGame(&player, delta);

        BeginDrawing();
            DrawGame(&player);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Update Function
void UpdateGame(Player *player, float delta)
{
    //Movement
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        player->y -= player->speed * delta;

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        player->y += player->speed * delta;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        player->x -= player->speed * delta;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        player->x += player->speed * delta;

    //To Keep player inside the window
    if (player->x < 0)
        player->x = 0;

    if (player->y < 0)
        player->y = 0;

    if (player->x > SCREEN_W - PLAYER_SIZE)
        player->x = SCREEN_W - PLAYER_SIZE;

    if (player->y > SCREEN_H - PLAYER_SIZE)
        player->y = SCREEN_H - PLAYER_SIZE;

    // Test ESC key
    if (IsKeyPressed(KEY_ESCAPE))
    {
        TraceLog(LOG_INFO, "ESC Pressed");
    }
}

// Draw Function
void DrawGame(Player *player)
{
    ClearBackground(BLACK);

    DrawRectangle(
        (int)player->x,
        (int)player->y,
        PLAYER_SIZE,
        PLAYER_SIZE,
        GREEN
    );

    DrawText("Madrasah of the Dead", 10, 10, 20, YELLOW);
    DrawText("WASD / Arrow Keys to Move", 10, 40, 20, WHITE);
    DrawText("Press ESC to test input", 10, 70, 20, GRAY);

    DrawFPS(700, 10);
}