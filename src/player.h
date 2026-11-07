#pragma once

#include "raylib.h"

#define PLAYER_SPEED 200

typedef struct Player {
	Rectangle rect;
	int lives;
} Player;

void drawPlayer(Player *p, float delta);