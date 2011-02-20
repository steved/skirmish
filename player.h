#ifndef SKIRMISH_PLAYER
#define SKIRMISH_PLAYER

#include "attributes.h"
#include "camera.h"

#include "SDL_events.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_PLAYERS 4

typedef struct division {
  int size;
  unit **units;
} division;

typedef struct player {
  const char *name;
  bool human;
  uint32_t color;

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
bool check_for_unit_at(bool, int, int, camera *, PLAYERS *players);

#endif
