#pragma once

#include "raylib.h"

#define PLAYER_SHOT_SPEED 150.0f
#define ALIEN_SHOT_SPEED 150.0f
#define ALIEN_SHOT_CUTOFF 550.0f

typedef struct PlayerProjectile {
    Vector2 position;
    bool active;
} PlayerProjectile;

typedef struct AlienProjectile {
    Vector2 position;
    bool active;
} AlienProjectile;

void updatePlayerProjectile(PlayerProjectile *p, float delta);

bool checkHitPlayerProjectile(PlayerProjectile p, Rectangle r);

void drawPlayerProjectile(PlayerProjectile p);

bool updateAlienProjectile(AlienProjectile *p, float delta);