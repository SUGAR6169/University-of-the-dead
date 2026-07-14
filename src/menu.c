#include "menu.h"
#include "raylib.h"

void DrawMainMenu(void) { DrawText("MAIN MENU", 400, 300, 30, WHITE); }
void DrawPauseMenu(void) { DrawText("PAUSED", 400, 300, 30, WHITE); }
void DrawGameOver(void) { DrawText("GAME OVER", 400, 300, 30, RED); }
void DrawWinScreen(void) { DrawText("YOU WIN!", 400, 300, 30, GOLD); }