#include "collision.h"
#include "player.h"
#include "terrain.h"
#include "units.h"
#include "unit_state_functions.h"

#include <stdbool.h>

void (*state_functions[5])(PLAYERS *, camera *, unit *) = { &do_wait, &do_attack, &do_charge, &do_retreat, &do_move };

void do_wait(PLAYERS *players, camera *camera, unit *u) {}
void do_attack(PLAYERS *players, camera *camera, unit *u) {}
void do_charge(PLAYERS *players, camera *camera, unit *u) {}
void do_retreat(PLAYERS *players, camera *camera, unit *u) {}
void do_move(PLAYERS *players, camera *camera, unit *u) {
  if(move_unit_towards(u, u->state.subject.vector, camera, players)) {
    u->state.current = waiting;
    u->state.subject.vector = NULL;
    printf("switching to wait state\n");
  }
}
