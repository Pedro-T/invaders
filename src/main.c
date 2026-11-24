#include "raylib.h"
#include "common.h"
#include "player.h"
#include "projectiles.h"
#include "aliens.h"
#include "title.h"
#include "bunker.h"
#include "audio.h"

#if defined(PLATFORM_WEB)
	#include <emscripten/emscripten.h>
#endif

#include <stddef.h>
#include "resource_dir.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define ALIEN_MOVE_INTERVAL 0.70f
#define ALIEN_SHOTS_POOL_SIZE 10
#define ALIEN_ATTACK_INTERVAL 0.80f
#define PLAYER_SHOT_MIN_TIME 0.5f

int score = 0;
float alienAttackTimer = 0.0f;
int alienDirection = 1;
float alienMoveTimer = 0.0f;
float playerShotTimer = PLAYER_SHOT_MIN_TIME;

Alien aliens[ALIEN_ROWS][ALIEN_COLS];
AlienProjectile alienShots[ALIEN_SHOTS_POOL_SIZE];

Player player;
PlayerProjectile playerShot;

typedef enum GameState {
	TITLE,
	PLAYING,
	GAME_OVER
} GameState;

GameState state = TITLE;

void initPlayer()
{
	player = (Player) {
		.lives = 3,
		.state = ALIVE,
		.frame = 0
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
	playerShotTimer = 0.0f;
}

void initAlienShots()
{
	for (int i = 0; i < ALIEN_SHOTS_POOL_SIZE; i++)
	{
		alienShots[i].active = false;
		alienShots[i].rect.width = 10.0f;
		alienShots[i].rect.height = 18.0f;
	}
}

void playerTakeHit()
{
	initAlienShots();
	playPlayerDestruction();
	player.state = DYING;
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
				playerTakeHit();
			}
			else
			{
				destroyed = checkBunkerHits(alienShots[i].rect);
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
					playHit();
				}
			}
		}
		bool hitBunker = checkBunkerHits(p->rect);
		if (hitBunker)
		{
			p->active = false;
			playHit();
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
	// set all aliens to inactive
	for (int i = 0; i < ALIEN_ROWS; i++)
	{
		for (int j = 0; j < ALIEN_COLS; j++)
		{
			aliens[i][j].alive = false;
		}
	}

	// same for projectiles
	for (int i = 0; i < ALIEN_SHOTS_POOL_SIZE; i++)
	{
		alienShots[i].active = false;
	}

	// player
	player.state = DEAD;
	playerShot.active = false;
	playGameOver();
}

void loadResources()
{
	loadPlayerResources();
	loadAlienResources();
	loadProjectileResources();
	loadBunkerResources();
}

void unloadResources()
{
	unloadAlienResources();
	unloadPlayerResources();
	unloadProjectileResources();
	unloadBunkerResources();
	unloadAudio();
}

void processGameTick(float delta)
{
	alienMoveTimer += delta;
	playerShotTimer += delta;

	// if player has finished "dying" i.e. animation done and still has lives, check lives left and respawn or move to game over
	if (player.state == DEAD)
	{
		if (player.lives > 1)
		{
			player.lives--;
			resetPlayerPosition(&player);
			player.state = ALIVE;
		}
		else
		{
			gameOver();
			state = GAME_OVER;
		}
	}

	// player updates
	if (IsKeyDown(KEY_SPACE) && !playerShot.active && player.state == ALIVE && playerShotTimer > PLAYER_SHOT_MIN_TIME)
	{
		playerShot.rect = (Rectangle){
			.x = player.rect.x + (player.rect.width / 2.0f), 
			.y = player.rect.y, 
			.width = 5.0f, 
			.height = 16.0f
		};
		playFireSound();
		playerShot.active = true;
		playerShotTimer = 0.0f;
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
	drawTextCentered(TextFormat("Score: %d", score), 10.0f, 20, WHITE);
	DrawLineV((Vector2){0, 550}, (Vector2){WINDOW_WIDTH, 550}, GREEN); // line under player above lives
	drawExtraLives(player.lives);

	drawBunkers();

	// player
	drawPlayerProjectile(playerShot);
	drawPlayer(&player, delta);

	// aliens
	drawAliens(aliens);
	drawAliensProjectiles(alienShots, ALIEN_SHOTS_POOL_SIZE);

	EndDrawing();
}

void UpdateDrawFrame()
{
	float delta = GetFrameTime();
	switch (state)
	{
		case TITLE:
			
			if (IsKeyPressed(KEY_SPACE))
			{
				state = PLAYING;
				initAliens(aliens);
				initPlayer();
				initAlienShots();
				initBunkers();
				resetPlayerPosition(&player);
				score = 0;
				alienDirection = 1;
			}
			drawTitle(delta);
			break;
		case PLAYING:
			processGameTick(delta);
			break;
		case GAME_OVER:
			if (IsKeyPressed(KEY_SPACE))
			{
				state = TITLE;
			}
			drawGameOverScreen(delta, score);
			break;
	}
}

int main ()
{

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Low Budget Invaders");
	SearchAndSetResourceDir("resources");
	initAudio();

	// this is for blocking spacebar scrolling on web while playing the game
	#if defined(PLATFORM_WEB)
	EM_ASM({
		window.addEventListener('keydown', function(e) {
			if (e.code === "Space") {
				e.preventDefault()
			}
		}, false);
	});
	#endif

	loadResources();

	state = TITLE;

	initTitle();

	#if defined(PLATFORM_WEB)
		emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
		SetTargetFPS(60);
	#endif
	
	while (!WindowShouldClose())
	{
		UpdateDrawFrame();		
	}

	unloadResources();

	CloseWindow();
	return 0;
}
