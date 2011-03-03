#ifndef SKIRMISH_UNIT_STATE
#define SKIRMISH_UNIT_STATE

#include "../../attributes.h"
#include "../../camera.h"
#include "../../player.h"

#include <stdbool.h>

typedef struct state {
  const char *name;
  void (*prepare)(unit *, void *);
  bool (*update)(PLAYERS *, camera *, unit *);
  void (*cleanup)(unit *);
} state;

void push_unit_state(unit *, state *, void *);
void pop_unit_state(unit *);

#endif
