#ifndef SKIRMISH_TEXT
#define SKIRMISH_TEXT

#include "SDL_ttf.h"

TTF_Font *font;
int init_ttf();
SDL_Surface *draw_text(char *text);
int font_height();

#endif
