#pragma once

#include "raylib.h"
#include "projectiles.h"

#define BUNKER_COUNT 4
#define BRICK_ROWS 3
#define BRICK_COLS 6

typedef struct Bunker {
    Vector2 pos;
    int grid[BRICK_ROWS][BRICK_COLS];
} Bunker;

void initBunkers();
void drawBunkers();
bool checkBunkerHits(AlienProjectile *p);
void loadBunkerResources();
void unloadBunkerResources();
