#ifndef SKIRMISH_SELECTED_ACTIONS
#define SKIRMISH_SELECTED_ACTIONS

#include "attributes.h"
#include "player.h"

#include <gsl/gsl_vector.h>

void move_selected_units_to(gsl_vector *, PLAYERS *);
void selected_units_attack(unit *); 

#endif
