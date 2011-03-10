#ifndef SKIRMISH_COLLISION
#define SKIRMISH_COLLISION

#include "util/terrain.h"

#include <gsl/gsl_vector.h>
#include <stdbool.h>

bool bounding_circle_collision(gsl_vector *, double, gsl_vector *, double);
bool allowed_on_terrain(gsl_vector *);
bool hit_water(int, int, int, int);
bool hit_unit(bool[][MAP_SIZE], int, int, int, int);
float vector_euclidian_distance(gsl_vector *, gsl_vector *);

#endif
