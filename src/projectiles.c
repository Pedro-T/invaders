#include "raylib.h"
#include "raymath.h"

#include "projectiles.h"

Texture2D projTexAlien;
Texture2D projTexPlayer;

const float playerProjectileWidth = 4.0f;
const float playerProjectileHeight = 8.0f;
const int frameLimit = 1;

// all alien shots use the same central frame counter for simplicity
const float alienProjectileFrameTime = 0.10f;
float alienProjectileFrameCurrentTime = 0.0f;
int alienProjectileCurrentFrame = 0;
const int alienProjectileFrameLimit = 1;

void loadProjectileResources()
{
	projTexAlien = LoadTexture("projectileAlien.png");
}

void unloadProjectileResources()
{
	UnloadTexture(projTexAlien);
}

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
	DrawRectangle(p.rect.x - p.rect.width / 2.0f, p.rect.y, p.rect.width, p.rect.height, WHITE);
}

void incrementAlienProjFrameTime(float delta)
{
	alienProjectileFrameCurrentTime += delta;
	if (alienProjectileFrameCurrentTime >= alienProjectileFrameTime)
	{
		alienProjectileCurrentFrame++;
		if (alienProjectileCurrentFrame > alienProjectileFrameLimit)
		{
			alienProjectileCurrentFrame = 0;
		}
		alienProjectileFrameCurrentTime -= alienProjectileFrameTime;
	}
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
		DrawTextureRec(
			projTexAlien, 
			(Rectangle){
				.x = alienProjectileCurrentFrame * 10.0f,
				.y = 0.0f,
				.width = p.rect.width,
				.height = p.rect.height
			},
			(Vector2){
				.x = p.rect.x - p.rect.width / 2.0f,
				.y = p.rect.y - p.rect.height / 2.0f
			},
			WHITE
		);
	}
}