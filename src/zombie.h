#ifndef ZOMBIE_H
#define ZOMBIE_H
#include "raylib.h"
#include "player.h"
#define MAX_ZOMBIES 10
typedef struct {
    float x,y;
    float w,h;
    float speed;
    int active;
    //active=1 means zombie exists on the map and chases the player
    //active=0 means zombie is inactive
    Texture2D image;
} Zombie;

extern Zombie zombies[MAX_ZOMBIES];
extern int zombieCount;

void InitZombies(void);
void SpawnZombie(void);
void UpdateZombies(Player*p,float delta);
//UpdateZombies takes Player *p to know where the player is to chase them and to check if a zombie touched them.
void DrawZombies(void);
void UnloadZombies(void);
//draws every active zombie on screen

#endif