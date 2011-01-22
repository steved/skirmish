#ifndef SKIRMISH_DISPLAY
#define SKIRMISH_DISPLAY

#include "SDL_gfxPrimitives.h"
#include "text.h"
#include "player.h"
#include "camera.h"

#define WIDTH 640
#define HEIGHT 480

SDL_Surface *display_game(camera *camera, player **players, int len);
void display_unit(SDL_Surface *surface, camera *camera, unit *unit, uint32_t color);

#endif
