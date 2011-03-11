#ifndef SKIRMISH_UNIT_MOVE
#define SKIRMISH_UNIT_MOVE

#include "attributes.h"
#include "player.h"

#include <gsl/gsl_vector.h>

bool move_unit_towards(unit *, gsl_vector *, PLAYERS *);

#endif
