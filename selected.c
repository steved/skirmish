#include "selected.h"
#include "units.h"

#include "util/linked_list.h"

#include <assert.h>

ll_node *selected_head = NULL;

void select_unit(unit *unit) {
  selected_head = ll_add(selected_head, unit);
}

void select_units(unit **units, int len) {
  for(int i = 0; i < len; i++) {
    select_unit(units[i]);
  }
}

void select_division(division *division) {
  select_units(division->units, division->size);
}

void unselect_all() {
  selected_head = ll_clear(selected_head);
}

void unselect_unit(unit *unit) {
  selected_head = ll_remove(selected_head, unit);
}

bool selected(unit *unit) {
  return ll_include(selected_head, unit);
}

bool units_selected() {
  return selected_head != NULL;
}

// this should probably be moved elsewhere
#include "util/astar.h"
#include "units/states/move_to.h"
#include "units/states/move_to_node.h"

void move_selected_units_to(gsl_vector *vector) {
  ll_node *node = selected_head;
  unit *u;
  while(node) {
    u = (unit *) node->value;
    print_unit(u);

    ll_node *path = shortest_path(u->vector, vector);
    if(path == NULL) { // couldn't get an astar path, so just go straight-line??
      push_unit_state(u, &move_to, vector);
    } else {
      printf("pushing move_to_node\n");
      push_unit_state(u, &move_to_node, path);
    }

    node = node->next;
  }
}
