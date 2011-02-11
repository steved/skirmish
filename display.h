#ifndef SKIRMISH_DISPLAY
#define SKIRMISH_DISPLAY

#include "SDL.h"
#include "attributes.h"
#include "camera.h"

#define WIDTH 640
#define HEIGHT 480

void display_unit(SDL_Surface *, camera *, unit *, uint32_t, float);

#endif
