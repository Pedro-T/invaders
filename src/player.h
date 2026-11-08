#pragma once

#include "raylib.h"

#define PLAYER_SPEED 200

typedef enum {
	ALIVE,
	DYING,
	DEAD
} PlayerState;

typedef struct Player {
	Rectangle rect;
	int lives;
	PlayerState state;
} Player;

void initPlayerRendering();

void drawExtraLives(int lives);

void drawPlayer(Player *p, float delta);

void resetPlayerPosition(Player *p);