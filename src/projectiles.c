#include "raylib.h"
#include "raymath.h"

#include "projectiles.h"

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

bool checkHitPlayerProjectile(PlayerProjectile p, Rectangle r)
{

	// todo: projectile will not always be a single point
	return !(p.position.x > r.x + r.width ||
		p.position.x < r.x || 
		p.position.y > r.y + r.height ||
		p.position.y < r.y
	);
}

void drawPlayerProjectile(PlayerProjectile p)
{
	if (!p.active)
	{
		return;
	}
	DrawLineV(p.position, Vector2Add(p.position, (Vector2){0.0f, 10.0f}), WHITE);
}

bool updateAlienProjectile(AlienProjectile *p, float delta)
{
	float newPos = p->position.y + ALIEN_SHOT_SPEED * delta;
	if (newPos >= ALIEN_SHOT_CUTOFF)
	{
		return true;
	}
	p->position.y = newPos;
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
		DrawLine(p.position.x, p.position.y, p.position.x, p.position.y + 8, RED);
	}
}