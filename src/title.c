#include "raylib.h"

const float frameTime = 0.40f; // for flashing of start game prompt, also used by static aliens
float frameTimeElapsed = 0.0f;
int frame = 0;

const char startPromptText[] = "Start Game";
const char startButtonText[] = "<Press Space>";
const char gameNameText[] = "Low Budget Invaders";

const char controls[] = "Spacebar to fire, left and right arrow to move.";
const char instructions[] = "Destroy aliens while avoiding their shots.\nLose if aliens reach the bottom or you run out of lives.";

void drawTitle(float delta)
{
    float windowWidth = GetScreenWidth();
    int xoffset;
    frameTimeElapsed += delta;
    if (frameTimeElapsed > frameTime)
    {
        frame++;
        frameTimeElapsed -= frameTime;
        if (frame > 1)
        {
            frame = 0;
        }
    }

    BeginDrawing();
    ClearBackground(BLACK);
    xoffset = (windowWidth / 2.0f) - (MeasureText(gameNameText, 28) / 2.0f);
    DrawText(gameNameText, xoffset, 20.0f, 28, WHITE);

    xoffset = (windowWidth / 2.0f) - (MeasureText(startPromptText, 20) / 2.0f);
    DrawText(startPromptText, xoffset, 100.0f, 20, WHITE);

    if (frame == 0)
    {
        xoffset = (windowWidth / 2.0f) - (MeasureText(startButtonText, 20) / 2.0f);
        DrawText(startButtonText, xoffset, 125.0f, 20, GREEN);
    }

    EndDrawing();
}