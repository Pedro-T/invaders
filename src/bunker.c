#include "raylib.h"
#include "bunker.h"
#include "projectiles.h"

#define BRICK_ROWS 3
#define BRICK_COLS 6

const float bunkerPosY = 450.0f;
const int brickWidth = 16.0f;
const int bunkerLayout[BRICK_ROWS][BRICK_COLS] = {
    {0, 1, 1, 1, 1, 0},
    {1, 1, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 1}
};

Bunker bunkers[BUNKER_COUNT] = {0};
Texture2D brickTexture;

void loadBunkerResources()
{
    brickTexture = LoadTexture("brick.png");
}

void unloadBunkerResources()
{
    UnloadTexture(brickTexture);
}

void initBunkers()
{
    float screenWidth = GetScreenWidth();
    float slotSize = screenWidth / 4.0f;
    for (int i = 0; i < BUNKER_COUNT; i++)
    {
        float xPos = (slotSize * i) + (slotSize / 2.0f) - (BRICK_COLS * brickWidth) / 2;
        Bunker *b = &bunkers[i];
        b->pos = (Vector2){
            .x = xPos,
            .y = bunkerPosY
        };
        for (int j = 0; j < BRICK_ROWS; j++)
        {
            for (int k = 0; k < BRICK_COLS; k++)
            {
                b->grid[j][k] = bunkerLayout[j][k];
            }
        }
    }
}

bool checkBunkerHits(AlienProjectile *p)
{
    // don't waste time checking unless projectile is in the bunker area
    if (p->rect.y + p->rect.height < bunkerPosY)
    {
        return false;
    }

    for (int i = 0; i < BUNKER_COUNT; i++)
    {
        Bunker *b = &bunkers[i];
        for (int j = 0; j < BRICK_ROWS; j++)
        {
            for (int k = 0; k < BRICK_COLS; k++)
            {
                if (b->grid[j][k] == 0)
                {
                    continue;
                }
                Rectangle brickRect = (Rectangle){
                    .x = b->pos.x + (k * brickWidth),
                    .y = b->pos.y + (j * brickWidth),
                    .height = brickWidth,
                    .width = brickWidth
                };
                if (CheckCollisionRecs(brickRect, p->rect))
                {
                    b->grid[j][k] = 0;
                    return true;
                }
            }
        }
    }

    return false;
}

void drawBunkers()
{
    for (int i = 0; i < BUNKER_COUNT; i++)
    {
        Bunker b = bunkers[i];
        for (int j = 0; j < BRICK_ROWS; j++)
        {
            for (int k = 0; k < BRICK_COLS; k++)
            {
                if (b.grid[j][k] == 0)
                {
                    continue;
                }
                DrawTexture(
                    brickTexture,
                    b.pos.x + k * brickWidth,
                    b.pos.y + j * brickWidth,
                    WHITE);
            }
        }
    }
}