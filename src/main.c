#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "projectiles.h"
#include "aliens.h"

#include <stdio.h>

#include "resource_dir.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PLAYER_SHOT_SPEED 150.0f
#define ALIEN_MOVE_INTERVAL 0.50f
#define ALIEN_SHOTS_POOL_SIZE 10
#define ALIEN_ATTACK_INTERVAL 0.80f

int score = 0;
float alienAttackTimer = 0.0f;

Alien aliens[ALIEN_ROWS][ALIEN_COLS];
AlienProjectile alienShots[ALIEN_SHOTS_POOL_SIZE];
Player player;

void initAlienShots()
{
	for (int i = 0; i < ALIEN_SHOTS_POOL_SIZE; i++)
	{
		alienShots[i].active = false;
	}
}

void playerDeath()
{
	player.lives--;
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


int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Low Budget Invaders");
	SearchAndSetResourceDir("resources");

	initPlayerRendering();

	player = (Player) {
		.lives = 3,
		.state = ALIVE
	};
	resetPlayerPosition(&player);
	

	PlayerProjectile playerShot = (PlayerProjectile){
		.rect = (Rectangle) {
			.x = -1,
			.y = -1,
			.width = 3.0f,
			.height = 6.0f
		},
		.active = false
	};

	int alienDirection = 1;
	float alienMoveTimer = 0.0f;
	initAlienShots();
	initAliens(aliens);

	
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float delta = GetFrameTime();
		alienMoveTimer += delta;

		// player updates
		if (IsKeyDown(KEY_SPACE) && !playerShot.active)
		{
			playerShot.rect = (Rectangle){
				.x = player.rect.x + (player.rect.width / 2.0f), 
				.y = player.rect.y, 
				.width = 3.0f, 
				.height = 6.0f
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


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
