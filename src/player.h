#pragma once

#include "raylib.h"

#define PLAYER_SPEED 200

typedef struct Player {
	Vector2 position;
	int lives;
	int width;
} Player;

void drawPlayer(Player *p, float delta);