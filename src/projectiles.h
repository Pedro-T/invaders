#pragma once

#include "raylib.h"

#define PLAYER_SHOT_SPEED 300.0f
#define ALIEN_SHOT_SPEED 80.0f
#define ALIEN_SHOT_CUTOFF 550.0f

typedef struct PlayerProjectile {
    Rectangle rect;
    bool active;
} PlayerProjectile;

typedef struct AlienProjectile {
    Rectangle rect;
    bool active;
    int frame;
} AlienProjectile;

void loadProjectileResources();

void unloadProjectileResources();

void updatePlayerProjectile(PlayerProjectile *p, float delta);

void drawPlayerProjectile(PlayerProjectile p);

bool updateAlienProjectile(AlienProjectile *p, float delta);

void drawAliensProjectiles(AlienProjectile projectiles[], int len);