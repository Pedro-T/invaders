#include "aliens.h"
#include "projectiles.h"

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

const int scoreValues[] = {40, 40, 20, 20, 10, 10}; // each row from top downward aliens have same value


void initAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS])
/**
 * create 2d grid of aliens. sets initial rects and offsets
 * grid is array of rows, since all aliens in the same row are the same type
 */
{
    for (int i = 0; i < ALIEN_ROWS; i++)
    {
        // set alien archetype here later on


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
                .value = scoreValues[i]
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
            DrawRectangle(alien.rect.x, alien.rect.y, alien.rect.width, alien.rect.height, BLUE);
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

