#ifndef MENU_H
#define MENU_H

void InitMenu(void);
void UnloadMenu(void);

void DrawMenuBackground(void);

void DrawMainMenu(int selectedOption);
void DrawPauseMenu(void);
void DrawGameOver(void);
void DrawWinScreen(void);

#endif