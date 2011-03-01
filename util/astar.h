#ifndef SKIRMISH_ASTAR
#define SKIRMISH_ASTAR

#include "nav_mesh.h"

ai_node **shortest_path(gsl_vector *, gsl_vector *);
ai_node *find_closest_node(gsl_vector *);

#endif
