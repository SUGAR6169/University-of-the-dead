#include "player.h"
#include "raylib.h"
#include "map.h"

// Global player instance
Player player;

//Initialize the player's starting attributes
void InitPlayer(Player *p) {
    p->x = 2400.0f;
    p->y = 1950.0f;
    
    // dimensions
    p->w = 40;        
    p->h = 40;
    
    // stats
    p->speed = 300.0f;    
    p->stamina = 100.0f;
    p->isRunning = 0;
    p->health = 1;
}

// Handling keyboard inputs, stamina, and map collisions
void UpdatePlayer(Player *p, float delta) {
    float spd = p->speed;
    p->isRunning = 0; // Reset running state each frame

    // sprint
    if (IsKeyDown(KEY_LEFT_SHIFT) && p->stamina > 0) {
        spd *= 1.8f;
        p->stamina -= 30.0f * delta;
        p->isRunning = 1; // Update state for animation/HUD purposes
        if (p->stamina < 0) p->stamina = 0;
    } else {
        p->stamina += 15.0f * delta;
        if (p->stamina > 100.0f) p->stamina = 100.0f;
    }
    
    // Exhaustion penalty
    if (p->stamina <= 0) spd *= 0.5f;

    float nx = p->x;
    float ny = p->y;

    // Movement inputs ,WASD - Arrow keys
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    ny -= spd * delta;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  ny += spd * delta;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  nx -= spd * delta;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) nx += spd * delta;

    // Collision check independently for X and Y axes
    // slide along walls
    Rectangle newRectX = {nx, p->y, p->w, p->h};
    Rectangle newRectY = {p->x, ny, p->w, p->h};

    if (!IsWallBlocked(newRectX)) p->x = nx;
    if (!IsWallBlocked(newRectY)) p->y = ny;

    // Keep player inside absolute map bounds
    if (p->x < 0) p->x = 0;
    if (p->y < 0) p->y = 0;
    if (p->x > MAP_WIDTH  - p->w) p->x = MAP_WIDTH  - p->w;
    if (p->y > MAP_HEIGHT - p->h) p->y = MAP_HEIGHT - p->h;
}

// player character on screen
void DrawPlayer(Player *p) {
    DrawCircle(
        (int)(p->x + p->w / 2),
        (int)(p->y + p->h / 2),
        p->w / 2, 
        GREEN
    );
    
    // Direction indicator yellow dot
    DrawCircle(
        (int)(p->x + p->w / 2),
        (int)(p->y + p->h / 2 - p->h / 3),
        5, 
        YELLOW
    );
}