#ifndef SKIRMISH_EVENTS
#define SKIRMISH_EVENTS

#include "camera.h"
#include "ui/ui_state.h"
#include <stdbool.h>

void poll_for_events(camera *, player **, int, ui_state *);
extern void toggle_pause();
extern bool paused;

#endif
