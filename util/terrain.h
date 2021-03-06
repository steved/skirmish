#ifndef SKIRMISH_TERRAIN
#define SKIRMISH_TERRAIN

#include "SDL.h"
#include <gsl/gsl_vector.h>

#define MAP_SIZE 2048 
#define HALF_MAP_SIZE 1024
#define NOISE 1.5f

#define WATER 0 
#define FOREST 140
#define MOUNTAINS 150

void generate_fractal_terrain();
float height_at(int, int);
float height_at_vector(gsl_vector *);
SDL_Surface *print_terrain();
extern SDL_Surface *screen;

#endif
