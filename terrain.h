#ifndef SKIRMISH_TERRAIN
#define SKIRMISH_TERRAIN

#include "SDL.h"

#define MAP_SIZE 2048 
#define HALF_MAP_SIZE 1024
#define NOISE 1.5f

void generate_fractal_terrain();
float height_at(int, int);
SDL_Surface *print_terrain();
extern SDL_Surface *screen;

#endif
