#include "units/states/waiting.h"
#include "units/states/attack.h"

#include "collision.h"
#include "units/attack.h"


state waiting = { "waiting", &waiting_prepare, &waiting_update, &waiting_cleanup };

void waiting_prepare(unit *u, void *data) {}
void waiting_cleanup(unit *u) {}

bool waiting_update(PLAYERS *players, camera *camera, unit *u) {
  if(u->state->next != NULL) {
    return false;
  }

  unit *to_attack = NULL;
  for(int p = 0; p < players->num; p++) {
    player *pl = players->players[p];
    if(u->player == pl)
      continue;

    for(int d = 0; d < pl->num_divisions; d++) {
      division *div = pl->divisions[d];

      for(int i = 0; i < div->size; i++) {
        to_attack = div->units[i];
        if(bounding_circle_collision(u->position, unit_range(u),
              to_attack->position, to_attack->collision_radius))
          goto found_unit_to_attack;

      }
    }

    to_attack = NULL;
  }

found_unit_to_attack:
  if(to_attack != NULL) {
    push_unit_state(u, &attack, to_attack);
  }

  return true;
}
