#include "hud.h"
#include "raylib.h"
#include <stdio.h>

void DrawHUD(Player *p, Timer *t) {
    // Timer display
    int mins = (int)(t->timeLeft) / 60;
    int secs = (int)(t->timeLeft) % 60;
    char timeStr[32];
    sprintf(timeStr, "TIME: %02d:%02d", mins, secs);
    Color timeColor = (t->timeLeft < 30.0f) ? RED : WHITE;
    DrawText(timeStr, 10, 10, 24, timeColor);

    // Frozen indicator
    if (t->frozen)
        DrawText("MOSQUE - TIMER FROZEN", 10, 40, 16, GREEN);

    // Stamina bar
    DrawText("STAMINA", 10, 70, 14, LIGHTGRAY);
    DrawRectangle(10, 88, 200, 12, DARKGRAY);
    int fillW = (int)(p->stamina / 100.0f * 200);
    Color stCol = (p->stamina < 30.0f) ? RED : LIME;
    DrawRectangle(10, 88, fillW, 12, stCol);
    DrawRectangleLines(10, 88, 200, 12, WHITE);
}