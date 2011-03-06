#include "collision.h"
#include "selected.h"
#include "units.h"

#include "ui/setup.h"
#include "ui/ui_state.h"

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

// this should all probably be moved elsewhere
#include "util/astar.h"
#include "units/states/attack.h"
#include "units/states/move_to.h"
#include "units/states/move_to_node.h"

void move_selected_units_to(gsl_vector *vector, PLAYERS *players) {
  ll_node *node = selected_head;
  unit *u;

  if(current_state == &setup_state) {
    int mid_x = 0, mid_y = 0, num = 0;
    while(node) {
      u = (unit *) node->value;
      mid_x += gsl_vector_get(u->vector, 0);
      mid_y += gsl_vector_get(u->vector, 1);
      num++;
      node = node->next;
    }
    mid_x /= num;
    mid_y /= num;

    node = selected_head;

    int offset_x, offset_y;
    while(node) {
      u = (unit *) node->value;
      offset_x = gsl_vector_get(u->vector, 0) - mid_x;
      offset_y = gsl_vector_get(u->vector, 1) - mid_y;
      //if(allowed_on_terrain(vector) &&
      //      check_for_unit_near(vector, players, u, false, false) == NULL) {

      place(u, gsl_vector_get(vector, 0) + offset_x, gsl_vector_get(vector, 1) + offset_y);
      ///}
      node = node->next;
    }
  } else { // game state
    while(node) {
      u = (unit *) node->value;

      ll_node *path = shortest_path(u->vector, vector);
      if(path == NULL) { // XXX couldn't get an astar path, so just go straight-line??
        push_unit_state(u, &move_to, vector);
      } else {
        push_unit_state(u, &move_to_node, path);
      }

      node = node->next;
    }
  }
}

// TODO: if theres more than one selected, check
// around attacked unit to find others
void selected_units_attack(unit *un) {
  // should be a better way to do this; delay a* search in
  // attack.c until update is called?
  if(current_state == &setup_state)
    return;

  ll_node *node = selected_head;
  unit *selected;
  while(node) {
    selected = (unit *) node->value;
    immedate_change_to_unit_state(selected, &attack, un);

    node = node->next;
  }
}
