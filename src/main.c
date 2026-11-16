#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "projectiles.h"
#include "aliens.h"
#include "title.h"

#include <stdio.h>

#include "resource_dir.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define ALIEN_MOVE_INTERVAL 0.70f
#define ALIEN_SHOTS_POOL_SIZE 10
#define ALIEN_ATTACK_INTERVAL 0.80f

int score = 0;
float alienAttackTimer = 0.0f;
int alienDirection = 1;
float alienMoveTimer = 0.0f;

Alien aliens[ALIEN_ROWS][ALIEN_COLS];
AlienProjectile alienShots[ALIEN_SHOTS_POOL_SIZE];

Player player;
PlayerProjectile playerShot;

typedef enum GameState {
	TITLE,
	PLAYING,
	GAME_OVER
} GameState;

void initPlayer()
{
	player = (Player) {
		.lives = 3,
		.state = ALIVE
	};
	playerShot = (PlayerProjectile){
		.rect = (Rectangle) {
			.x = -1,
			.y = -1,
			.width = 5.0f,
			.height = 16.0f
		},
		.active = false
	};
}

void initAlienShots()
{
	for (int i = 0; i < ALIEN_SHOTS_POOL_SIZE; i++)
	{
		alienShots[i].active = false;
	}
}

void playerDeath()
{
	initAlienShots();
	player.lives--;

	// todo player explosion animation

	resetPlayerPosition(&player);

	if (player.lives <= 0)
	{
		// todo show game over screen
	}
}

void processAlienProjectiles(float delta)
{
	for (int i = 0; i < ALIEN_SHOTS_POOL_SIZE; i++)
	{
		if (!alienShots[i].active)
		{
			continue;
		}
		bool destroyed = updateAlienProjectile(&alienShots[i], delta);
		if (!destroyed)
		{
			bool playerHit = CheckCollisionRecs(alienShots[i].rect, player.rect);
			if (playerHit)
			{
				destroyed = true;
				playerDeath();
			}
		}
		if (destroyed)
		{
			alienShots[i].active = false;
		}
	}
}

void processPlayerShotUpdate(PlayerProjectile *p, Alien aliens[ALIEN_ROWS][ALIEN_COLS], float delta)
{
	updatePlayerProjectile(p, delta);
	if (p->active)
	{
		for (int i = 0; i < ALIEN_ROWS; i++)
		{
			for (int j = 0; j < ALIEN_COLS; j++)
			{
				Alien *a = &aliens[i][j];
				if (!a->alive)
				{
					continue;
				}
				if (CheckCollisionRecs(p->rect, a->rect))
				{
					a->alive = false;
					p->active = false;
					score += a->value;
				}
			}
		}
	}
}

void aliensAttack(float delta)
{
	alienAttackTimer += delta;
	if (alienAttackTimer >= ALIEN_ATTACK_INTERVAL)
	{
		AlienProjectile *p = NULL;
		for (int i = 0; i < ALIEN_SHOTS_POOL_SIZE; i++)
		{
			if (!alienShots[i].active)
			{
				p = &alienShots[i];
				break;
			}
		}
		if (p == NULL)
		{
			return;
		}
		aliensFireShot(aliens, p);
		alienAttackTimer = 0.0f;
	}
}

void gameOver()
{
	printf("Game Over!\n");
}

void loadResources()
{
	loadPlayerResources();
	loadAlienResources();
	loadProjectileResources();
}

void unloadResources()
{
	unloadAlienResources();
	unloadPlayerResources();
	unloadProjectileResources();
}

void processGameTick(float delta)
{
	alienMoveTimer += delta;

	// player updates
	if (IsKeyDown(KEY_SPACE) && !playerShot.active)
	{
		playerShot.rect = (Rectangle){
			.x = player.rect.x + (player.rect.width / 2.0f), 
			.y = player.rect.y, 
			.width = 5.0f, 
			.height = 16.0f
		};
		playerShot.active = true;
	}
	processPlayerShotUpdate(&playerShot, aliens, delta);
	

	// alien updates
	if (alienMoveTimer > ALIEN_MOVE_INTERVAL)
	{
		alienMoveTimer = 0.0f;
		
		if (aliensHitBound(aliens))
		{
			bool bottomReached = shiftAliensDown(aliens);
			if (bottomReached)
			{
				gameOver();
			}
			alienDirection = -alienDirection;
		}
		moveAliens(aliens, alienDirection);
	}
	aliensAttack(delta);
	processAlienProjectiles(delta);
	incrementAlienProjFrameTime(delta);
	

	BeginDrawing();

	// core UI
	ClearBackground(BLACK);
	DrawText(TextFormat("Score: %d", score), 350, 10, 20, WHITE);
	DrawLineV((Vector2){0, 550}, (Vector2){WINDOW_WIDTH, 550}, GREEN); // line under player above lives
	drawExtraLives(player.lives);

	// player
	drawPlayerProjectile(playerShot);
	drawPlayer(&player, delta);

	// aliens
	drawAliens(aliens);
	drawAliensProjectiles(alienShots, ALIEN_SHOTS_POOL_SIZE);

	EndDrawing();
}

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Low Budget Invaders");
	SearchAndSetResourceDir("resources");

	loadResources();

	GameState state = TITLE;

	
	initAlienShots();
	initAliens(aliens);
	initPlayer();
	resetPlayerPosition(&player);
	initTitle();
	
	while (!WindowShouldClose())
	{
		float delta = GetFrameTime();
		switch (state)
		{
			case TITLE:
				drawTitle(delta);
				if (IsKeyPressed(KEY_SPACE))
				{
					state = PLAYING;
					// need to re-fire inits here, or do it at game over
				}
				break;
			case PLAYING:
				processGameTick(delta);
				break;
			case GAME_OVER:
				if (IsKeyPressed(KEY_SPACE))
				{
					state = TITLE;
				}
				break;
		}
		
	}

	unloadResources();

	CloseWindow();
	return 0;
}
