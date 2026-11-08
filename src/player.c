#include "raylib.h"
#include "raymath.h"

#include "player.h"

const float playerStartPosX = 350.0f;
const float playerStartPosY = 520.0f;

const float livesOffsetY = 560.0f;
const float livesOffsetX = 50.0f;

void drawExtraLives(int lives)
{
	int currOffset = livesOffsetX;
	for (int i = 0; i < lives; i++)
	{
		DrawRectangle(currOffset, livesOffsetY, 40.0f, 15.0f, GREEN);
		currOffset += livesOffsetX;
	}
}

void resetPlayerPosition(Player *p)
{
	p->rect = (Rectangle) {
		.x = playerStartPosX,
		.y = playerStartPosY,
		.width = 100.0f,
		.height = 30.0f
	};
}

void drawPlayer(Player *p, float delta)
{
	if (!p)
	{
		return;
	}
	
	int direction = 0;
	if (IsKeyDown(KEY_LEFT))
	{
		direction = -1;
	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		direction = 1;
	}
	float newX = p->rect.x + (direction * PLAYER_SPEED * delta);
	newX = Clamp(newX, 0.0f, GetScreenWidth() - p->rect.width);

	p->rect.x = newX;
	DrawRectangle(p->rect.x, p->rect.y, p->rect.width, 25, GREEN);
}
