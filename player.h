#ifndef SKIRMISH_PLAYER
#define SKIRMISH_PLAYER

#include "attributes.h"

typedef struct division {
  unit **units;
  int size;
} division;

typedef struct player {
  const char *name;
  //bool human;

  division **divisions;
  int num_divisions;

  unit **units;
  int num_units;
} player;


player *create_human_player(const char *name, int units);
player *create_ai_player(int units);
void remove_player(player *player);

static const char *AI_NAMES[] = {
  "Hannibal", "Sun Tzu", "Ramesses II", "Ambiorix",
  "Leonidas", "Philip II of Macedon", "Alexander the Great",
  "David", "Hammurabi", "Nebuchadrezzar II", "Xerxes", "Darius", 
  "Marcus Licinius Crassus", "Julius Caesar", "Scipio Africanus"
};

#endif
