#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "projectiles.h"
#include "aliens.h"

#include "resource_dir.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PLAYER_SHOT_SPEED 150.0f
#define ALIEN_MOVE_INTERVAL 0.50f

const float playerSpeed = 200.0f;

Alien aliens[ALIEN_ROWS][ALIEN_COLS];


int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Low Budget Invaders");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Player player;
	player.position = (Vector2){350, 520};
	player.score = 0;
	player.lives = 3;
	player.width = 100;

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
		updatePlayerProjectile(&playerShot, delta);

		if (alienMoveTimer > ALIEN_MOVE_INTERVAL)
		{
			alienMoveTimer = 0.0f;
			
			if (aliensHitBound(aliens))
			{
				shiftAliensDown(aliens);
				alienDirection = -alienDirection;
			}
			moveAliens(aliens, alienDirection);
		}
		

		BeginDrawing();

		ClearBackground(BLACK);

		DrawText(TextFormat("Score: %d", player.score), 350, 10, 20, WHITE);

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
