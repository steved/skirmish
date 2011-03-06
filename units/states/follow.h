#ifndef SKIRMISH_FOLLOW
#define SKIRMISH_FOLLOW

#include "unit_state.h"

extern state follow;
void follow_prepare(unit *, void *);
bool follow_update(PLAYERS *, camera *, unit *);
void follow_cleanup(unit *);

#endif
