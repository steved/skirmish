#ifndef SKIRMISH_SELECTED
#define SKIRMISH_SELECTED

#include "attributes.h"
#include "player.h"

#include <stdbool.h>

typedef struct selected_node {
  unit *unit;
  struct selected_node *next;
} selected_node;

void select_unit(unit *);
void select_units(unit **, int);
void select_division(division *);
void unselect_all();
void unselect_unit(unit *);
bool selected(unit *);
void move_selected_units_to(gsl_vector *);

#endif
