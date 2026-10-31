#pragma once

#include "raylib.h"

#include "player.h"

typedef struct PlayerProjectile {
    Vector2 position;
    bool active;
} PlayerProjectile;

typedef struct EnemyProjectile {
    Vector2 position;
} EnemyProjectile;

void updatePlayerProjectile(PlayerProjectile *p, float delta);

void drawPlayerProjectile(PlayerProjectile p);
