#include "aliens.h"


#define ALIEN_CELL_SIZE 64.0f

void initAliens(Alien a[ALIEN_ROWS][ALIEN_COLS])
/**
 * create 2d grid of aliens. sets initial positions and offsets
 * grid is array of rows, since all aliens in the same row are the same type
 */
{
    for (int i = 0; i < ALIEN_ROWS; i++)
    {
        // set alien archetype here later on


        // offset going down from top of grid for this row
        float offsetY = i * ALIEN_CELL_SIZE;

        for (int j = 0; j < ALIEN_COLS; j++)
        {
            a[i][j] = (Alien){(Vector2){offsetY, j * ALIEN_CELL_SIZE}, 32, 32};
        }
    }

}




