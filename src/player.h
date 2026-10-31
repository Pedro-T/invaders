#pragma once

#define PLAYER_SPEED 200

typedef struct player {
	Vector2 position;
	int score;
	int lives;
	int width;
} Player;

void drawPlayer(Player *p, float delta);