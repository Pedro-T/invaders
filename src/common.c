#include "raylib.h"

// common utility functions and some central configs

Color globalTint = WHITE;

void setGlobalTint(Color c)
{
    globalTint = c;
}

void getGlobalTint()
{
    return globalTint;
}

void drawTextCentered(const char *text, float yPos, int fontSize, Color color)
{
    DrawText(
        text,
        GetScreenWidth() / 2.0f - (MeasureText(text, fontSize) / 2.0f),
        yPos,
        fontSize,
        color
    );
}