#ifndef SKIRMISH_TERRAIN
#define SKIRMISH_TERRAIN

#include "SDL.h"

#include <gsl/gsl_vector.h>
#include <stdbool.h>

#define MAP_SIZE 2048 
#define HALF_MAP_SIZE 1024
#define NOISE 1.5f

void generate_fractal_terrain();
float height_at(int, int);
bool allowed(gsl_vector *);
SDL_Surface *print_terrain();
extern SDL_Surface *screen;

#endif
