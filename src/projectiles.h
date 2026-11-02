#pragma once

#include "raylib.h"

#include "player.h"
#include "aliens.h"

typedef struct PlayerProjectile {
    Vector2 position;
    bool active;
} PlayerProjectile;

typedef struct EnemyProjectile {
    Vector2 position;
} EnemyProjectile;

void updatePlayerProjectile(PlayerProjectile *p, float delta);

bool checkHitPlayerProjectile(PlayerProjectile p, Alien alien);

void drawPlayerProjectile(PlayerProjectile p);
