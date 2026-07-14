#include "raylib.h"
#include "map.h"
#include <string.h>
#include <math.h>

// The big map texture
static Texture2D mapTexture;

// Collision walls — rectangles over every building
// Format: {x, y, width, height} in MAP pixel coordinates
// These match the labeled buildings in your AI map image
Rectangle walls[MAX_WALLS];
int       wallCount = 0;

// Special interactive zones
Zone zones[MAX_ZONES];
int  zoneCount = 0;

// Helper to add a wall rectangle
static void AddWall(float x, float y, float w, float h) {
    if (wallCount < MAX_WALLS) {
        walls[wallCount++] = (Rectangle){x, y, w, h};
    }
}

// Helper to add an interactive zone
static void AddZone(float x, float y, float w, float h,
                    ZoneType type, const char *label) {
    if (zoneCount < MAX_ZONES) {
        zones[zoneCount].rect   = (Rectangle){x, y, w, h};
        zones[zoneCount].type   = type;
        zones[zoneCount].active = 1;
        strncpy(zones[zoneCount].label, label, 31);
        zoneCount++;
    }
}

void LoadMapData(void) {
    // Load the map image as a texture
    mapTexture = LoadTexture("assets/map.png");

    // ── COLLISION WALLS ──────────────────────────────────────
    // These pixel coordinates match the buildings in your map.
    // You will need to fine-tune these numbers by testing in game.
    // Method: run the game, walk to a building edge, print player
    // position, use that as the rectangle boundary.

    // Outer boundary walls (campus perimeter)
    AddWall(0,    0,    MAP_WIDTH, 40);   // top border
    AddWall(0,    MAP_HEIGHT-40, MAP_WIDTH, 40); // bottom border
    AddWall(0,    0,    40, MAP_HEIGHT);  // left border
    AddWall(MAP_WIDTH-40, 0, 40, MAP_HEIGHT);    // right border

    // Academic Building 1 (large central building)
    AddWall(980,  580, 680, 180);

    // Academic Building 2 (top center)
    AddWall(1300, 130, 680, 180);

    // Academic Building 3 (top left area)
    AddWall(540,  130, 680, 180);

    // Academic Building 3 duplicate (second block)
    AddWall(330,  360, 460, 140);

    // North Hall (top right dorm)
    AddWall(2160, 130, 480, 160);

    // South Hall (right dorm)
    AddWall(2200, 520, 480, 160);

    // Lab 1
    AddWall(350,  490, 380, 140);

    // Lab 2
    AddWall(350,  700, 380, 140);

    // Administration Building
    AddWall(870,  720, 440, 200);

    // Cafeteria + Library
    AddWall(1380, 720, 440, 200);

    // CDS Building
    AddWall(2180, 730, 320, 180);

    // IUT Auditorium
    AddWall(820,  1000, 440, 200);

    // Female Hall of Residence (bottom left)
    AddWall(90,   1700, 420, 360);

    // ── INTERACTIVE ZONES ────────────────────────────────────
    // These are areas the player ENTERS — not walls

    // Mosque interior (timer freezes here)
    AddZone(1310, 1160, 840, 560, ZONE_MOSQUE,    "Mosque");

    // Academic buildings (collectibles)
    AddZone(980,  580,  680, 180, ZONE_CLASSROOM, "Academic Building 1");
    AddZone(1300, 130,  680, 180, ZONE_CLASSROOM, "Academic Building 2");
    AddZone(540,  130,  680, 180, ZONE_CLASSROOM, "Academic Building 3");

    // Labs
    AddZone(350,  490,  380, 140, ZONE_LAB,       "Lab 1");
    AddZone(350,  700,  380, 140, ZONE_LAB,       "Lab 2");

    // Cafeteria
    AddZone(1380, 720,  440, 200, ZONE_CAFETERIA, "Cafeteria + Library");

    // Auditorium
    AddZone(820,  1000, 440, 200, ZONE_AUDITORIUM,"IUT Auditorium");

    // Dorms
    AddZone(2160, 130,  480, 160, ZONE_DORM,      "North Hall");
    AddZone(2200, 520,  480, 160, ZONE_DORM,      "South Hall");
    AddZone(90,   1700, 420, 360, ZONE_DORM,      "Female Hall");

    // Main Gate (spawn area)
    AddZone(2300, 1900, 300, 200, ZONE_MAIN_GATE, "Main Gate");

    // Central Field (open play area)
    AddZone(2060, 940,  740, 860, ZONE_FIELD,     "Central Field");

    // Medical
    AddZone(90,   130,  320, 160, ZONE_MEDICAL,   "Medical Center");

// Exit (Top left corner near Medical Center)
AddZone(50, 50, 200, 200, ZONE_EXIT, "Top Left Exit");
}

void DrawMapBackground(Camera2D cam) {
    if (mapTexture.width > 0) {
        Rectangle src = {0, 0, 
                         (float)mapTexture.width, 
                         (float)mapTexture.height};
        Rectangle dst = {0, 0, 
                         (float)MAP_WIDTH, 
                         (float)MAP_HEIGHT};
        DrawTexturePro(mapTexture, src, dst, 
                       (Vector2){0,0}, 0.0f, WHITE);
    }
}

void DrawZoneHighlights(float time) {
    // Draw subtle glowing highlights over interactive zones
    for (int i = 0; i < zoneCount; i++) {
        Zone *z = &zones[i];
        Color col = {0, 0, 0, 0};
        float pulse = (float)(0.15 + 0.08 * sinf(time * 2.5f));

        switch (z->type) {
            case ZONE_MOSQUE:
                col = (Color){40, 220, 100, (unsigned char)(pulse * 255)};
                break;
            case ZONE_CLASSROOM:
            case ZONE_LAB:
                col = (Color){60, 120, 255, (unsigned char)(pulse * 180)};
                break;
            case ZONE_DORM:
                col = (Color){180, 80, 220, (unsigned char)(pulse * 150)};
                break;
            case ZONE_EXIT:
                col = (Color){255, 215, 0,
                    (unsigned char)((0.3f + 0.3f * sinf(time * 4)) * 255)};
                break;
            case ZONE_CAFETERIA:
                col = (Color){255, 140, 30, (unsigned char)(pulse * 160)};
                break;
            default:
                col = (Color){200, 200, 200, (unsigned char)(pulse * 100)};
                break;
        }
        DrawRectangleRec(z->rect, col);
    }
}

void UnloadMapData(void) {
    UnloadTexture(mapTexture);
}

int IsWallBlocked(Rectangle playerRect) {
    for (int i = 0; i < wallCount; i++) {
        if (CheckCollisionRecs(playerRect, walls[i])) return 1;
    }
    return 0;
}

Zone* GetCurrentZone(Rectangle playerRect) {
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i].active &&
            CheckCollisionRecs(playerRect, zones[i].rect)) {
            return &zones[i];
        }
    }
    return NULL;
}