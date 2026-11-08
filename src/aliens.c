#include "aliens.h"
#include "projectiles.h"

#include "raylib.h"
#include <stdlib.h>

const int aliensMoveResetFrame = 1; // two frames of animation 0, 1
const int scoreValues[] = {40, 40, 20, 20, 10, 10}; // each row from top downward aliens have same value
const int frameWidth = 32;


// alien textures are 64x32 with two frames horizontally
Texture2D alienA;
Texture2D alienB;
Texture2D alienC;
Texture2D *textures[6];

void loadAlienResources()
{
    alienA = LoadTexture("alienA.png");
    alienB = LoadTexture("alienB.png");
    textures[0] = &alienA;
    textures[1] = &alienA;
    textures[2] = &alienB;
    textures[3] = &alienB;
    textures[4] = &alienA;
    textures[5] = &alienA;
}

void unloadAlienResources()
{
    UnloadTexture(alienA);
    UnloadTexture(alienB);
    UnloadTexture(alienC);
}



void initAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS])
{
    // aliens are 32x32 and sit in the center of 64x64 cells
    for (int i = 0; i < ALIEN_ROWS; i++)
    {
        // offset going down from top of grid for this row
        float offsetY = ALIEN_BLOCK_OFFSET_Y + i * ALIEN_CELL_SIZE;

        for (int j = 0; j < ALIEN_COLS; j++)
        {
            float xPos = ALIEN_BLOCK_OFFSET_X + j * ALIEN_CELL_SIZE;
            aliens[i][j] = (Alien){
                .rect = (Rectangle) {
                    .x = xPos,
                    .y = offsetY,
                    .width = 32,
                    .height = 32
                },
                .alive = true,
                .value = scoreValues[i],
                .texture = textures[i],
                .frame = 0
            };
        }
    }

}


void moveAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS], int direction)
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
            a->rect.x += direction * ALIEN_MOVE_AMOUNT;
            a->frame++;
            if (a->frame > aliensMoveResetFrame)
            {
                a->frame = 0;
            }
        }
    }
}


bool shiftAliensDown(Alien aliens[ALIEN_ROWS][ALIEN_COLS])
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
            int newPos = a->rect.y + SHIFT_DOWN_AMOUNT;
            if ((newPos + a->rect.height) >= ALIEN_GOAL_Y)
            {
                return true;
            }
            a->rect.y = newPos;
        }
    }
    return false;
}


bool aliensHitBound(Alien aliens[ALIEN_ROWS][ALIEN_COLS])
{
    float xMin = GetScreenWidth();
    float xMax = 0.0f;
    for (int i = 0; i < ALIEN_ROWS; i++)
    {
        for (int j = 0; j < ALIEN_COLS; j++)
        {
            Alien a = aliens[i][j];
            if (!a.alive)
            {
                continue;
            }
            if (a.rect.x < xMin)
            {
                xMin = a.rect.x;
            }
            if (a.rect.x + ALIEN_CELL_SIZE > xMax)
            {
                xMax = a.rect.x;
            }
        }
    }
    return xMax > GetScreenWidth()- BOUND_OFFSET || xMin < 0 + BOUND_OFFSET;
}


void drawAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS])
{
    for (int i = 0; i < ALIEN_ROWS; i++)
    {
        for (int j = 0; j < ALIEN_COLS; j++)
        {
            Alien alien = aliens[i][j];
            if (!alien.alive)
            {
                continue;
            }
            // frames are stored in one png. each frame is 32px wide, so use the frame counter to offset from the left
            DrawTextureRec(
                *(alien.texture), 
                (Rectangle){
                    .x = 32*alien.frame,
                    .y = 0.0f, 
                    .width = 32.0f,.height = 32.0f
                }, 
                (Vector2){
                    .x = alien.rect.x, 
                    .y = alien.rect.y
                }, 
                WHITE);
        }
    }
}

bool aliensFireShot(Alien aliens[ALIEN_ROWS][ALIEN_COLS], AlienProjectile *shot)
// find all aliens eligible to fire a shot (first alien from bottom in each column)
// choose one at random to fire
{
    Alien *eligible[ALIEN_COLS] = {0};
    int foundCount = 0;
    for (int j = 0; j < ALIEN_COLS; j++)
    {
        for (int i = ALIEN_ROWS - 1; i >= 0; i--)
        {
            if (aliens[i][j].alive)
            {
                eligible[foundCount] = &aliens[i][j];
                foundCount++;
                break;
            }
        }
        
    }

    if (foundCount == 0)
    {
        return false;
    }

    int index = rand() % foundCount;
    shot->rect.x = eligible[index]->rect.x + eligible[index]->rect.width / 2.0;
    shot->rect.y = eligible[index]->rect.y + eligible[index]->rect.height;
    shot->active = true;
    return true;
}

