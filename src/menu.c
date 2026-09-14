#include "menu.h"
#include "raylib.h"

static Texture2D menuBackground;

void InitMenu(void)
{
    menuBackground = LoadTexture("assets/menu_bg.png");
}

void UnloadMenu(void)
{
    UnloadTexture(menuBackground);
}

void DrawMainMenu(int selectedOption)
{
    DrawMenuBackground(); //draws assets/menu_bg.png as background 

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Title
    const char *title = "UNIVERSITY OF THE DEAD";
    int titleSize = 45;
    int titleWidth = MeasureText(title, titleSize);

    DrawText(
        title, //text
        (screenWidth - titleWidth) / 2, //x position
        screenHeight / 2 - 250, //y position
        titleSize,
        GOLD
    );

    // Main Menu heading
    const char *heading = "MAIN MENU";
    int headingSize = 35;
    int headingWidth = MeasureText(heading, headingSize);

    DrawText(
        heading,
        (screenWidth - headingWidth) / 2,
        screenHeight / 2 - 170,
        headingSize,
        WHITE
    );

    // Menu option colors
    Color startColor = WHITE;
    Color leaderboardColor = WHITE;
    Color exitColor = WHITE;

    //Highlight the selected option 
    if (selectedOption == 0)
        startColor = GOLD;

    if (selectedOption == 1)
        leaderboardColor = GOLD;

    if (selectedOption == 2)
        exitColor = GOLD;

    // START GAME
    const char *startText = "START GAME";
    int startSize = 30;
    int startWidth = MeasureText(startText, startSize);

    DrawText(
        startText, //text 
        (screenWidth - startWidth) / 2, //x
        screenHeight / 2 - 30, //y
        startSize,
        startColor
    );

    // LEADERBOARD
    const char *leaderboardText = "LEADERBOARD";
    int leaderboardSize = 30;
    int leaderboardWidth = MeasureText(leaderboardText, leaderboardSize);

    DrawText(
        leaderboardText,
        (screenWidth - leaderboardWidth) / 2,
        screenHeight / 2 + 30,
        leaderboardSize,
        leaderboardColor
    );

    // EXIT
    const char *exitText = "EXIT";
    int exitSize = 30;
    int exitWidth = MeasureText(exitText, exitSize);

    DrawText(
        exitText,
        (screenWidth - exitWidth) / 2,
        screenHeight / 2 + 90,
        exitSize,
        exitColor
    );

    // Instructions
    const char *instruction1 = "Use UP / DOWN to select";
    const char *instruction2 = "Press ENTER to confirm";

    int instructionSize = 20;

    int instruction1Width = MeasureText(instruction1, instructionSize);
    int instruction2Width = MeasureText(instruction2, instructionSize);

    DrawText(
        instruction1,
        (screenWidth - instruction1Width) / 2,
        screenHeight / 2 + 180,
        instructionSize,
        LIGHTGRAY
    );

    DrawText(
        instruction2,
        (screenWidth - instruction2Width) / 2,
        screenHeight / 2 + 210,
        instructionSize,
        LIGHTGRAY
    );
}

void  DrawMenuBackground(void)
{
    DrawTexturePro(
        menuBackground,
        (Rectangle){
            0, 0,
            (float)menuBackground.width,
            (float)menuBackground.height
        },
        (Rectangle){
            0, 0,
            (float)GetScreenWidth(),
            (float)GetScreenHeight()
        },
        (Vector2){0, 0},
        0.0f,
        WHITE
    );

    DrawRectangle(
        0, 0,
        GetScreenWidth(),
        GetScreenHeight(),
        Fade(BLACK, 0.35f)
    );
}

void DrawPauseMenu(void)
{
    DrawText("PAUSED", 400, 300, 30, WHITE);
}

void DrawGameOver(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  Fade(RED, 0.6f));

    const char *text = "GAME OVER";
    int fontSize = 80;
    int textWidth = MeasureText(text, fontSize);

    DrawText(text,
             (GetScreenWidth() - textWidth) / 2,
             GetScreenHeight() / 2 - 40,
             fontSize,
             WHITE);

    const char *prompt = "Press ENTER to return to menu";
    int promptWidth = MeasureText(prompt, 20);

    DrawText(prompt,
             (GetScreenWidth() - promptWidth) / 2,
             GetScreenHeight() / 2 + 80,
             20,
             LIGHTGRAY);
}

void DrawWinScreen(void)
{
    DrawText("YOU WIN!", 400, 300, 30, GOLD);
}