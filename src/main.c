#include <stdio.h>    // Required for sprintf (Debug text)
#include "raylib.h"
#include "map.h"
#include "player.h"
#include "timer.h"
#include "hud.h"
#include "menu.h"
#include "leaderboard.h"


// Game states
typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAMEOVER,
    STATE_WIN,
    STATE_LEADERBOARD
} GameState;

GameState gameState = STATE_MENU;

int menuSelection = 0;

// Global debug toggle
int debugMode = 0;

/* Leaderboard name entry */
char playerName[PLAYER_NAME_LENGTH] = "";
int nameLength = 0;
int scoreSubmitted = 0;

int main(void) {

    // Start the game directly in fullscreen mode
        SetConfigFlags(FLAG_FULLSCREEN_MODE);

        // Create the fullscreen game window
        InitWindow(0, 0, "University of the Dead");

        SetExitKey(KEY_NULL);

        // Get the actual screen resolution being used
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        SetTargetFPS(120);

    // Declare core game objects
    Player player;
    Timer timer;

    // Load assets and initialize game objects
    LoadMapData();
    InitPlayer(&player);  
    InitTimer(&timer, 30.0f); // Set to 30 seconds initial countdown
    InitLeaderboard();
    InitMenu();

    // Set up Camera2D — this follows the player
    Camera2D camera = {0};
    camera.zoom   = 0.25f;  // Zoomed out to see the map layout on the menu
    camera.offset = (Vector2){
        screenWidth / 2.0f, //x cordinate
        screenHeight / 2.0f //y co 
        };
    camera.target = (Vector2){MAP_WIDTH / 2.0f, MAP_HEIGHT / 2.0f}; // Centers the map on the menu

    // Player starts at Main Gate position on the map
    player.x = 3000.0f;
    player.y = 2200.0f;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        float time  = GetTime();

        // ── GLOBAL HOTKEYS ──────────────────────────────────
       
        // ── UPDATE SYSTEM ──────────────────────────────────
        switch (gameState) {
            case STATE_MENU:

                if (IsKeyPressed(KEY_DOWN))
                {
                    menuSelection++;

                    if (menuSelection > 2)
                        menuSelection = 0;
                }

                if (IsKeyPressed(KEY_UP))
                {
                    menuSelection--;

                    if (menuSelection < 0)
                        menuSelection = 2;
                }

                if (IsKeyPressed(KEY_ENTER))
                {
                    if (menuSelection == 0)
                    {
                        // Start Game
                        gameState = STATE_PLAYING;

                        camera.zoom = 0.65f;

                        UnloadPlayer(&player);
                        //initplayer has been already called before once
                        //before calling it again freeing memory is must

                        InitPlayer(&player);
                        InitTimer(&timer, 30.0f);

                        player.x = 3000.0f;
                        player.y = 2200.0f;

                        nameLength = 0;
                        playerName[0] = '\0';
                        scoreSubmitted = 0;
                    }
                    else if (menuSelection == 1)
                    {
                        // Open Leaderboard
                        gameState = STATE_LEADERBOARD;
                    }
                   else if (menuSelection == 2)
                    {
                        CloseWindow();
                        return 0;
                    }
                }

                break;

            case STATE_LEADERBOARD:

                    if (IsKeyPressed(KEY_ESCAPE))
                    {
                        menuSelection = 0;
                        gameState = STATE_MENU;
                    }

                    break;

            case STATE_PLAYING: {
                // Toggle Debug Overlay Mode
                if (IsKeyPressed(KEY_F1)) debugMode = !debugMode;

                // 1. Update Camera Position to track the player coordinate origin
                camera.target = (Vector2){
                    player.x + player.w / 2.0f,
                    player.y + player.h / 2.0f
                };

                // ── ADVANCED CAMERA BOUNDARY CLAMPING ────────────────────
                // Prevents the camera from viewing past the boundaries of the IUT map image
               float minX = screenWidth / (2.0f * camera.zoom);
               float maxX = MAP_WIDTH - (screenWidth / (2.0f * camera.zoom));

                float minY = screenHeight / (2.0f * camera.zoom);
                float maxY = MAP_HEIGHT - (screenHeight / (2.0f * camera.zoom));

                if (minX > maxX) {
                    camera.target.x = MAP_WIDTH / 2.0f;
                } else {
                    if (camera.target.x < minX) camera.target.x = minX;
                    if (camera.target.x > maxX) camera.target.x = maxX;
                }

                if (minY > maxY) {
                    camera.target.y = MAP_HEIGHT / 2.0f;
                } else {
                    if (camera.target.y < minY) camera.target.y = minY;
                    if (camera.target.y > maxY) camera.target.y = maxY;
                }
                // ────────────────────────────────────────────────────────

                // 2. Camera Zoom Controls via Mouse Wheel
                camera.zoom += GetMouseWheelMove() * 0.02f;
                
                // Keep user zoom scale configurations within constraints
                if (camera.zoom < 0.3f)  camera.zoom = 0.3f;
                if (camera.zoom > 1.0f)  camera.zoom = 1.0f;

                // Fixed manual zoom controls with keyboard (+ / - keys)
                if (IsKeyPressed(KEY_EQUAL)) camera.zoom += 0.05f; 
                if (IsKeyPressed(KEY_MINUS)) camera.zoom -= 0.05f; 

                // 3. Update Player Physics (Movement handling & wall collisions)
                UpdatePlayer(&player, delta);

                // 4. Zone Interaction & Timer Management Logic
                Rectangle pRect = {player.x, player.y, player.w, player.h};
                Zone *currentZone = GetCurrentZone(pRect);

                // Mosque area features custom mechanic: freezes game clock
                timer.frozen = (currentZone && currentZone->type == ZONE_MOSQUE) ? 1 : 0;
                UpdateTimer(&timer, delta);

                // 5. Evaluation of Terminal Game States
                // Win condition — reached the target top left exit zone
                if (currentZone && currentZone->type == ZONE_EXIT) {
                    gameState = STATE_WIN;
                }

                // Lose condition — Countdown timer expires
                if (timer.timeLeft <= 0.0f) gameState = STATE_GAMEOVER;

                // Pause State Switch
                if (IsKeyPressed(KEY_ESCAPE))
                    {
                        menuSelection = 0;
                        gameState = STATE_MENU;
                    }
                break;
            }

            case STATE_PAUSED:
                if (IsKeyPressed(KEY_ESCAPE)) gameState = STATE_PLAYING;
                if (IsKeyPressed(KEY_Q))      gameState = STATE_MENU;
                break;

            // ── GAME OVER SCREEN ──
            //The actual DrawGameOver() at the bottom is inside BeginDrawing() and is working correctly
            //So replaced the unnecessary case state_gameover
            case STATE_GAMEOVER:
    if (IsKeyPressed(KEY_ENTER)) {
        gameState = STATE_MENU;
        menuSelection = 0;
    }
    break;

           // ── WIN STATE ──
            case STATE_WIN: {

                /* Get characters typed by the player */
                int key = GetCharPressed();

                while (key > 0)
                {
                    /* Accept normal printable characters */
                    if (key >= 32 &&
                        key <= 125 &&
                        nameLength < PLAYER_NAME_LENGTH - 1)
                    {
                        playerName[nameLength] = (char)key;
                        nameLength++;

                        /* Keep the string terminated */
                        playerName[nameLength] = '\0';
                    }

                    key = GetCharPressed();
                }

                /* Delete the last character */
                if (IsKeyPressed(KEY_BACKSPACE) && nameLength > 0)
                {
                    nameLength--;
                    playerName[nameLength] = '\0';
                }

                /* Submit score */
                if (IsKeyPressed(KEY_ENTER) &&
                    nameLength > 0 &&
                    scoreSubmitted == 0)
                {
                    AddLeaderboardScore(playerName, timer.timeLeft);

                    scoreSubmitted = 1;
                }

                /* Return to main menu after submitting */
                if (scoreSubmitted &&
                    IsKeyPressed(KEY_ESCAPE))
                {
                    gameState = STATE_MENU;
                }

                break;
            }
        }

        // ── DRAW PIPELINE ───────────────────────────────────
        BeginDrawing();
        ClearBackground(BLACK);

        // 1. WORLD SPACE SYSTEM (Camera Transformations Applied)
       if (gameState == STATE_PLAYING || gameState == STATE_PAUSED)
        {
                BeginMode2D(camera);

                    DrawMapBackground(camera);
                    DrawZoneHighlights(time);

                    DrawPlayer(&player);
                

        if (debugMode)
        {
            for (int i = 0; i < wallCount; i++) {
                    DrawRectangleLinesEx(walls[i], 3, RED);
                 }
                    for (int i = 0; i < zoneCount; i++) {
                    DrawRectangleLinesEx(zones[i].rect, 2, BLUE);
                }
        }

     EndMode2D();
        }

        // 2. SCREEN SPACE SYSTEM (Static UI Text Overlay)
        if (debugMode) {
            char posText[64];
            sprintf(posText, "POS: %.0f, %.0f | ZOOM: %.2f", player.x, player.y, camera.zoom);
            DrawText(posText, 10, screenHeight - 30, 16, YELLOW);
        }

        // Route render pipeline execution based on current structural game state
 if (gameState == STATE_PLAYING)
    DrawHUD(&player, &timer);

if (gameState == STATE_PAUSED)
    DrawPauseMenu();

if (gameState == STATE_MENU)
    DrawMainMenu(menuSelection);

if (gameState == STATE_LEADERBOARD)
{

    DrawMenuBackground();
    
    DrawLeaderboard();

    DrawText("Press ESC to return to menu",
             650,
             700,
             20,
             LIGHTGRAY);
}    

if (gameState == STATE_GAMEOVER)
    DrawGameOver();

if (gameState == STATE_WIN)
{
    int fontSize = 80;

    const char *text = "YOU WIN!";

    int textWidth = MeasureText(text, fontSize);

    DrawMenuBackground();

    /* Main WIN text */
    DrawText(
        text,
        (GetScreenWidth() - textWidth) / 2,
        GetScreenHeight() / 2 - 180,
        fontSize,
        WHITE
    );

    /* Show remaining time */
    char timeText[64];

    sprintf(timeText, "TIME REMAINING: %.2f seconds",
            timer.timeLeft);

    int timeWidth = MeasureText(timeText, 30);

    DrawText(
        timeText,
        (GetScreenWidth() - timeWidth) / 2,
        GetScreenHeight() / 2 - 70,
        30,
        GOLD
    );

    if (!scoreSubmitted)
    {
        /* Name entry */
        const char *nameLabel = "ENTER YOUR NAME";

        int nameLabelWidth = MeasureText(nameLabel, 25);

        DrawText(
            nameLabel,
            (GetScreenWidth() - nameLabelWidth) / 2,
            GetScreenHeight() / 2,
            25,
            WHITE
        );

        /* Name input box */
        DrawRectangle(
            GetScreenWidth() / 2 - 250,
            GetScreenHeight() / 2 + 45,
            500,
            55,
            DARKGRAY
        );

        DrawText(
            playerName,
            GetScreenWidth() / 2 - 230,
            GetScreenHeight() / 2 + 58,
            25,
            WHITE
        );

        DrawText(
            "Press ENTER to submit",
            GetScreenWidth() / 2 - 130,
            GetScreenHeight() / 2 + 125,
            20,
            WHITE
        );

        DrawText(
            "BACKSPACE to delete",
            GetScreenWidth() / 2 - 120,
            GetScreenHeight() / 2 + 155,
            20,
            LIGHTGRAY
        );
    }
    else
    {
        DrawText(
            "SCORE SAVED!",
            GetScreenWidth() / 2 - 100,
            GetScreenHeight() / 2 + 30,
            30,
            GREEN
        );

        DrawText(
            "Press ESC to return to menu",
            GetScreenWidth() / 2 - 140,
            GetScreenHeight() / 2 + 90,
            20,
            WHITE
        );
    }
}

        EndDrawing();
    }

    // Context resource deallocation safe cleanup routines
    UnloadPlayer(&player);
    UnloadMapData();
    UnloadMenu();
    CloseWindow();
    return 0;
}