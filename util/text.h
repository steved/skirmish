#ifndef SKIRMISH_TEXT
#define SKIRMISH_TEXT

#include "SDL_ttf.h"

TTF_Font *font;
int init_ttf();
void close_ttf();
SDL_Surface *draw_text(char *);
int font_height();

#endif
