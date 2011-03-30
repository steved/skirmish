#include "units/states/unit_state.h"

void cleanup_current_state(unit *);

void push_unit_state(unit *u, state *st, void *data) {
  u->state = ll_add_to_bottom(u->state, st, data);
}

void immediate_change_to_unit_state(unit *u, state *st, void *data) {
  cleanup_current_state(u);

  u->state = ll_add(u->state, st);
  u->state->extra_data = data;
  st->prepare(u, data);
}

void pop_unit_state(unit *u) {
  cleanup_current_state(u);

  if(u->state != NULL) {
    state *current_state = (state *) u->state->value;
    current_state->prepare(u, u->state->extra_data);
  }
}

void cleanup_current_state(unit *u) {
  state *current_state = (state *) u->state->value;
  current_state->cleanup(u);
  u->state = ll_remove(u->state, current_state);
}
