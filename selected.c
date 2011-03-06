#include "selected.h"

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
