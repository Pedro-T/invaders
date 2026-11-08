#include "raylib.h"
#include "raymath.h"

#include "projectiles.h"

void updatePlayerProjectile(PlayerProjectile *p, float delta)
{
	if (p->rect.y < 0)
	{
		p->rect.x = -1.0f;
		p->rect.y = -1.0f;
        p->active = false;
		return;
	}
	float newY = p->rect.y - (PLAYER_SHOT_SPEED * delta);
	p->rect.y = newY;
}

void drawPlayerProjectile(PlayerProjectile p)
{
	if (!p.active)
	{
		return;
	}
	DrawLine(p.rect.x, p.rect.y, p.rect.x, p.rect.y + 10.0f, WHITE);
}

bool updateAlienProjectile(AlienProjectile *p, float delta)
{
	float newPos = p->rect.y + ALIEN_SHOT_SPEED * delta;
	if (newPos >= ALIEN_SHOT_CUTOFF)
	{
		return true;
	}
	p->rect.y = newPos;
	return false;
}

void drawAliensProjectiles(AlienProjectile projectiles[], int len)
{
	for (int i = 0; i < len; i++)
	{
		AlienProjectile p = projectiles[i];
		if (!p.active)
		{
			continue;
		}
		DrawLine(p.rect.x, p.rect.y, p.rect.x, p.rect.y + 8, RED);
	}
}