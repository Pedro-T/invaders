#include "raylib.h"
#include "raymath.h"

#include "player.h"

const float playerStartPosX = 350.0f;
const float playerStartPosY = 520.0f;

const float livesOffsetY = 560.0f;
const float livesOffsetX = 50.0f;

Texture2D playerTexture;
Texture2D dyingAnimation;
const char *playerTexturePath = "player.png";
const char *dyingTexturePath = "player_death.png";

const float playerFrameTime = 0.3f;
float playerFrameTimeCurrent = 0.0f;

void loadPlayerResources()
{
	playerTexture = LoadTexture(playerTexturePath);
	dyingAnimation = LoadTexture(dyingTexturePath);
}

void unloadPlayerResources()
{
	UnloadTexture(playerTexture);
}

void drawExtraLives(int lives)
{
	int currOffset = livesOffsetX;
	for (int i = 0; i < lives; i++)
	{
		DrawRectangle(currOffset, livesOffsetY, 40.0f, 15.0f, GREEN);
		currOffset += livesOffsetX;
	}
}

void resetPlayerPosition(Player *p)
{
	p->rect = (Rectangle) {
		.x = playerStartPosX,
		.y = playerStartPosY,
		.width = 72.0f,
		.height = 16.0f
	};
}

void drawPlayer(Player *p, float delta)
{
	if (!p)
	{
		return;
	}

	switch (p->state)
	{
		case ALIVE:
			int direction = 0;
			if (IsKeyDown(KEY_LEFT))
			{
				direction = -1;
			}
			else if (IsKeyDown(KEY_RIGHT))
			{
				direction = 1;
			}
			float newX = p->rect.x + (direction * PLAYER_SPEED * delta);
			newX = Clamp(newX, 0.0f, GetScreenWidth() - p->rect.width);

			p->rect.x = newX;
			DrawTexture(playerTexture, p->rect.x, p->rect.y, GREEN);
			break;
		case DYING:
			playerFrameTimeCurrent += delta;
			if (playerFrameTimeCurrent >= playerFrameTime)
			{
				p->frame++;
				playerFrameTimeCurrent -= playerFrameTime;
			}
			if (p->frame >= 4)
			{
				p->state = DEAD;
				p->frame = 0;
				break;
			}
			DrawTextureRec(
				dyingAnimation,
				(Rectangle) {
					.x = p->frame * 72.0f,
					.y = 0.0f,
					.height = 32.0f,
					.width = 72.0f
				},
				(Vector2) {
					p->rect.x,
					p->rect.y - 16.0f // reg player sprite is 16px tall, explosion is 32px
				},
				GREEN
			);
			break;
	}
	

}
