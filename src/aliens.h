#pragma once

#include "raylib.h"

#define ALIEN_ROWS 6
#define ALIEN_COLS 8

typedef struct Alien {
    Vector2 position;
    int width;
    int height;
} Alien;

void initAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);

void updateAlien(Alien *a);

void drawAlien(Alien a);