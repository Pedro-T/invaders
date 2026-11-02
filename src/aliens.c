#include "aliens.h"
#include "projectiles.h"

#include "raylib.h"
#include "raymath.h"

const int scoreValues[] = {40, 40, 20, 20, 10, 10}; // each row from top downward aliens have same value


void initAliens(Alien aliens[ALIEN_ROWS][ALIEN_COLS])
/**
 * create 2d grid of aliens. sets initial positions and offsets
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
                .position = (Vector2){xPos, offsetY}, 
                .width = 32, 
                .height = 32,
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
            a->position.x += direction * ALIEN_MOVE_AMOUNT;
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
            int newPos = a->position.y + SHIFT_DOWN_AMOUNT;
            if ((newPos + a->height) >= ALIEN_GOAL_Y)
            {
                return true;
            }
            a->position.y = newPos;
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
            if (a.position.x < xMin)
            {
                xMin = a.position.x;
            }
            if (a.position.x + ALIEN_CELL_SIZE > xMax)
            {
                xMax = a.position.x;
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
            DrawRectangle(alien.position.x, alien.position.y, alien.width, alien.height, BLUE);
        }
    }
}



