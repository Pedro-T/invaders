#pragma once

#include "raylib.h"

Color getGlobalTint();
void setGlobalTint(Color c);
void drawTextCentered(const char *text, float yPos, int fontSize, Color color);