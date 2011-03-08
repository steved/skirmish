#ifndef SKIRMISH_PLAYER
#define SKIRMISH_PLAYER

#include "attributes.h"
#include "camera.h"

#include <stdint.h>
#include <stdbool.h>

#define MAX_PLAYERS 4

typedef struct division_structure {
  // enum of how it's layed out AKA (infantry, cavalry, artillery) or (cavalry, artiller, infantry), etc.
  // which is then sorted when loaded

  int num_per_row; // how many units per row
  int row_padding; // how much padding between each row
} structure;

typedef struct division {
  int size;
  unit **units;
  structure structure;
} division;

typedef struct player {
  const char *name;
  bool human;
  uint32_t color;
  // how much padding around each division both x + y
  int column_padding; 

  division **divisions;
  int num_divisions;
} player;

typedef struct players {
  int num;
  player **players;
  bool setup; // have the players + units been set-up?
} PLAYERS;

player *create_human_player(const char *, int);
player *create_ai_player(int);
void remove_player(player *);
bool select_units_at(bool, int, int, camera *, PLAYERS *players);

#endif
