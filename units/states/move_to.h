#ifndef SKIRMISH_MOVE_TO
#define SKIRMISH_MOVE_TO

#include "unit_state.h"

extern state move_to;
void move_to_prepare(unit *, void *);
void move_to_cleanup(unit *);
bool move_to_update(PLAYERS *, camera *, unit *);

#endif
