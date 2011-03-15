#include "units/states/move_to.h"
#include "units/states/util.h"
#include "units/states/waiting.h"

#include "units/move.h"

#include <assert.h>

state move_to = { "move_to", &move_to_prepare, &move_to_update, &move_to_cleanup };

void move_to_prepare(unit *u, void *data) {
  u->state_data.vector = (gsl_vector *) data;
  push_unit_state(u, &waiting, NULL);
}

void move_to_cleanup(unit *u) {
  gsl_vector_free(u->state_data.vector);
}

bool move_to_update(PLAYERS *players, camera *camera, unit *u) {
  // if there is another move state pending
  // immediately switch to that
  if(is_movement_state(u->state->next)) {
    return false;
  }

  return !move_unit_towards(u, u->state_data.vector, players);
}
