#ifndef SKIRMISH_EVENTS
#define SKIRMISH_EVENTS

#include "camera.h"
#include "ui/ui_state.h"
#include <stdbool.h>

void poll_for_events(camera *, PLAYERS *players, ui_state *);
extern bool game_running;

#endif
