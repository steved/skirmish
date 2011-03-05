#ifndef SKIRMISH_ATTACK
#define SKIRMISH_ATTACK

#include "unit_state.h"

extern state attack;
void attack_prepare(unit *, void *);
void attack_cleanup(unit *);
bool attack_update(PLAYERS *, camera *, unit *);

#endif
