#ifndef SKIRMISH_PLAYER
#define SKIRMISH_PLAYER

#include "attributes.h"

typedef struct division {
  unit **units;
  int size;
} division;

typedef struct player {
  char *name;
  //bool human;

  division **divisions;
  int num_divisions;

  unit **units;
  int num_units;
} player;



#endif
