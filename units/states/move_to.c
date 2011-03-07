#include "units/states/move_to.h"

#include "units.h"

state move_to = { "move_to", &move_to_prepare, &move_to_update, &move_to_cleanup };

void move_to_prepare(unit *u, void *data) {
  u->state_data.vector = gsl_vector_calloc(3);
  gsl_vector_memcpy(u->state_data.vector, (gsl_vector *) data);
}

void move_to_cleanup(unit *u) {
  gsl_vector_free(u->state_data.vector);
}

bool move_to_update(PLAYERS *players, camera *camera, unit *u) {
  return !move_unit_towards(u, u->state_data.vector, players);
}
