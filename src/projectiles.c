#include "raylib.h"
#include "raymath.h"

#include "projectiles.h"

#define PLAYER_SHOT_SPEED 150.0f

void updatePlayerProjectile(PlayerProjectile *p, float delta)
{
	if (p->position.y < 0)
	{
		p->position.x = -1.0f;
		p->position.y = -1.0f;
        p->active = false;
		return;
	}
	float newY = p->position.y - (PLAYER_SHOT_SPEED * delta);
	p->position.y = newY;
}

void drawPlayerProjectile(PlayerProjectile p)
{
	if (!p.active)
	{
		return;
	}
	DrawLineV(p.position, Vector2Add(p.position, (Vector2){0.0f, 10.0f}), WHITE);
}