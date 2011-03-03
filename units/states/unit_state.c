#include "unit_state.h"

void push_unit_state(unit *u, state *st, void *data) {
  printf("in push\n");
  u->state = ll_add_to_bottom(u->state, st, data);
}

void pop_unit_state(unit *u) {
  printf("popping unit state\n");
  state *current_state = (state *) u->state->value;
  printf("\t%s\n", current_state->name);
  printf("cleaning up current state\n");
  current_state->cleanup(u);
  printf("removing current state\n");
  u->state = ll_remove(u->state, current_state);

  printf("going to the next state\n");
  if(u->state != NULL) {
    current_state = (state *) u->state->value;
    printf("\t%s\n", current_state->name);
    current_state->prepare(u, u->state->extra_data);
  }
  printf("out\n");
}
