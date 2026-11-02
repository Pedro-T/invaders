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

const float playerSpeed = 200.0f;

int score = 0;

Alien aliens[ALIEN_ROWS][ALIEN_COLS];

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
				if (checkHitPlayerProjectile(*p, *a))
				{
					a->alive = false;
					p->active = false;
					score += a->value;
				}
			}
		}
	}
}

void gameOver()
{
	printf("Game Over!\n");
}


int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Low Budget Invaders");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Player player = (Player) {
		.position = (Vector2){350, 520},
		.lives = 3,
		.width = 100
	};

	PlayerProjectile playerShot = (PlayerProjectile){(Vector2){-1, -1}, false};

	int alienDirection = 1;
	float alienMoveTimer = 0.0f;
	initAliens(aliens);

	
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float delta = GetFrameTime();
		alienMoveTimer += delta;

		if (IsKeyDown(KEY_SPACE) && !playerShot.active)
		{
			playerShot.position = Vector2Add(player.position, (Vector2){player.width / 2.0f, 0.0f});
			playerShot.active = true;
		}

		processPlayerShotUpdate(&playerShot, aliens, delta);
		

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
		

		BeginDrawing();

		ClearBackground(BLACK);

		DrawText(TextFormat("Score: %d", score), 350, 10, 20, WHITE);

		DrawLineV((Vector2){0, 550}, (Vector2){WINDOW_WIDTH, 550}, GREEN);


		
		drawPlayerProjectile(playerShot);
		drawPlayer(&player, delta);
		drawAliens(aliens);

		EndDrawing();
	}


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
