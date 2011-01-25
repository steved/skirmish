#ifndef SKIRMISH_TERRAIN
#define SKIRMISH_TERRAIN

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"

#define MAP_SIZE 2048 
#define HEIGHT_SCALE 2 

float random_float(float);
void generate_fractal_terrain();
float diamond(float, float, float, float, float);
SDL_Surface *print_terrain();
extern SDL_Surface *screen;

#endif
