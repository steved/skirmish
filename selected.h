#ifndef SKIRMISH_SELECTED
#define SKIRMISH_SELECTED

#include "attributes.h"
#include "player.h"

#include "util/linked_list.h"

#include <stdbool.h>

extern ll_node *selected_head;

void select_unit(unit *);
void select_units(unit **, int);
void select_division(division *);
void unselect_all();
void unselect_unit(unit *);
bool selected(unit *);

#endif
