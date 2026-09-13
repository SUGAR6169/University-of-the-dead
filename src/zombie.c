#include "zombie.h"
#include "player.h"
#include "raylib.h"
#include "map.h"
#include <math.h>
#include <stdlib.h>

Zombie zombies[MAX_ZOMBIES];
int    zombieCount = 0;

void InitZombies(void) {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        zombies[i].active = 0;
        zombies[i].image = LoadTexture("assets/zombie.png");

    }
    zombieCount = 0;
}

void SpawnZombie(void) {
    if (zombieCount >= MAX_ZOMBIES) return;

    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (zombies[i].active == 0) {
            zombies[i].x      = rand() % (MAP_WIDTH  - 100) + 50;
            zombies[i].y      = rand() % (MAP_HEIGHT - 100) + 50;
            zombies[i].w      = 40;
            zombies[i].h      = 40;
            zombies[i].speed  = 120.0f;
            zombies[i].active = 1;
            zombieCount++;
            return;
        }
    }
}

void UpdateZombies(Player *p, float delta) {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (zombies[i].active == 0) continue;

        float dx = (p->x + p->w / 2) - (zombies[i].x + zombies[i].w / 2);
        float dy = (p->y + p->h / 2) - (zombies[i].y + zombies[i].h / 2);
        float distance = sqrtf(dx * dx + dy * dy);

        if (distance > 0) {
            zombies[i].x += (dx / distance) * zombies[i].speed * delta;
            zombies[i].y += (dy / distance) * zombies[i].speed * delta;
        }

        Rectangle zRect = {zombies[i].x, zombies[i].y, 
                           zombies[i].w, zombies[i].h};
        Rectangle pRect = {p->x, p->y, p->w, p->h};

        if (CheckCollisionRecs(zRect, pRect)) {
            p->health = 0;
        }
    }
}

void DrawZombies(void) {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        if (zombies[i].active == 0) continue;

        Rectangle src = {0, 0,
                         (float)zombies[i].image.width,
                         (float)zombies[i].image.height};

        float drawSize = 80.0f;
        float offset = (drawSize - zombies[i].w) / 2.0f;

        Rectangle dst = {
            zombies[i].x - offset,
            zombies[i].y - offset,
            drawSize,
            drawSize
        };

        DrawTexturePro(zombies[i].image, src, dst,
                       (Vector2){0, 0}, 0.0f, WHITE);
    }
}

void UnloadZombies(void) {
    UnloadTexture(zombies[0].image);
}