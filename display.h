#ifndef SKIRMISH_DISPLAY
#define SKIRMISH_DISPLAY

#include "SDL_gfxPrimitives.h"
#include "text.h"
#include "units.h"
#include "camera.h"

#define WIDTH 640
#define HEIGHT 480

SDL_Surface *display_game(unit **units, int len);
void display_unit(SDL_Surface *surface, unit *unit);

#endif
