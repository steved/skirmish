#ifndef SKIRMISH_WAITING
#define SKIRMISH_WAITING

#include "unit_state.h"

extern state waiting;
void waiting_prepare(unit *, void *);
void waiting_cleanup(unit *);
bool waiting_update(PLAYERS *, camera *, unit *);

#endif
