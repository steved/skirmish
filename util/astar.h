#ifndef SKIRMISH_ASTAR
#define SKIRMISH_ASTAR

#include "player.h"

#include "util/linked_list.h"
#include "util/nav_mesh.h"

ll_node *shortest_path(PLAYERS *, gsl_vector *, gsl_vector *);
ai_node *find_closest_node(gsl_vector *);

#endif
