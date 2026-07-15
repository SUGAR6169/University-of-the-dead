#include <stdio.h>    // Required for sprintf (Debug text)
#include "raylib.h"
#include "map.h"
#include "player.h"
#include "timer.h"
#include "hud.h"
#include "menu.h"

#define SCREEN_W 1920
#define SCREEN_H 1280

// Game states
typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAMEOVER,
    STATE_WIN,
} GameState;

GameState gameState = STATE_MENU;

// Global debug toggle
int debugMode = 0;

int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "University of the Dead");
    SetTargetFPS(60); // 60 FPS provides stable physics processing for bounding boxes

    // Declare core game objects
    Player player;
    Timer timer;

    // Load assets and initialize game objects
    LoadMapData();
    InitPlayer(&player);  
    InitTimer(&timer, 30.0f); // Set to 30 seconds initial countdown

    // Set up Camera2D — this follows the player
    Camera2D camera = {0};
    camera.zoom   = 0.25f;  // Zoomed out to see the map layout on the menu
    camera.offset = (Vector2){SCREEN_W / 2.0f, SCREEN_H / 2.0f};
    camera.target = (Vector2){MAP_WIDTH / 2.0f, MAP_HEIGHT / 2.0f}; // Centers the map on the menu

    // Player starts at Main Gate position on the map
    player.x = 3000.0f;
    player.y = 2200.0f;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        float time  = GetTime();

        // ── GLOBAL HOTKEYS ──────────────────────────────────
        // Toggle fullscreen display resolution parameters dynamically
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        // ── UPDATE SYSTEM ──────────────────────────────────
        switch (gameState) {
            case STATE_MENU:
                if (IsKeyPressed(KEY_ENTER)) {
                    gameState = STATE_PLAYING;
                    camera.zoom = 0.65f; // Zoom in tightly on the player for active gameplay
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
                float minX = SCREEN_W / (2.0f * camera.zoom);
                float maxX = MAP_WIDTH - (SCREEN_W / (2.0f * camera.zoom));
                float minY = SCREEN_H / (2.0f * camera.zoom);
                float maxY = MAP_HEIGHT - (SCREEN_H / (2.0f * camera.zoom));

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
                
                // Keep user zoom scale configurations within safe limits
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
                if (IsKeyPressed(KEY_ESCAPE)) gameState = STATE_PAUSED;
                break;
            }

            case STATE_PAUSED:
                if (IsKeyPressed(KEY_ESCAPE)) gameState = STATE_PLAYING;
                if (IsKeyPressed(KEY_Q))      gameState = STATE_MENU;
                break;

            // ── GAME OVER SCREEN ──
            case STATE_GAMEOVER: {
                int fontSize = 80;
                const char* text = "GAME OVER";
                int textWidth = MeasureText(text, fontSize);
                
                // Draw centered on screen
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RED, 0.6f));
                DrawText(text, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2 - 40, fontSize, WHITE);
                break;
            }

            // ── WIN SCREEN ──
            case STATE_WIN: {
                int fontSize = 80;
                const char* text = "YOU WIN!";
                int textWidth = MeasureText(text, fontSize);
                
                // Draw centered on screen
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GREEN, 0.6f));
                DrawText(text, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2 - 40, fontSize, WHITE);
                break;
            }
        }

        // ── DRAW PIPELINE ───────────────────────────────────
        BeginDrawing();
        ClearBackground(BLACK);

        // 1. WORLD SPACE SYSTEM (Camera Transformations Applied)
        BeginMode2D(camera);
            DrawMapBackground(camera); // Render campus graphic background asset
            DrawZoneHighlights(time);  // Draw active neon interactive zones overlays
            
            if (gameState == STATE_PLAYING || gameState == STATE_PAUSED) {
                DrawPlayer(&player);
            }

            // Debug Geometry Rendering Mode
            if (debugMode) {
                for (int i = 0; i < wallCount; i++) {
                    DrawRectangleLinesEx(walls[i], 3, RED);
                }
                for (int i = 0; i < zoneCount; i++) {
                    DrawRectangleLinesEx(zones[i].rect, 2, BLUE);
                }
            }
        EndMode2D();

        // 2. SCREEN SPACE SYSTEM (Static UI Text Overlay)
        if (debugMode) {
            char posText[64];
            sprintf(posText, "POS: %.0f, %.0f | ZOOM: %.2f", player.x, player.y, camera.zoom);
            DrawText(posText, 10, SCREEN_H - 30, 16, YELLOW);
        }

        // Route render pipeline execution based on current structural game state
        if (gameState == STATE_PLAYING)  DrawHUD(&player, &timer);
        if (gameState == STATE_PAUSED)   DrawPauseMenu();
        if (gameState == STATE_MENU)     DrawMainMenu();
        if (gameState == STATE_GAMEOVER) DrawGameOver();
        if (gameState == STATE_WIN)      DrawWinScreen();

        EndDrawing();
    }

    // Context resource deallocation safe cleanup routines
    UnloadMapData();
    CloseWindow();
    return 0;
}