#ifndef SKIRMISH_PLAYER
#define SKIRMISH_PLAYER

#include "attributes.h"
#include <stdint.h>

typedef struct division {
  unit **units;
  int size;
} division;

typedef struct player {
  const char *name;
  uint32_t color;
  //bool human;

  division **divisions;
  int num_divisions;

  unit **units;
  int num_units;
} player;


player *create_human_player(const char *name, int units);
player *create_ai_player(int units);
void remove_player(player *player);

#endif
