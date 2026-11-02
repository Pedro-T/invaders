#pragma once

#include "raylib.h"

#define ALIEN_ROWS 6
#define ALIEN_COLS 8
#define ALIEN_BLOCK_OFFSET_Y 32.0f
#define ALIEN_CELL_SIZE 64.0f
#define ALIEN_BLOCK_OFFSET_X (GetScreenWidth() - ALIEN_CELL_SIZE * ALIEN_COLS) / 2
#define ALIEN_MOVE_AMOUNT 20.0f
#define BOUND_OFFSET 40.0f
#define SHIFT_DOWN_AMOUNT 32.0f
#define ALIEN_GOAL_Y 520.0f

typedef struct Alien {
    Vector2 position;
    int width;
    int height;
    bool alive;
    int value;
} Alien;

void initAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);

void moveAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS], int direction);

void drawAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);

bool aliensHitBound(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);

bool shiftAliensDown(Alien aliens[ALIEN_ROWS][ALIEN_COLS]);