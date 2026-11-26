#include "raylib.h"
#include "aliens.h"
#include "common.h"

const float frameTime = 0.40f; // for flashing of start game prompt, also used by static aliens
float frameTimeElapsed = 0.0f;
int frame = 0;

const char *startPromptText = "Start Game";
const char *startButtonText = "<Press Space>";
const char *gameNameText = "Low Budget Invaders";
const char *instructions = "Destroy aliens while avoiding their shots.\nLose if aliens reach the bottom or you\nrun out of lives.\nSpacebar to fire, left and right arrow\nto move.";

const int lowerAreaOffsetY = 450.0f;

Texture2D *titleAliens[3];
int scores[] = {40, 20, 10};

void initTitle()
{
    titleAliens[0] = getAlienTex(0);
    titleAliens[1] = getAlienTex(2);
    titleAliens[2] = getAlienTex(4);
}

void drawScoreTable()
{
    // simple score table for aliens

    for (int i = 0; i < 3; i++)
    {
        const char *text = TextFormat("Value = %d", scores[i]);
        // alien texture is 32 wide, add 32 space ahead of text
        int xAlienOffset = GetScreenWidth() - (32.0f + 32.0f + MeasureText(text, 20)) - 10.0f;
        DrawTextureRec(*titleAliens[i],
            (Rectangle){
                .x = frame * 32.0f,
                .y = 0.0f,
                .width = 32.0f,
                .height = 32.0f
            },
            (Vector2) {
                .x = xAlienOffset,
                .y = lowerAreaOffsetY + i * 32.0f
            },
            WHITE
        );
        DrawText(text, xAlienOffset + 32.0f + 32.0f, lowerAreaOffsetY + 32.0f * i, 20, WHITE);
    }

}

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

    // title area
    drawTextCentered(gameNameText, 20.0f, 30, WHITE);

    drawTextCentered(startPromptText, 200.0f, 20, WHITE);

    if (frame == 0)
    {
        drawTextCentered(startButtonText, 225.0f, 20, GREEN);
    }

    DrawText(instructions, 10.0f, lowerAreaOffsetY, 20, WHITE);

    drawScoreTable();

    EndDrawing();
}

void drawGameOverScreen(float delta, int score)
{
    BeginDrawing();

    ClearBackground(BLACK);

    drawTextCentered("Game Over", 100.0f, 40, WHITE);

    const char *scoreText = TextFormat("Score: %05d", score);
    drawTextCentered(scoreText, 200.0f, 30, WHITE);

    drawTextCentered("You can do better!", 400.0f, 20, WHITE);
    drawTextCentered("Press space to return to title screen.", 420.0f, 20, WHITE);

    EndDrawing();
}