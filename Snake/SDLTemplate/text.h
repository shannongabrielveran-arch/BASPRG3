#pragma once
#include "common.h"
#include "draw.h"

extern void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y);
extern SDL_Texture* loadTexture(char* filename);

void initFonts(void);
void drawText(int x, int y, int r, int g, int b, int align, const char* format, ...);