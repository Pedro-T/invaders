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
#define ALIEN_SHOTS_POOL_SIZE 10
#define ALIEN_ATTACK_INTERVAL 0.80f
#define PLAYER_SHOT_MIN_TIME 0.5f
#define BONUS_ALIEN_INTERVAL 20.0f

int score = 0;
float alienAttackTimer = 0.0f;
int alienDirection = 1;
float alienMoveTimer = 0.0f;
float bonusAlienTimer = 0.0f;
float playerShotTimer = PLAYER_SHOT_MIN_TIME;

float alienMoveInterval = 0.70f;
const float minAlienMoveInterval = 0.10f;
float speedIncreasePerAlien;

int destroyedCounter = 0;



Alien aliens[ALIEN_ROWS][ALIEN_COLS];
AlienProjectile alienShots[ALIEN_SHOTS_POOL_SIZE];

Alien bonusAlien = {0};

Player player;
PlayerProjectile playerShot;

typedef enum GameState {
	TITLE,
	PLAYING,
	RESETTING,
	GAME_OVER
} GameState;

GameState state = TITLE;

void initPlayer(int lives)
{
	player = (Player) {
		.lives = lives,
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

void reset()
{
	initAliens(aliens);
	initPlayer(player.lives);
	resetBonusAlien(&bonusAlien, false);
	initAlienShots();
	initBunkers();
	resetPlayerPosition(&player);
	alienMoveInterval = 0.70f;
	alienDirection = 1;
	destroyedCounter = 0;
	alienAttackTimer = 0.0f;
	alienMoveTimer = 0.0f;
	bonusAlienTimer = 0.0f;
}


void restart()
{
	initPlayer(3);
	score = 0;
	reset();
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
			if (player.state == ALIVE)
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
					destroyedCounter++;
					alienMoveInterval -= speedIncreasePerAlien;
					playHit();
					return;

					if (destroyedCounter >= ALIEN_ROWS * ALIEN_COLS)
					{
						reset();
						return;
					}
				}
			}
		}

		if (bonusAlien.alive)
		{
			bool hitBonus = CheckCollisionRecs(bonusAlien.rect, p->rect);
			if (hitBonus)
			{
				bonusAlien.alive = false;
				score += bonusAlien.value;
				p->active = false;
				playHit();
				return;
			}
		}
		
		bool hitBunker = checkBunkerHits(p->rect);
		if (hitBunker)
		{
			p->active = false;
			playHit();
			return;
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
	state = GAME_OVER;
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

	bonusAlien.alive = false;

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
	bonusAlienTimer += delta;
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
	if (bonusAlienTimer > BONUS_ALIEN_INTERVAL)
	{
		bonusAlienTimer -= BONUS_ALIEN_INTERVAL;
		resetBonusAlien(&bonusAlien, true);
		bonusAlien.alive = true;
	}

	moveBonusAlien(&bonusAlien, delta);
	
	if (alienMoveTimer > alienMoveInterval)
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
	drawBonusAlien(&bonusAlien);

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
				restart();
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
	speedIncreasePerAlien = ((alienMoveInterval - minAlienMoveInterval) / (ALIEN_ROWS * ALIEN_COLS));

	initTitle();

	#if defined(PLATFORM_WEB)
		emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
		SetTargetFPS(60);
		while (!WindowShouldClose())
		{
			UpdateDrawFrame();		
		}
	#endif
	unloadResources();

	CloseWindow();
	return 0;
}
