#ifndef SKIRMISH_DISPLAY
#define SKIRMISH_DISPLAY

#include "SDL.h"
#include "attributes.h"
#include "player.h"
#include "camera.h"
#include "selected.h"

#define WIDTH 512
#define HEIGHT 512

SDL_Surface *display_game(SDL_Surface *, camera *, player **, int);
void display_unit(SDL_Surface *, camera *, unit *, uint32_t);
extern SDL_Surface *screen;

#endif
