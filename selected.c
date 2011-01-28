#include "selected.h"

#include <assert.h>

selected_node *selected_head = NULL;

void select_unit(unit *unit) {
  selected_node *node = malloc(sizeof(selected_node));
  assert(node != NULL);
  node->unit = unit;
  node->next = selected_head;
  selected_head = node;
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
  selected_node *node = selected_head;
  while(node) {
    selected_node *next = node->next;
    free(node);
    node = next;
  }
  free(node);
  selected_head = NULL;
}

void unselect_unit(unit *unit) {
  selected_node *prev = 0;
  selected_node *node = selected_head;
  while(node) {
    if(node->unit == unit) {
      prev->next = node->next;
      free(node);
      return;
    }
    node = node->next;
  }
}

bool selected(unit *unit) {
  selected_node *node = selected_head;
  while(node) {
    if(node->unit == unit)
      return true;
    node = node->next;
  }
  return false;
}
