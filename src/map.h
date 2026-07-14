#ifndef MAP_H
#define MAP_H

#include "raylib.h"

// The real map is 3840x2560 pixels (your AI image)
#define MAP_WIDTH   3840
#define MAP_HEIGHT  2560

// How many collision zones (buildings + walls)
#define MAX_WALLS   40

// How many special interaction zones
#define MAX_ZONES   15

// Zone types
typedef enum {
    ZONE_MOSQUE,
    ZONE_CLASSROOM,
    ZONE_DORM,
    ZONE_CAFETERIA,
    ZONE_AUDITORIUM,
    ZONE_LAB,
    ZONE_ADMIN,
    ZONE_MEDICAL,
    ZONE_FIELD,
    ZONE_MAIN_GATE,
    ZONE_EXIT,
} ZoneType;

// A special zone the player can enter
typedef struct {
    Rectangle rect;      // pixel area on the map
    ZoneType  type;
    char      label[32]; // display name
    int       active;
} Zone;

// Exported data
extern Rectangle walls[MAX_WALLS];
extern int       wallCount;
extern Zone      zones[MAX_ZONES];
extern int       zoneCount;

// Functions
void LoadMapData(void);
void DrawMapBackground(Camera2D cam);
void DrawZoneHighlights(float time);
void UnloadMapData(void);
int  IsWallBlocked(Rectangle playerRect);
Zone* GetCurrentZone(Rectangle playerRect);

#endif