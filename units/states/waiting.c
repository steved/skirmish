#include "units/states/waiting.h"

state waiting = { "waiting", &waiting_prepare, &waiting_update, &waiting_cleanup };

void waiting_prepare(unit *u, void *data) {}
void waiting_cleanup(unit *u) {}

bool waiting_update(PLAYERS *players, camera *camera, unit *u) {
  if(u->state->next != NULL) {
    return false;
  }
  return true;
}
