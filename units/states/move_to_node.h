#ifndef SKIRMISH_MOVE_TO_NODE
#define SKIRMISH_MOVE_TO_NODE

#include "unit_state.h"

extern state move_to_node;
void move_to_node_prepare(unit *, void *);
void move_to_node_cleanup(unit *);
bool move_to_node_update(PLAYERS *, camera *, unit *);

#endif
