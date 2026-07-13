#ifndef PLAYER_H
#define PLAYER_H

//Player structure
typedef struct {
    float x, y;           // Position on the screen in pixels
    float speed;          // Normal walking speed
    float stamina;        // Value from 0.0f to 100.0f
    int   isRunning;      // Boolean: 1 if sprinting, 0 if walking
    int   width, height;  // Dimensions of the bounding box for collision detection
    int   health;         // Player health state (1 = alive, 0 = dead)
    int   hasMosqueKey;   // Inventory item flag: 0 or 1 for the locked door puzzle
} Player;

// Sets up initial coordinates, speed, and default stats
void InitPlayer(Player *p);

// Processes WASD input, adjusts coordinates, and calculates delta-time movement
void UpdatePlayer(Player *p, float delta);

// Renders the player character visual placeholder onto the screen
void DrawPlayer(Player *p);

#endif // PLAYER_H