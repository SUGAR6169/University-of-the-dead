#include "menu.h"
#include "raylib.h"

void DrawMainMenu(int selectedOption)
{
    DrawText("UNIVERSITY OF THE DEAD",
             600, 200, 45, GOLD);

    DrawText("MAIN MENU",
             760, 300, 35, WHITE);

    Color startColor = WHITE;
    Color leaderboardColor = WHITE;
    Color exitColor = WHITE;

    if (selectedOption == 0)
        startColor = GOLD;

    if (selectedOption == 1)
        leaderboardColor = GOLD;

    if (selectedOption == 2)
        exitColor = GOLD;

    DrawText("START GAME",
             750, 400, 30, startColor);

    DrawText("LEADERBOARD",
             720, 460, 30, leaderboardColor);

    DrawText("EXIT",
             785, 520, 30, exitColor);

    DrawText("Use UP / DOWN to select",
             700, 620, 20, LIGHTGRAY);

    DrawText("Press ENTER to confirm",
             700, 650, 20, LIGHTGRAY);
}

void DrawPauseMenu(void)
{
    DrawText("PAUSED", 400, 300, 30, WHITE);
}

void DrawGameOver(void)
{
    DrawText("GAME OVER", 400, 300, 30, RED);
}

void DrawWinScreen(void)
{
    DrawText("YOU WIN!", 400, 300, 30, GOLD);
}