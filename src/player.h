#ifndef PLAYER_H
#define PLAYER_H

// Player structure
typedef struct {
    float x, y;           // Position on the map in pixels
    float w, h;           // Dimensions for collision detection (MUST be float for Raylib Rectangles)
    float speed;          // Normal walking speed
    float stamina;        // Stamina value 0.0f-100.0f
    int   isRunning;      // Boolean: 1 if sprinting, 0 if walking
    int   health;         // Player health state (1 = alive, 0 = dead)
} Player;

// Make the global player instance visible to main.c and other files
extern Player player;

// Sets up initial coordinates, speed, and default stats
void InitPlayer(Player *p);

// Processes WASD input, adjusts coordinates, and calculates delta-time movement
void UpdatePlayer(Player *p, float delta);

// Render player character visual placeholder
void DrawPlayer(Player *p);

#endif // PLAYER_H