#ifndef SKIRMISH_FILE_LOAD
#define SKIRMISH_FILE_LOAD

#include "player.h"

#include "util/linked_list.h"

PLAYERS *read_file(char *name);
extern ll_node *unit_types;

#endif
