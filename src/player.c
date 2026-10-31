#include "raylib.h"
#include "raymath.h"

#include "player.h"

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
	float newX = p->position.x + (direction * PLAYER_SPEED * delta);
	newX = Clamp(newX, 0.0f, GetScreenWidth() - p->width);

	p->position.x = newX;
	DrawRectangle(p->position.x, p->position.y, p->width, 25, GREEN);
}
