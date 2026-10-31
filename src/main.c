#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "projectiles.h"

#include "resource_dir.h"

#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PLAYER_SHOT_SPEED 150.0f

const float playerSpeed = 200.0f;



void drawPlayer(Player *p, float delta)
{
	if (p == NULL)
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
	float newX = p->position.x + (direction * playerSpeed * delta);
	newX = Clamp(newX, 0.0f, WINDOW_WIDTH - p->width);

	p->position.x = newX;
	DrawRectangle(p->position.x, p->position.y, p->width, 25, GREEN);
}


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

	
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float delta = GetFrameTime();

		if (IsKeyDown(KEY_SPACE) && !playerShot.active)
		{
			playerShot.position = Vector2Add(player.position, (Vector2){player.width / 2.0f, 0.0f});
			playerShot.active = true;
		}
		updatePlayerProjectile(&playerShot, delta);

		BeginDrawing();

		ClearBackground(BLACK);

		DrawText(TextFormat("Score: %d", player.score), 350, 10, 20, WHITE);

		DrawLineV((Vector2){0, 550}, (Vector2){WINDOW_WIDTH, 550}, GREEN);


		
		drawPlayerProjectile(playerShot);
		drawPlayer(&player, delta);

		EndDrawing();
	}


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
