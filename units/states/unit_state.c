#include "unit_state.h"

void push_unit_state(unit *u, state *st, void *data) {
  u->state = ll_add_to_bottom(u->state, st, data);
}

void pop_unit_state(unit *u) {
  state *current_state = (state *) u->state->value;
  current_state->cleanup(u);
  u->state = ll_remove(u->state, current_state);

  if(u->state != NULL) {
    current_state = (state *) u->state->value;
    current_state->prepare(u, u->state->extra_data);
  }
}
