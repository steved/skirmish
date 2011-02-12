#ifndef SKIRMISH_FILE_LOAD
#define SKIRMISH_FILE_LOAD

#include "player.h"

typedef struct players {
  int num;
  player **players;
} PLAYERS;

PLAYERS *read_file(char *name);

#endif
