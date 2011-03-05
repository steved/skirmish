#ifndef SKIRMISH_SELECTED
#define SKIRMISH_SELECTED

#include "attributes.h"
#include "player.h"

#include <stdbool.h>

void select_unit(unit *);
void select_units(unit **, int);
void select_division(division *);
void unselect_all();
void unselect_unit(unit *);
bool selected(unit *);

void move_selected_units_to(gsl_vector *, PLAYERS *players);
void selected_units_attack(unit *);

#endif
