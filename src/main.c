#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"

#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PLAYER_SHOT_SPEED 150.0f

const float playerSpeed = 200.0f;


struct player {
	Vector2 position;
	int score;
	int lives;
	int width;
};

typedef struct player Player;

void updatePlayerProjectile(Vector2 *position, float delta)
{
	if (position->y < 0)
	{
		position->x = -1.0f;
		position->y = -1.0f;
		return;
	}
	float newY = position->y - (PLAYER_SHOT_SPEED * delta);
	position->y = newY;
}

void drawPlayerProjectile(Vector2 position)
{
	if (position.x < 0)
	{
		return;
	}
	DrawLineV(position, Vector2Add(position, (Vector2){0.0f, 10.0f}), WHITE);
}


void drawPlayer(Player *p, float delta)
{
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

	Vector2 playerShot = (Vector2){-1.0f, -1.0f};

	
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float delta = GetFrameTime();

		ClearBackground(BLACK);

		if (IsKeyDown(KEY_SPACE) && playerShot.y < 0)
		{
			playerShot = Vector2Add(player.position, (Vector2){player.width / 2.0f, 0.0f});
		}

		DrawText(TextFormat("Score: %d", player.score), 350, 10, 20, WHITE);

		DrawLineV((Vector2){0, 550}, (Vector2){WINDOW_WIDTH, 550}, GREEN);

		updatePlayerProjectile(&playerShot, delta);
		drawPlayerProjectile(playerShot);
		drawPlayer(&player, delta);

		EndDrawing();
	}


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}   
