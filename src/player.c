#include "player.h"
#include "raylib.h"

//Initialize the player's starting attributes
void InitPlayer(Player *p) {
    
    //This will later be dynamically set based on the map's spawn tile
    p->x = 200.0f;
    p->y = 200.0f;
    
    // box dimensions
    p->width = 24;
    p->height = 24;
    
    p->speed = 150.0f; // Movement (pixels per second)
    
    // State management
    p->stamina = 100.0f;
    p->isRunning = 0;
    p->health = 1;
    p->hasMosqueKey = 0;
}

// For Handling real-time keyboard inputs and position recalculation
void UpdatePlayer(Player *p, float delta)
{
    if (IsKeyDown(KEY_W))
        p->y -= p->speed * delta;

    if (IsKeyDown(KEY_S))
        p->y += p->speed * delta;

    if (IsKeyDown(KEY_A))
        p->x -= p->speed * delta;

    if (IsKeyDown(KEY_D))
        p->x += p->speed * delta;


    // Screen boundaries
    if(p->x < 0)
        p->x = 0;

    if(p->y < 0)
        p->y = 0;

    if(p->x > 800 - p->width)
        p->x = 800 - p->width;

    if(p->y > 600 - p->height)
        p->y = 600 - p->height;
}

// Render the player character onto the current frame
void DrawPlayer(Player *p) {
    
    // Green Square for the presentation prototype
    DrawRectangle((int)p->x, (int)p->y, p->width, p->height, GREEN);
}